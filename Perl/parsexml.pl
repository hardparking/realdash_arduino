#!/usr/bin/env perl

use strict;
use 5.010;

use Data::Dumper;
use XML::Simple;
use utf8;

binmode STDOUT, ":utf8";

my $filename = 'version1_180.xml';

my $xml = new XML::Simple;
my $data = $xml->XMLin($filename);

my %xml_hash = %{$data->{'frameData'}->{'variables'}->{'symbol'}};

print "{ NULL, NULL, 0 },\n"; #ecumasters starts at 1 instead of 0 like C?

foreach my $key ( sort sort_channels (keys(%xml_hash))) {
    next unless $xml_hash{$key}->{'channel'};
    print "{ \"$key\", \"$xml_hash{$key}->{'unit'}\", $xml_hash{$key}->{'divider'} },\n";
}

sub sort_channels {
    $xml_hash{$a}->{'channel'} <=> $xml_hash{$b}->{'channel'};
}
