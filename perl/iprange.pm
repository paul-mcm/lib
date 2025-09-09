#!/usr/bin/perl -w
############################
# Returns range of Host IPs from CIDR addr
# Input: 
# %ips = ( cidr => ip/cidr,
# 	  low => '',
#	  high => '',
# );
# OUTPUT returns %hash with
# values for low/high 
############################
package iprange;
require Exporter;
use strict;
use warnings;
our (@ISA, @EXPORT, @EXPORT_OK);
@ISA = qw(Exporter);
@EXPORT = ();
@EXPORT_OK = qw(iprange);
my $MAX_OCT = 4;

sub iprange {
	my $ip; 
	my $nmask;
	my @octets;
	my $bocti;
	my $hbits;
	my $i;
	my $maxr = 1;
	my $ips;

	if ($#_ > 0) {
	    print "Error: Too many args to function\n";
	    exit;
	}

	$ips = shift @_; 
	if (ref($ips) =~ 'HASH') {
	    $ips->{high} = undef;
	    $ips->{low} = undef;
	} else {
	    print "Error: function takes hash ref\n";
	    exit -1;
	}

	# Parse input
	($ip = $ips->{cidr}) =~ s/\/.*$//;

	($nmask = $ips->{cidr}) =~ s/^.*\///;
	if ($nmask =~ /\D{1,2}/ || length($nmask) == 0
		|| $nmask < 1 || $nmask > 32) { 
	    print "Error: bad netmask\n";
	    return $ips;
	}

	@octets = split /\./, $ip;
	if (@octets != $MAX_OCT) {
	    print "Error: too many octets\n";
	    return $ips;
	}

	for ($i = 0; $i < $MAX_OCT; $i++) {
	    if ($octets[$i] < 0 || $octets[$i] > 255) {
		print "Error: Octet out of range\n";
		return $ips;
	    }
	}

	$bocti = int(($nmask / 8)); # $bocti is octet w/ the host bits
	$hbits = 8 - ($nmask % 8);  # number of host bits

	# Calc/set lowest value of host range
	$octets[$bocti] >>= $hbits; # shift off the host bits
	$octets[$bocti] <<= $hbits; 
	set_octs(\@octets, ($bocti + 1), 0);
	format_addr(\@octets, \$ips->{low});
	
	# Calc/Set high end
	($maxr <<= ((32 - $nmask) % 8)) == 1 ? $maxr = 255 : $maxr--;
	$octets[$bocti] += $maxr;
	set_octs(\@octets, ($bocti + 1), 255);
	format_addr(\@octets, \$ips->{high});

	return $ips;
}

sub format_addr {
    (my $o, my $ip) = @_;

    $$ip = $$o[0];
    for (my $i = 1; $i < $MAX_OCT; $i++) {
	$$ip = $$ip . ".";
	$$ip = $$ip . $$o[$i];
    }
}

sub set_octs {
    (my $o, my $i, my $v) = @_;
    for (; $i < $MAX_OCT; $i++) {
	$$o[$i] = $v;
    }
}
1;

