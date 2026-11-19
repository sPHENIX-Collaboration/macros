#!/usr/bin/env perl

use strict;
use warnings;
use File::Basename;
use DBI;
use Data::Dumper;

if ($#ARGV < 2)
{
    print "\nCreateFileLists.pl will produce a list file which can be read into\n";
    print "the Fun4All_New_Year2_Fitting.C macro\n";
    print "usage: CreateFileLists.pl <runnumber> <segment> <tag>\n";
    print "tag is e.g. new_nocdbtag_v003\n";
    exit(0);
}

my $runnumber = $ARGV[0];
my $segment = $ARGV[1];
my $dsttag = $ARGV[2];

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
$dbh = DBI->connect("dbi:ODBC:FileCatalog_read") || goto CONNECTAGAIN;
#if ($attempts > 0)
#{
#    print "connections succeded at $attempts attempt\n";
#}
$dbh->{LongReadLen}=2000; # full file paths need to fit in here

my $getfiles = $dbh->prepare("select filename from datasets where runnumber = $runnumber and segment = $segment order by filename");

$getfiles->execute();
unlink "files.list";
my $inlist = sprintf("files.list");
open(F,">$inlist");
my $nfiles = 0;
my %tags = ();
if ($getfiles->rows == 0)
{
    print "no files for run $runnumber and segment $segment\n";
    exit(1);
}
while (my @res = $getfiles->fetchrow_array())
{
    my @sp1 = split(/-/,$res[0]);
    my @sp2 = split(/_/,$sp1[0]);
    my $foundtag = sprintf("%s_%s_%s",$sp2[$#sp2-2],$sp2[$#sp2-1],$sp2[$#sp2]);
    $tags{$foundtag} = 1;
    if ($res[0] =~ /DST_TRIGGERED_EVENT/ && $res[0] =~ /$dsttag/)
    {
	print F "$res[0]\n";
	$nfiles++;
    }
}
close(F);
if ($nfiles == 0)
{
    print "\nno file found for $dsttag\n";
    print "here is a list of existing ones:\n";
    foreach my $tag (sort keys %tags)
    {
	print "$tag\n";
    }
}
$getfiles->finish();
$dbh->disconnect;
