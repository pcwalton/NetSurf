#!/usr/bin/perl

use warnings;
use strict;

# Usage: restore-from-backup.pl /path/to/repository
# Expects to be run from the directory containing the backup files
# Also expects the repository to have been created

die "Usage: restore-from-backup.pl /path/to/repository" if (@ARGV == 0);

open MANIFEST,"<manifest" or die "Failed to open manifest: $!\n";

open LOG,">restore.log" or die "Failed to open restore.log: $!\n";

# The manifest contains an ordered list of days on which backups occurred
# The first entry is always a full backup. Subsequent entries are incremental.
foreach my $line (<MANIFEST>) {
	# Extract the day
	my ($day) = ($line =~ /(.*)\t.*/);
	chomp $day;

	# Ensure the backup exists. If not, bail noisily so as to prevent damage
	if (! -f "netsurf-svn-$day.gz") {
		print STDERR "Expected backup netsurf-svn-$day.gz not found\n";
		print LOG "Expected backup netsurf-svn-$day.gz not found\n";
		exit;
	}

	# Load into the repository
	print STDOUT "Loading netsurf-svn-$day.gz\n";
	command("gunzip -c netsurf-svn-$day.gz | svnadmin load $ARGV[0]");
}

close LOG;
close MANIFEST;

print STDOUT "Restore successful\n";

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

