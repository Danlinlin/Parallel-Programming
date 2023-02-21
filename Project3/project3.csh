#!/bin/csh
#number of threads:
foreach t ( 1 2 4 8 )
	foreach k ( 5 50 100 200 400 800 1600 3200)
		g++ -DNUMT=$t -DNUMNODES=$k project2.cpp -o prog -lm -fopenmp
		./prog
	end
end


