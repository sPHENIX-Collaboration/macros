#!/usr/bin/env perl

use strict;
use warnings;
use File::Basename;
use DBI;
use Data::Dumper;

if ($#ARGV < 1)
{
    print "usage: CreateFileLists.pl <runnumber> <daqhost>\n";
    exit(0);
}

my $runnumber = $ARGV[0];
my $daqhost = $ARGV[1];

my $dbh;
my $attempts = 0;

CONNECTAGAIN:
if ($attempts > 0)
{
    sleep(int(rand(21) + 10)); # sleep 10-30 seconds before retrying
}
$attempts++;
if ($attempts > 200)
{
    print "giving up connecting to DB after $attempts attempts\n";
    exit(1);
}
$dbh = DBI->connect("dbi:ODBC:RawDataCatalog_read") || goto CONNECTAGAIN;
if ($attempts > 0)
{
    print "connections succeded after $attempts attempts\n";
}
$dbh->{LongReadLen}=2000; # full file paths need to fit in here

my $getfiles = $dbh->prepare("select filename,daqhost from datasets where runnumber = $runnumber and (daqhost = '$daqhost' or daqhost = 'gl1daq') order by filename");

$getfiles->execute();

my %flist = ();
while (my @res = $getfiles->fetchrow_array())
{
    push @{$flist{$res[1]}}, $res[0];
#    print "$res[0] on $res[1]\n";
}
#print Dumper(\%flist);
foreach my $host (keys %flist)
{
    my $listname = sprintf("%s.list",$host);
    open(F,">$listname");
    for my $i ( 0 .. $#{ $flist{$host} } )
    {
	print F "$flist{$host}[$i]\n";
    }
#    print F "@{ $flist{$host} }\n";
    close(F);
}
