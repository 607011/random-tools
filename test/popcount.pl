#!/usr/bin/perl -w

use strict;
use warnings;

print "static const size_t PopCount[65536] = {\n  ";
for (my $i = 0; $i < 65536; ++$i)
{
	my $popCount = 0;
	for (my $x = $i; $x != 0; $x = $x & ($x - 1)) 
	{
		++$popCount;
	}
	printf "%2d", $popCount;
	print "," if $i < 65535;
	print "\n  " if ($i < 65535) && (($i % 16) == 15);
}
print "\n};\n";

0;
