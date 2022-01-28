#!/bin/bash

for i in 2 4
do 
	for file in $(ls $1 | grep .dat)
	do
		mpirun -n $i ./sprogram $1/$file
	done
done

