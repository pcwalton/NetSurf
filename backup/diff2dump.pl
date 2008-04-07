#!/usr/bin/perl

# Convert a commits list mail into an incremental dump 
# suitable for loading using svnadmin load.
#
# It is advisable to check the dump (and /tmp/diff2dump.tmp) for sanity 
# afterwards as it's highly likely that the mail parser and dump file builder
# aren't aware of all the types of things that can appear. It's only been
# tested with commit messages that involve simple modifications to files.
# Thus, addition, deletion and renaming of files may break, as may directory
# tree changes.
#
# Usage: diff2dump.pl <raw-mail> <path-to-working-copy> <output-file>

use warnings;
use strict;
use Digest::MD5 qw(md5_hex);
use Time::Piece; # libtime-piece-perl

if (@ARGV < 3) {
	die "Usage: diff2dump.pl <raw-mail> <path-to-working-copy> <output-file>\n";
}

my $pwd = `pwd`;
chomp $pwd;

my $email = $ARGV[0];
my $working_copy = $ARGV[1];
my $dump_file = $ARGV[2];

chdir $working_copy;

my $changes = `svn status`;
chomp $changes;

die "ERROR: working copy has uncommitted changes\n" if ($changes ne "");

my $in_log = 0;
my $in_modified = 0;
my $in_diff = 0;

my $first_log_line = 1;

my $author = "";
my $date = "";
my $revision = "";
my $log = "";
my $diff = "";

my @modified;

open EML,"<$email" or die "ERROR: Failed opening email\n";
{
	# Strip headers -- they're CRLF separated
	local $/ = "\r\n";
	while (<EML>) {
		chomp $_;
	
		last if ($_ eq "");
	}
}

foreach my $line (<EML>) {
	chomp $line;

	if ($in_log == 1) {
		if ($line =~ /^Modified:/) {
			$in_log = 0;
			$in_modified = 1;
		} else {
			if ($first_log_line == 1) {
				$first_log_line = 0;
			} else {
				$log .= "\n";
			}

			$log .= $line;
		}
	} elsif ($in_modified == 1) {
		if ($line eq "") {
			$in_modified = 0;
			$in_diff = 1;
		} else {
			$line =~ s/^\s+//;
			$line =~ s/\s+$//;
			push(@modified, $line);
		}
	} elsif ($in_diff == 1) {
		if ($line =~ /^____/) {
			$in_diff = 0;
		} else {
			$diff .= "$line\n";
		}
	} else {
		if ($line =~ /^Author: /) {
			$author = substr($line, length("Author: "));
		} elsif ($line =~ /^Date: /) {
			$date = substr($line, length("Date: "));
		} elsif ($line =~ /^New Revision: /) {
			$revision = substr($line, length("New Revision: "));
		} elsif ($line =~ /^Log:/) {
			$in_log = 1;
		}
	}
}

close EML;

my $wc_url = "";
my $wc_uuid = "";
my @info = `svn info`;
foreach my $line (@info) {
	next if ($line !~ /^URL:/ && $line !~ /^Repository UUID:/);

	if ($line =~ /^URL:/) {
		$wc_url = (split(' ', $line))[1];
	} else {
		$wc_uuid = (split(' ', $line))[2];
	}
}
die "ERROR: couldn't read working copy URL\n" if ($wc_url eq "");
die "ERROR: couldn't read working copy UUID\n" if ($wc_uuid eq "");

die "ERROR: working copy not of trunk\n" if (substr($wc_url, length($wc_url) - 5) ne "trunk");

open DIFF,">/tmp/diff2dump.tmp" or die "Failed opening temporary file\n";
print DIFF $diff;
close DIFF;

`patch --strip 1 --input=/tmp/diff2dump.tmp`;

open DUMP,">$pwd/$dump_file" or die "Failed opening dump file\n";

print DUMP "SVN-fs-dump-format-version: 2\n";
print DUMP "\nUUID: $wc_uuid\n";
print DUMP "\n";

# This is annoying -- svnmailer outputs local time, but doesn't include the 
# offset from UTC. Thus, if the time of the machine on which svnmailer is 
# running isn't UTC, the timestamp differ from reality.
my $proptime = Time::Piece->strptime($date, "%a%n%b%n%d%n%T%n%Y");
# We get to guess at the microseconds part -- assume zero
my $propdate = $proptime->strftime("%Y-%m-%dT%H:%M:%S.000000Z");

# The log data has a trailing newline courtesy of the trailing blank line
# added by svnmailer. Therefore, we need to compensate for this in the length
# calculation. We make use of this trailing newline as the field separator.
my $props  = "K 7\nsvn:log\nV " . (length($log) - 1) . "\n$log";
   $props .= "K 10\nsvn:author\nV " . length($author) . "\n$author\n";
   $props .= "K 8\nsvn:date\nV " . length($propdate) . "\n$propdate\n";
   $props .= "PROPS-END\n";

print DUMP "Revision-number: $revision\n";
print DUMP "Prop-content-length: " . length($props) . "\n";
print DUMP "Content-length: " . length($props) . "\n\n";
print DUMP $props;
print DUMP "\n";

foreach my $file (@modified) {
	my $contents = "";

	open MOD,"<$working_copy/../$file" or die "Failed opening $file\n";
	{
		local $/;
		$contents = <MOD>;
	}
	close MOD;

	print DUMP "Node-path: $file\n";
	print DUMP "Node-kind: file\n";
	print DUMP "Node-action: change\n";
	print DUMP "Text-content-length: " . length($contents) . "\n";
	print DUMP "Text-content-md5: " . md5_hex($contents) . "\n";
	print DUMP "Content-length: " . length($contents) . "\n\n";
	print DUMP $contents;
	print DUMP "\n\n";
}

close DUMP;


