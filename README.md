# Dristribute By Median

## Problem description
This is the second project of Parallel and Distributed subject of Electrical and Computer Engineering in Aristotle University of Thessaloniki. The main concept of this project is the creation of a Message Passing Interface(MPI) application. 


The application is to distribute N points of dimension d at k processes by the median of their distance from a pivot point. Moreover, the k processes are loading N/k points of dimension d. Then the leader is going to chose among his points a pivot and broadcast it to all processes. The others calculate the distances and send it back to the leader. The leader find the median using quickselect and send it back to the processes. Then each process is splitting their points to the ones that have greater diastance from median and the ones with smaller. The processes with id smaller than half the total number of processes are going to send the points that have distance greater than median, while the processes with id greater than half the total number of processes the opposite. 


This procedure is repeated recursivly changing the leader and the number of processes. In the end each one process should have points with minimum distance from pivot grater than the previous process and maximum distance smaller than the next process. The program should verify the correctness of the program by its own.

## Implementation
The application where build in C and as MPI implementation OpenMPI is used. The building tool is CMake.

## Depedencies
For the main application:
1. Open MPI >= 4.1.2
2. CMake >= 3.21

For the data creation:
1. python >= 3.8.10
2. pip >= 20.0.2
3. numpy >= 1.19.4
4. torch >= 1.7.1
5. pandas >= 1.1.5

You can run ``pip install -r requirements.txt`` inside the scripts folder. Alertenativly you can try using your versions.

## How to build
1. create a build folder ``mkdir build && cd build``
2. ``cmake ..``
3. ``make``
4. You can run ``mpirun -n k sprogram <path to data>`` where k is the number of processes(should be less than your actual number of cores). 

## Testing
The program outputs to the console if the distribution succeded. In case of success save in the out folder (inside the build) the execution times and the info on the run. This file can be read in python using pandas.

In addition to this there are some GoogleTests to verify the correctness of some basic utilities.
To build them:
1. ``cmake -BUILD_TESTING=ON``
2. ``make``
3. ``make test`` or ``ctest -V`` to see the verbose of failing tests.
