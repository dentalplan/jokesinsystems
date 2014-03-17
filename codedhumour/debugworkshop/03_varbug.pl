

$filename = "animals.txt";
open(ANI, $filename) || die("could not open file"); #Opens list of animals 

@animals = <ANI>; #places list into array
$ln = @animals;   #gets


for($i=0; $i < $ln; $i++){

	$letter = substr($animals[$i],0,1);
	${$letter} = $animals[$i];
	print $i;
	print $animals[$i];
}

@alpha = (a .. z);
#alpha = ('a','b','c','d');
$ln = @alpha;

for($i=0; $i < $ln; $i++){
	print $alpha[$i] . " is for ";
	$letter = $alpha[$i];
	print ${$letter} . "\n";

}




