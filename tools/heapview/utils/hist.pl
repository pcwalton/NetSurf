#!/usr/bin/env perl
#
# Example input:
#
# MEM mprintf.c:1094 malloc(32) = e5718
# MEM mprintf.c:1103 realloc(e5718, 64) = e6118
# MEM sendf.c:232 free(f6520)

use warnings;
use strict;

my %sizemap;

my $file = $ARGV[0];

if(! -f $file) {
    print "Usage: hist.pl <dump file>\n";
    exit;
}

open(FILE, "<$file");

while(<FILE>) {
    chomp $_;
    my $line = $_;

    if($line =~ /^MEM ([^ ]*):(\d*) (.*)/) {
        # generic match for the filename+linenumber
        my $source = $1;
        my $linenum = $2;
        my $function = $3;

        if($function =~ /free\(0x([0-9a-f]*)/) {
        }
        elsif($function =~ /malloc\((\d*)\) = 0x([0-9a-f]*)/) {
            my $size = $1;

            $sizemap{$size}++;
        }
        elsif($function =~ /calloc\((\d*),(\d*)\) = 0x([0-9a-f]*)/) {
            my $size = $1*$2;

            $sizemap{$size}++;
        }
        elsif($function =~ /realloc\(0x([0-9a-f]*), (\d*)\) = 0x([0-9a-f]*)/) {
            my $newsize = $2;

            $sizemap{$newsize}++;
        }
        elsif($function =~ /strdup\(0x([0-9a-f]*)\) \((\d*)\) = 0x([0-9a-f]*)/) {
            my $size = $2;

            $sizemap{$size}++;
        }
        elsif($function =~ /strndup\(0x([0-9a-f]*), (\d*)\) \((\d*)\) = 0x([0-9a-f]*)/) {
            # strndup(a5b50, 20) (8) = df7c0

            my $size = $3;

            $sizemap{$size}++;
        }
        else {
            print "Not recognized input line: $function\n";
        }
    }
    else {
    }
}
close(FILE);

foreach my $size (sort { $a <=> $b } keys %sizemap) {
    print "$size,$sizemap{$size}\n";
}
