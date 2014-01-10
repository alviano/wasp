#!/usr/bin/perl

@files = <*.asp>;
foreach $file (@files) 
{
    print("Reading file $file\n");
    $var = "\\#hide. \\#show match/3.";
    system("echo $var >> $file");   
}
