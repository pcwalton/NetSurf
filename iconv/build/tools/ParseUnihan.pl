#!/usr/bin/perl

use warnings;
use strict;

# Usage: ParseUnihan.pl <unihan> <charset>
#
# <unihan> is the path of the Unihan database, or - to read from stdin
#
# <charset> is one of the items in MAP

use constant MAP => { 
	"GB2312-80"         => { tag => "kIRG_GSource", prefix => "0" }, 
	"JIS X 0208:1990"   => { tag => "kIRG_JSource", prefix => "0" },
	"JIS X 0212:1990"   => { tag => "kIRG_JSource", prefix => "1" },
	"JIS X 0213:2000-1" => { tag => "kIRG_JSource", prefix => "3" },
	"JIS X 0213:2000-2" => { tag => "kIRG_JSource", prefix => "4" },
	"JIS X 0213:2004"   => { tag => "kIRG_JSource", prefix => "3A" },
	"KS C 5601-1987"    => { tag => "kIRG_KSource", prefix => "0" },
	"CNS 11643-1992-1"  => { tag => "kIRG_TSource", prefix => "1" },
	"CNS 11643-1992-2"  => { tag => "kIRG_TSource", prefix => "2" },
	"CNS 11643-1992-3"  => { tag => "kIRG_TSource", prefix => "3" },
	"CNS 11643-1992-4"  => { tag => "kIRG_TSource", prefix => "4" },
	"CNS 11643-1992-5"  => { tag => "kIRG_TSource", prefix => "5" },
	"CNS 11643-1992-6"  => { tag => "kIRG_TSource", prefix => "6" },
	"CNS 11643-1992-7"  => { tag => "kIRG_TSource", prefix => "7" }
};

usage() if (@ARGV != 2);

my $unihan = shift @ARGV;
my $charset = shift @ARGV;

my $umap = MAP->{$charset};
die "Failed finding map for $charset\n" if (!defined($umap));

my %mbtab = ();

open UNIHAN,"<$unihan" or die "Failed opening $unihan: $!\n";

while (my $line = <UNIHAN>) {
	# Skip comments
	next if ($line =~ /^#/);
	chomp $line;

	# Format: <unicode>\t<tag>\t<data>
	my ($uni, $tag, $data) = split('\t', $line);

	# Ignore if this isn't a tag we're interested in
	next if ($tag ne $umap->{tag});

	# Strip 'U+' from unicode codepoint
	$uni =~ s/^U\+//;

	# Data is a list of space-separated fields
	my @sources = split('\s', $data);

	foreach my $source (@sources) {
		chomp $source;

		# Format: <prefix>-<codepoint>
		my ($prefix, $codepoint) = split('-', $source);

		# Skip prefixes we're not interested in
		next if ($prefix ne $umap->{prefix});

		# Insert into multibyte table
		my $b1 = hex($codepoint) >> 8;
		my $b2 = hex($codepoint) & 0xFF;

		if (!defined($mbtab{$b1})) {
			$mbtab{$b1} = ();
		}

		$mbtab{$b1}{$b2} = hex($uni);
		
		# Drop out early if we've got the mapping for this codepoint
		last;
	}
}

close UNIHAN;

#my $offset = 0;
for (my $i = 1; $i <= 94; $i++) {
	print "Row $i:";

	for (my $j = 1; $j <= 94; $j++) {
#		if ($offset % 16 == 0) {
#			printf("%07x:", $offset);
#		}

		if (defined($mbtab{$i}) && defined($mbtab{$i}{$j})) {
			printf(" %04x", $mbtab{$i}{$j});
		} else {
			print " ffff";
		}

#		$offset += 2;
#
#		if ($offset % 16 == 0) {
#			print "\n";
#		}
	}

	print "\n";
}

sub usage
{
	my $map = MAP;

	print STDERR <<EOF;
Usage: ParseUnihan.pl <unihan> <charset>

<unihan> is the path of the Unihan database, or - to read from stdin

<charset> is one of:
EOF

	foreach my $cset (sort(keys(%$map))) {
		print STDERR "\t$cset\n";
	}

	exit 1;
}

