# Dristribute By Median

## Problem description
This is the second project of Parallel and Distributed subjects of Electrical and Computer Engineering in Aristotle University of Thessaloniki. The main concept of this project is the creation of a Message Passing Interface(MPI) application.


The application is to distribute N points of dimension d at k processes by the median of their distance from a pivot point. Moreover, the k processes are loading N/k points of dimension d. Then the leader is going to choose among his points a pivot and broadcast it to all processes. The others calculate the distances and send it back to the leader. The leader finds the median using quickselect and send it back to the processes. Then each process is splitting their points to the ones that have greater distance from median and the ones with smaller. The processes with id smaller than half the total number of processes are going to send the points that have distance greater than median, while the processes with id greater than half the total number of processes the opposite.


This procedure is repeated recursively changing the leader and the number of processes. In the end each one process should have points with minimum distance from pivot greater than the previous process and maximum distance smaller than the next process. The program should verify the correctness of the program on its own.

## Implementation
The application where built in C and as MPI implementation OpenMPI is used. The building tool is CMake. 

There is available documentation in latex and html format in case you have doxygen. To create it you need to run ``doxygen sample_dox.conf`` in root directory and open html/index.htlm or run ``make`` inside the latex folder and open the created pdf.

## Dependencies
For the main application:
1. Open MPI >= 4.1.2
2. CMake >= 3.21

For the data creation:
1. python >= 3.8.10
2. pip >= 20.0.2
3. numpy >= 1.19.4
4. torch >= 1.7.1
5. pandas >= 1.1.5

You can run ``pip install -r requirements.txt`` inside the scripts folder. Alternatively you can try using your versions.

## How to build
1. create and go in a build folder ``mkdir build && cd build``
2. ``cmake ..``
3. ``make``
4. You can run ``mpirun -n k sprogram <path to data>`` where k is the number of processes(should be less than your actual number of cores).
5. Alternatively you can run a parametric test with ses.sh script. You can overwrite in the first loop the default processes 2 and 4 and put any other power of 2 and pass as argument the path to a folder with a lot of data with the .dat specifier. More specifically inside the build folder run ``./ses.sh <path to folder>`` . Note here that every time you cηange the number of processes in the ses.sh script you need to run ``cmake ..`` again inside the build folder.

## Datasets
The application needs some data in a specific format in order to run. The data are N points of d dimension. The points are saved on a binary file in contiguous form. Moreover the first two float32 numbers are the number and the dimension of the points. Then all the coordinates of points are saved in float32 in contiguous form.
You can create data with some scripts included in the scripts folder.
1. To create random points based on a distribution run inside the scripts folder ``python3 create_data.py -n <number of points> -d <dimension of points> -o <distribution id>``, where the id distribution is 1 for uniform, 2 for normal and 3 for exponential.
2. To create points bases on MNIST databases run inside the scripts folder ``python3 datasets.py -n <number of points> -o <mnist dataset id>``, where the mnist dataset id is 1 for digit dataset and 2 for fashion clothes.


Alternatively you can download some data samples used for testing from my drive: (~1GB).
## Testing
The program outputs to the console if the distribution succeeded. In case of success, it saves in the out folder (inside the build) the execution times and the info of the run. This file can be read in python using pandas.

In addition to this there are some GoogleTests to verify the correctness of some basic utilities.
To build them:
1. ``cmake -BUILD_TESTING=ON ..``
2. ``make``
3. ``make test`` or ``ctest -V`` to see the verbose of failing tests.
