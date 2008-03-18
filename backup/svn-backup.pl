#!/usr/bin/perl

use warnings;
use strict;

use constant REPOS_PATH => "/home/netsurf/svn";

open LOG, ">backup.log" or die "failed to open backup.log: $!\n";

# Get old revision
my $old_revision = 0;
foreach my $f (glob 'netsurf-svn-r*') {
	($old_revision) = ($f =~ /^netsurf-svn-r(.*)\.gz/);
}

# Calculate base for this dump
my $base_revision = $old_revision + 1;

# Revision of current head
my $cur_revision = (command("svnlook youngest " . REPOS_PATH))[0];
chomp $cur_revision;

# Don't do anything if there haven't been any commits since last time
if ($cur_revision == $old_revision) {
	print LOG "Nothing changed\n";
	exit;
}

# Perform the dump
command("svnadmin dump " . REPOS_PATH . " -r $base_revision:$cur_revision " .
	"--incremental --deltas >>netsurf-svn");

# Gzip it, in an rsync-friendly manner
command("gzip -5 -c --rsyncable netsurf-svn >netsurf-svn-r$cur_revision.gz");

# Rsync it to batfish
command("rsync --verbose --compress --times netsurf-svn-r$cur_revision.gz " .
	"netsurf\@netsurf-browser.org:/home/netsurf/");

# And remove the previous backup
if ($old_revision > 0) {
	command("rm -f netsurf-svn-r$old_revision.gz");
}

sub command {
	my $cmd = shift;
	print LOG "> $cmd\n";
	my @output = `$cmd`;
	foreach my $line (@output) {
		print LOG "| $line";
	}
	my $status = $? / 256;
	print LOG "exit status $status\n" if $status;
	print LOG "\n";
	die "$cmd:\nexit status $status\n" if $status;
	return @output;
}

