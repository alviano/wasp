#!/usr/bin/perl

@files = <*.asp>;
foreach $file (@files) 
{
    print("Reading file $file\n");
    $file_gringo = $file . ".gringo";
    system("gringo $file --shift > ./ground/$file_gringo");
    
    print("Executing wasp\n");
    $result = $file . ".result";
    system("cat ./ground/$file_gringo | wasp_mg_benchmarks --gringo --silent > ./results/$result");
    print("End");
}
