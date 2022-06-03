#!/usr/bin/perl

use warnings;
use strict;

die "program NUM" unless (scalar(@ARGV) == 1);

my ($NUM) = @ARGV;

my $i = 0;
my $j = 0;
my $k = 0;
while (++$i < $NUM) {
	$j = int(rand(127) + 1);
	$k = int(rand(127) + 1);
	print "$j $k\n";
}
