#!/usr/bin/perl

@files = <*.asp>;
foreach $file (@files) 
{
    print("Reading file $file\n");

    $file_test = $file . ".test.py";
    $file_gringo = $file . ".gringo";
    $result = $file . ".result";

    system("echo input = \\\"\\\"\\\" >> ./tests/$file_test");
    system("cat ./ground/$file_gringo >> ./tests/$file_test");
    system("echo \\\"\\\"\\\" >> ./tests/$file_test");

    system("echo output = \\\"\\\"\\\" >> ./tests/$file_test");
    system("cat ./results/$result >> ./tests/$file_test");
    system("echo \\\"\\\"\\\" >> ./tests/$file_test");
}
