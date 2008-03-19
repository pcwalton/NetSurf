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

if ($day == 0 || ! -f "currev") {
	# Perform complete backup
	command("svnadmin dump " . REPOS_PATH .
		" -r 0:$cur_revision --deltas" .
		" >netsurf-svn-$days[$day]");
} else {
	# Incremental since yesterday, please
	# Get yesterday's revision
	my $old_revision = 0;
	open REVFILE, "<currev" or die "failed to open currev: $!\n";
	foreach my $line (<REVFILE>) {
		$old_revision = $line;
	}
	close REVFILE;
	chomp $old_revision;

	# Calculate base for this dump
	my $base_revision = $old_revision + 1;

	if ($cur_revision == $old_revision) {
		print LOG "Nothing changed\n";
		# FIXME: what do we actually want to do here?
		command("rm -f netsurf-svn-$days[$day]");
		command("touch netsurf-svn-$days[$day]");
	} else {
		# Perform the dump
		command("svnadmin dump " . REPOS_PATH . 
			" -r $base_revision:$cur_revision " .
			"--incremental --deltas >netsurf-svn-$days[$day]");
	}
}

# Gzip it, in an rsync-friendly manner
command("gzip -5 -c --rsyncable netsurf-svn-$days[$day] " .
	">netsurf-svn-$days[$day].gz");

# Rsync it to batfish
command("rsync --verbose --compress --times netsurf-svn-$days[$day].gz " .
	"netsurf\@netsurf-browser.org:/home/netsurf/svn-backups/");

# Update current revision indicator
open REVFILE, ">currev" or die "failed to open currev: $!\n";
print REVFILE "$cur_revision\n";
close REVFILE;

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

