use strict;


for(;;){#makes the fun run forever

	my $val1 = int(rand(20));#make a random number variable
	my $val2 = int(rand(20));#make another random number variable
	&DividePrint ($val1, $val2);#pass these variables to a subroutine

}

sub DividePrint {

	my ($val1, $val2) = @_;#retrieves variables
	print $val1/$val2; #divides one variable by another and prints
	print "\n\n";#prints some line breaks
	sleep (1);#wait a moment!

}
