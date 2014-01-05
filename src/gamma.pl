#!/usr/bin/env perl
#
#   gamma.pl creates a C header file with a lookup table to be used in
#   uC firmware. pass the tablesize as number of elements and the pwm
#   resolution as number of steps

use strict;
use warnings;

use Carp;
use English;

if ( $#ARGV + 1 != 3 ) {
    croak "Usage: $0 GAMMA TABLESIZE PWMRES\n";
}

my $gamma = shift @ARGV;
my $tablesize = shift @ARGV;
my $pwmres = shift @ARGV;

my @out;
my $x = ( ( $tablesize - 1 ) ** $gamma ) / ( $pwmres - 1 );

print "/*  save this to gamma.h\n";
print " *  gamma was $gamma\n";
print " *  tablesize was $tablesize\n";
print " *  pwmres was $pwmres\n";
print " *  x was $x */\n\n";

print "#ifndef _GAMMA_H_\n";
print "#define _GAMMA_H_\n\n";

print "#include <stdint.h>\n";
print "#include <avr/pgmspace.h>\n\n";

if ( $pwmres > 256 ) {
    print "const uint16_t gammatbl[$tablesize] PROGMEM = {";
} else {
    print "const uint8_t gammatbl[$tablesize] PROGMEM = {";
}

for ( my $lpc = 0; $lpc < $tablesize; $lpc++ ) {
    if ( !($lpc % 10) ) {
        print "\n    ";
    }
    $out[$lpc] = int ( $lpc ** $gamma / $x );
    if ( $lpc < $tablesize - 1 ) {
        print "$out[$lpc], ";
    } else {
        print "$out[$lpc]";
    }
}

print "\n};\n\n";

print "#endif /* _GAMMA_H_ */\n"
