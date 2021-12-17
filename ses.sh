#!/bin/bash

for i in 4
do 
	for file in $(ls data/$1)
	do
		mpirun -n $i ./sprogram ../data/$1/$file
	done
done

