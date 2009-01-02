#!/usr/bin/perl -w

use strict;
use warnings;


use constant PI   => 3.14159265358979323846;
use constant PID2 => PI/2;


sub ChiSquareProbability($$);
sub ChiSquareCritical($$);

sub _Norm($)
{
    my $z = shift;
    my $q = $z * $z;
    if (abs($z) > 7) {
	return (1 - 1 / $q + 3 / ($q * $q)) * exp(-$q / 2) / (abs($z) * sqrt(PID2));
    }
    else {
	return ChiSquareProbability($q, 1);
    }
}


sub ChiSquareProbability($$)
{
    my $x = shift;
    my $df = shift;
    if ($x > 1000 || $df > 1000)
    {
	my $q = _Norm((pow($x / $df, 1.0 / 3.0) + 2 / (9 * $df) - 1) / sqrt(2 / (9 * $df))) / 2;
	return ($x > $df)? $q : 1 - $q;
    }
    my $p = exp(-0.5 * $x);
    if ( ($df % 2) == 1) {
	$p *= sqrt(2 * $x / PI);
    }
    my $k = $df;
    while ($k >= 2)
    {
	$p = $p * $x / $k;
	$k -= 2;
    }
    my $t = $p;
    my $a = $df;
    while ($t > 1e-15 * $p)
    {
	$a += 2;
	$t = $t * $x / $a;
	$p += $t;
    }
    return 1 - $p;
}
    


sub ChiSquareCritical($$)
{
    my $p = shift;
    my $df = shift;
    my $v = 0.5;
    my $dv = 0.5;
    my $x = 0;
    while ($dv > 1e-10)
    {
	$x = 1 / $v - 1;
	$dv /= 2;
	if (ChiSquareProbability($x, $df) > $p) {
	    $v -= $dv;
	}
	else {
	    $v += $dv;
	}
    }
    return $x;
}


sub ChiSq
{
    my $n0 = shift;
    my $v = shift;
    my $chi = 0;
    foreach my $n (@{$v}) {
	next unless defined($n);
	my $d = $n - $n0;
	$chi += $d * $d / $n0;
    }
    return $chi;
}


my $max = 6;
my $n = shift || 100000;
my $p = $n / $max;
print "p = ", $p, "\n";
my @X;
my @histo;
map { push @X, 1 + int(rand($max)); }(1..$n);
map { ++$histo[$_]; }@X;
foreach my $i (1..6)
{
    $histo[$i] = 0 if !defined($histo[$i]);
    print $i, ":\t", $histo[$i], "\n";
}
my $chisq = ChiSq($p, \@histo);
print "Chi-Quadrat = ", $chisq, "\n";
print "p-Wert = ", ChiSquareProbability($chisq, 5), "\n";

1;

