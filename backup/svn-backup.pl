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

# Rsync dump to batfish
command("rsync --verbose --compress --times " .
	"netsurf-svn-$days[$day].gz " .
	"netsurf\@netsurf-browser.org:/home/netsurf/svn-backups/");

# Now update manifest
#
# We can't do this before we've rsynced the backup as, if the rsync fails, the 
# remote copy will be out-of-sync with us. Then, next time we run, if the rsync
# succeeds, there'll be a mismatch between the manifest and .gz archives.
#
# For example:
#
# If the manifest contains entries for sun,mon,tue
# We run on wed, the rsync fails, but our local manifest's been updated
# We run on thu, rsync succeeds
#
# We have a full set of backups, so that's ok.
#
# On the remote machine, however, the manifest now contains entries for 
# sun->thu (as new entries are appended to the end), but it only has .gzs for 
# sun,mon,tue and thu. As we overwrite the previous week's backup archives, if 
# a restore is attempted on the remote host, then it'll attempt to restore 
# sun,mon,tue,wed-a-week-ago,thu. This is clearly broken.
open MANIFEST, ">>manifest" or die "failed to open manifest: $!\n";
print MANIFEST "$days[$day]\t$cur_revision\n";
close MANIFEST;

# Finally, rsync the manifest to batfish
#
# If this fails, then the remote copy will have a full complement of .gz
# but today's entry will be missing from the manifest. Therefore, a restore
# using the remote copy will omit restoring today's backup, thus:
#
# Manifest contains sun,mon,tue
# Today is wed - .gz rsyncs successfully, local manifest is updated, rsync fails
# Restore is attempted using remote copy, restores sun,mon,tue but not wed.
#
# Ideally, the restore script will check for this case (by consulting the 
# datestamps on the .gz files), and inform the user attempting a restore. 
# They'll then have to restore the omitted .gz manually.
command("rsync --verbose --compress --times " .
	"manifest " .
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

