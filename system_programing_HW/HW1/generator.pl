#!/usr/bin/perl

use warnings;
use strict;

die "program a1 a2 i" unless (scalar(@ARGV) == 3);

my ($a1, $a2, $NUM) = @ARGV;

print "# define NUMS $NUM\n";
print "int input[NUMS] = \n{";

my $i = 0;
my $j = 0;
while (++$i < $NUM) {
	$j = int(rand($a2 - $a1) + $a1);
	print "$j ,\n";
}

$j = int(rand($a2 - $a1) + $a1);
print "$j\n};\n"
