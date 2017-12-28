#!/usr/bin/env perl

use strict;
use 5.010;

use Data::Dumper;
use XML::Simple;
use utf8;

binmode STDOUT, ":utf8";

my $filename = 'version1_200.xml';

my $xml = new XML::Simple;
my $data = $xml->XMLin($filename);

my %xml_hash = %{$data->{'frameData'}->{'variables'}->{'symbol'}};

my $counter = 1;
print "\t{ NULL, NULL, 0, 0, 0},\n"; #ecumasters starts at 1 instead of 0 like C?

foreach my $key ( sort sort_channels (keys(%xml_hash))) {
    next unless $xml_hash{$key}->{'channel'};
    next if ($key eq "cel");
    print "\t{ \"$key\", \"$xml_hash{$key}->{'unit'}\", $xml_hash{$key}->{'divider'}, $xml_hash{$key}->{'gaugeMin'}, $xml_hash{$key}->{'gaugeMax'} },\n";
    $counter++;
}

while ($counter <= 254) {
    print "\t{ NULL, NULL, 0, 0, 0 },\n";
    $counter++;
}

print "\t{\"cel\", \"\", 1, 0, 0}\n";

sub sort_channels {
    $xml_hash{$a}->{'channel'} <=> $xml_hash{$b}->{'channel'};
}
