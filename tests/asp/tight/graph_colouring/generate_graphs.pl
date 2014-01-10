#!/usr/bin/perl

$encoding = 
"% Guess colours.\n
chosenColour(N,C) | notChosenColour(N,C) :- node(N), colour(C).\n
\n
% At least one color per node.\n
:- node(X), not colored(X).\n
colored(X) :- chosenColour(X,Fv1).\n
\n
% Only one color per node.\n
:- chosenColour(N,C1), chosenColour(N,C2), C1!=C2.\n
\n
% No two adjacent nodes have the same colour. \n
:- link(X,Y),  X<Y, chosenColour(X,C), chosenColour(Y,C).\n";

$nodes = $ARGV[1];

if( not $ARGV[0] )
{
	$ARGV[0] = 3;
}

if( not $ARGV[1] )
{
	$ARGV[1] = 30;
}

if( not $ARGV[2] )
{
	$ARGV[2] = 5;
}

$colour = $ARGV[0];
$nodes = $ARGV[1];
$links = $nodes / $ARGV[2]; 

$encoding = $encoding . "colour(1..$colour).\n";

$i = 0;
while( $i < $nodes )
{
	$encoding = $encoding . "node($i).\n";	
	$i += 1;
}

$i = 0;
while( $i < $nodes )
{	
	$j = 0;
	while( $j < $links )
	{
		$j += 1;
		my $random_node = int(rand( $nodes ));
		if( $random_node == $i )
		{
			$random_node += 1;
		}		
		$encoding = $encoding . "link($i,$random_node).\n";
	}

	$i += 1;
}

print($encoding);
