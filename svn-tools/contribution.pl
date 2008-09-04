#!/usr/bin/perl

use strict;
use warnings;
use XML::Parser;

# Usage: contribution.pl <repository_url> <username>

die "Usage: contribution.pl <repository_url> <username>" if (scalar(@ARGV) != 2);

my $repos = $ARGV[0];
my $user = $ARGV[1];

# Get SVN log for repository
my @log = command("svn log --xml $repos");
# And convert it to one long string
my $log = "";
foreach my $line (@log) {
	$log .= $line;
}

# Parser states
use constant INITIAL => 0;
use constant LOG     => 1;
use constant ENTRY   => 2;
use constant AUTHOR  => 3;
my $state = INITIAL;

# Parser context, and output array
my @revisions;
my $cur_revision;
my $cur_author = "";

# Create parser and parse log
my $parser = new XML::Parser(ErrorContext => 2);
$parser->setHandlers(Start => \&start_handler,
		     End => \&end_handler,
		     Char => \&char_handler);
$parser->parse($log);

# Dump revision diffs to file
mkdir($user);
chdir($user);
foreach my $rev (@revisions) {
	my @diff = command("svn diff -c$rev $repos");

	open FH, ">$rev" or die "Failed opening $rev $!\n";

	foreach my $line (@diff) {
		print FH "$line";
	}

	close FH;

	print "$rev\n";
}

################################################################################

sub start_handler
{
	my $p = shift; 
	my $el = shift;

	if ($state == INITIAL && $el eq "log") {
		$state = LOG;
	} elsif ($state == LOG && $el eq "logentry") {
		while (@_) {
			my $attr = shift;
			my $val = shift;

			if ($attr eq "revision") {
				$cur_revision = $val;
			}
		}
		$state = ENTRY;
	} elsif ($state == ENTRY && $el eq "author") {
		$state = AUTHOR;
	}
}

sub end_handler
{
	my $p = shift;
	my $el = shift;

	if ($state == LOG && $el eq "log") {
		$state = INITIAL;
	} elsif ($state == ENTRY && $el eq "logentry") {
		$state = LOG;
	} elsif ($state == AUTHOR && $el eq "author") {
		if ($cur_author eq $user) {
			push(@revisions, $cur_revision);
		}
		$cur_author = "";
		$state = ENTRY;
	}
}

sub char_handler
{
	my $p = shift;
	my $data = shift;

	if ($state == AUTHOR) {
		$cur_author .= $data;
	}
}

sub command
{
	my $cmd = shift;

	my @output = `$cmd`;

	my $status = $? / 256;

	die "Failed running '$cmd' ($status)" if $status;

	return @output;
}

