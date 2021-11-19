#!/usr/bin/perl

use warnings;
use strict;

die "program a1 a2 i" unless (scalar(@ARGV) == 3);

my ($a1, $a2, $NUM) = @ARGV;

my $i = -1;
my $j = 0;
$a2 = $a2 - $a1 + 1;

if ($NUM >= 10000)
{
	print "$NUM\n";
}
elsif ($NUM >= 1000)
{
	print " $NUM\n";
}
elsif ($NUM >= 100)
{
	print "  $NUM\n";
}
elsif ($NUM >= 10)
{
	print "   $NUM\n";
}
else
{
	print "    $NUM\n";
}

while (++$i < $NUM) {
	$j = int(rand($a2) + $a1);
	if ($j >= 10000)
	{
		print "$j\n";
	}
	elsif ($j >= 1000)
	{
		print " $j\n";
	}
	elsif ($j >= 100)
	{
		print "  $j\n";
	}
	elsif ($j >= 10)
	{
		print "   $j\n";
	}
	else
	{
		print "    $j\n";
	}
}
