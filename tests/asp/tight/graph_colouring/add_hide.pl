#!/usr/bin/perl

@files = <*.asp>;
foreach $file (@files) 
{
    print("Reading file $file\n");
    $var = "\\#hide. \\#show col/2.";
    system("echo $var >> $file");   
}
