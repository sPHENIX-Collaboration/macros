#!/usr/bin/perl

use strict;
use File::Basename;
use File::Path;
use File::Find;

my $myself = `whoami`;
chomp $myself;
my $outpathroot = sprintf("/sphenix/user/%s/bcoqa",$myself);
if (! -d $outpathroot)
{
    mkpath $outpathroot;
}

#if ($#ARGV < 0)
#{
#    print "need list file as argument\n";
#    exit(1);
#}
my $curdir = `pwd`;
chomp $curdir;
opendir my $mydir, '.' or die "cannot open directory $!";
my @files = grep{ /tpc/ && /\.list/} readdir($mydir);
close $mydir;
foreach my $f (sort @files)
{
    print "tpc list file: $f\n";
}
my $gl1file = sprintf("gl1daq.list");
if (! -f $gl1file)
{
    print "input list file $gl1file does not exist\n";
    exit(1);
}
open(F,"$gl1file");
my $anyfile = <F>;
chomp $anyfile;
my $anylfn = basename($anyfile);
my $runnumber;
if ($anylfn =~ /(\S+)-(\d+)-(\d+).*\..*/)
{
    $runnumber = int($2);
}
if (! defined $runnumber)
{
    print "could not extract run number from $anylfn\n";
    exit(1);
}
close(F);
print "run: $runnumber\n";
my $outpath = sprintf("%s/%08d",$outpathroot,$runnumber);
if (! -d $outpath)
{
    mkpath($outpath);
}
my $condorlogpath = sprintf("%s/condorlog",$outpath);
if (! -d $condorlogpath)
{
    mkpath $condorlogpath;
}
my $outdir = sprintf("%s/data",$outpath);
if (! -d $outdir)
{
    mkpath $outdir;
}
open(F1,">condor.list");
foreach my $file (sort @files)
{
    chomp $file;
    my $lfn = basename($file);
    $lfn =~ s/\.list//;
    print "file is $lfn\n";
    my $ntupfile = sprintf("%s.root",$lfn);
    my $condorout = sprintf("%s/condor-%s.out",$condorlogpath,$lfn);
    my $condorerr = sprintf("%s/condor-%s.err",$condorlogpath,$lfn);
    my $condorlog = sprintf("/tmp/%scondor-%s.log",$myself,$lfn);
    print F1 "$curdir/run_tpcqa.sh $file $ntupfile $outdir $condorout $condorerr $condorlog $curdir\n";
}
# now for the gl1
my $file=$gl1file;
    my $lfn = basename($gl1file);
    $lfn =~ s/\.list//;
    print "gl1file is $lfn\n";
    my $ntupfile = sprintf("%s.root",$lfn);
    my $condorout = sprintf("%s/condor-%s.out",$condorlogpath,$lfn);
    my $condorerr = sprintf("%s/condor-%s.err",$condorlogpath,$lfn);
    my $condorlog = sprintf("/tmp/%scondor-%s.log",$myself,$lfn);
    print F1 "$curdir/run_gl1qa.sh $gl1file $ntupfile $outdir $condorout $condorerr $condorlog $curdir\n";

close(F1);
