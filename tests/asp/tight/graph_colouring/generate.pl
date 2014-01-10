#!/usr/bin/perl

$i = 0;
$cols = 3;
$nodes = 30;
$ratio = 5;

while($i < 10)
{	
	system("./generate_graphs.pl $cols $nodes $ratio > graph.colouring.$cols.$nodes.$ratio\_$i.asp");
	$i += 1;
}

$i = 0;
$cols = 4;
$nodes = 30;
$ratio = 5;

while($i < 10)
{	
	system("./generate_graphs.pl $cols $nodes $ratio > graph.colouring.$cols.$nodes.$ratio\_$i.asp");
	$i += 1;
}

$i = 0;
$cols = 3;
$nodes = 40;
$ratio = 5;

while($i < 10)
{	
	system("./generate_graphs.pl $cols $nodes $ratio > graph.colouring.$cols.$nodes.$ratio\_$i.asp");
	$i += 1;
}

$i = 0;
$cols = 4;
$nodes = 40;
$ratio = 5;

while($i < 10)
{	
	system("./generate_graphs.pl $cols $nodes $ratio > graph.colouring.$cols.$nodes.$ratio\_$i.asp");
	$i += 1;
}
