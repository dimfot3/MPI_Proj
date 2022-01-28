#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "utils.h"
#include "core_func.h"
#include "distributeByMedian.h"

int main(int argc, char** argv) {
    if(argc<2)
    {
        printf("Error! You need to specify dataset path.\n");
        exit(0);
    }
    srand(time(NULL));
    //MPI basic operations
    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    if(world_rank == 0)
        printf("Name of device: %s, Rank: %d, Total processors: %d \n", processor_name, world_rank, world_size);
    
    char *path = argv[1];
    //load points
    struct data points;
    int verbose = world_rank == 0 ? 1 : 0;
    get_points(path, &points, world_rank, world_size, verbose);  

    //pick and broadcast pivot
    int pivot=-1;
    float *pivot_point = (float*)malloc(sizeof(float)*points.dim);
    if(world_rank == 0)
    {
        pivot = rand() % points.num;
        memcpy(pivot_point, points.points[pivot], sizeof(float)*points.dim);
        printf("Pivot is %d\n", pivot);       
    }
    MPI_Bcast(pivot_point, points.dim, MPI_FLOAT, 0, MPI_COMM_WORLD );

    //distribute by median recursive procedure and then calculate again the distances
    double t1, t2; 
    t1 = MPI_Wtime(); 
    distributeByMedian(0, world_size, &points, pivot_point);
    MPI_Barrier(MPI_COMM_WORLD);
    t2 = MPI_Wtime(); 

    //autotesting
    calculateDistances(pivot_point, &points);
    transmitDistances(&points, world_rank, 0, world_size);
    if(world_rank == 0)
    {
        int succ = evaluate_result(points.total_dist, points.num, world_size);
        if(succ == 0)
            printf("Distribution failed\n");
        else
        {
            printf("Distribution succeeded in %lf seconds\n", t2 - t1);
            save_time(&points, world_size, (float)(t2 - t1));
        } 
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}