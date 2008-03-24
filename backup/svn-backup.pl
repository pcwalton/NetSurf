#!/usr/bin/perl

use warnings;
use strict;

use constant REPOS_PATH => "/home/netsurf/svn";

open LOG, ">backup.log" or die "failed to open backup.log: $!\n";

my @days = ( "sun", "mon", "tue", "wed", "thu", "fri", "sat" );

# Day of the week [0,6] 0 == Sunday
my $day = (localtime(time))[6];

# Revision of current head
my $cur_revision = (command("svnlook youngest " . REPOS_PATH))[0];
chomp $cur_revision;

if ($day == 0 || ! -f "manifest") {
	# Perform complete backup
	command("svnadmin dump " . REPOS_PATH .
		" -r 0:$cur_revision --deltas --quiet" .
		" | gzip -5 -c --rsyncable >netsurf-svn-$days[$day].gz");

	# Remove the old manifest, if it exists
	# This ensures that the first entry in the manifest is a full backup
	command("rm --force --verbose manifest");
} else {
	# Incremental since yesterday, please
	# Get yesterday's revision
	my $old_revision = 0;
	open MANIFEST, "<manifest" or die "failed to open manifest: $!\n";
	foreach my $line (<MANIFEST>) {
		($old_revision) = ($line =~ /.*\t(.*)/);
	}
	close MANIFEST;
	chomp $old_revision;

	# Calculate base for this dump
	my $base_revision = $old_revision + 1;

	if ($cur_revision == $old_revision) {
		print LOG "Nothing changed\n";
		# Exit here as there's nothing further to do.
		exit;
	} else {
		# Perform the dump
		command("svnadmin dump " . REPOS_PATH . 
			" -r $base_revision:$cur_revision " .
			"--incremental --deltas --quiet " .
			"| gzip -5 -c --rsyncable >netsurf-svn-$days[$day].gz");
	}
}

# Update manifest
open MANIFEST, ">>manifest" or die "failed to open manifest: $!\n";
print MANIFEST "$days[$day]\t$cur_revision\n";
close MANIFEST;

# Rsync dump and manifest to batfish
command("rsync --verbose --compress --times " .
	"manifest netsurf-svn-$days[$day].gz " .
	"netsurf\@netsurf-browser.org:/home/netsurf/svn-backups/");

close LOG;

sub command {
	my $cmd = shift;
	print LOG "> $cmd\n";
	my @output = `$cmd 2>&1`;
	foreach my $line (@output) {
		print LOG "| $line";
	}
	my $status = $? / 256;
	print LOG "exit status $status\n" if $status;
	print LOG "\n";
	die "$cmd:\nexit status $status\n" if $status;
	return @output;
}

