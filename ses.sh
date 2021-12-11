#!/bin/bash

for i in {2..4..2}
do 
	for file in $(ls data)
	do
		mpirun -n $i ./sprogram ../data/$file
	done
done

