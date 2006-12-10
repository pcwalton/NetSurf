#!/usr/bin/perl
#
# Scan the current directory for packages and output an index.
#

use warnings;
use strict;

die "Usage: $0 base-url\n" if @ARGV != 1;
my $url = shift;

foreach my $zip (glob '*.zip') {
	my $control = `unzip -p '$zip' RiscPkg/Control`;
	chomp $control;
	my $size = -s $zip;
	my $md5 = `md5sum $zip`;
	$md5 = (split / /, $md5)[0];
	print <<END;
$control
Size: $size
MD5Sum: $md5
URL: $url$zip

END
}

