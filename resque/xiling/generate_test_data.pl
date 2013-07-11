#!/usr/bin/perl
use Getopt::Long;

sub trim($);

my $inFile;
my $outFile;
my $printHelp = 0;

$result = GetOptions(   "in=s"          	=> \$inFile,    	# input File
                        "out=s"         	=> \$outFile,   	# output File
                        "help!"         	=> \$printHelp  	# print help
                     );

if($result || $printHelp){
    print "--in=InputFile\n--out=OutPutFile\n--help\n";
}

print "read: $inFile\nwrite: $outFile\nprint help: $printHelp\nresult: $result\n";

exit if !$result;

open INF, "<$inFile"  ||  die "can not open $inFile for reading\n";
open OUTF, ">$outFile"  || die "can not open $outFile for writing\n";

$fileLines = `wc -l $inFile`;
$fileLines=~/(\d+)\s*[\w|\_|\s]*/;
$fileLines = $1;
print "File Line: $fileLines\n";

$lineNum = 0;
$preChrName = "";
$lastPercent = 0;
$lastPos = 0;

while(<INF>){
    $lineNum++;
    $sentence = trim($_);
	
    $percent = sprintf "%.1f", (100*($lineNum/$fileLines));
    if($percent != $lastPercent){
        $lastPercent = $percent;
        proc_bar($percent, 100);
    }

    @contents = split(/\t/, $sentence);

    $tile_id   = $contents[0];
    $object_id = $contents[1];
    $polygon   = $contents[2];

    $len = scalar(@contents);

    print "len = $len ";
    print "0 = $tile_id ";
    print "1 = $object_id ";
    print "2 = $polygon ";

    print OUTF "$tile_id\t2\t$object_id\t1\t1\t1\t$polygon\n"; 
    #print OUTF "$pos $score\n" if ($step eq "variable");
}

print "\nHandled $lineNum lines\n";

sub trim($)
{
    my $string = shift;
    $string =~ s/^\s+//;
    $string =~ s/\s+$//;
    return $string;
}

sub proc_bar{
    local $| = 1;
    my $i = $_[0] || return 0;
    my $n = $_[1] || return 0;
    print "\r\033[36m[\033[33m".("|" x int(($i/$n)*50)).
    (" " x (50 - int(($i/$n)*50)))."\033[36m]";
    printf("%2.1f%%\033[0m",$i/$n*100);
    local $| = 0;
}

