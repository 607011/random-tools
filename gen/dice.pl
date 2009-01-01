#!/usr/bin/perl -w

use strict;
use warnings;


my @X;
my @Y;

map { push @X, 1 + int(rand(6)); }(1..17);
map { push @Y, 1 + ($_ + 1) % 6; }@X;

print join(', ', @X), "\n";
print join(', ', @Y), "\n";


1;

