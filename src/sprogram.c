#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "core_func.h"

void distributeByMedian(int leader_id, int num_of_proc, struct data* points, float* pivot_point)
{
    if(num_of_proc < 2)
        return;
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    calculateDistances(pivot_point, points);

    transmitDistances(points, world_rank, leader_id, num_of_proc);

    //find and transmit median to all processes
    float median;
    if(world_rank == leader_id)
        median = quickselect(points->total_dist, 0, points->num*num_of_proc-1, points->num*num_of_proc/2);
    groupedBcast_median(&median, leader_id, world_rank, num_of_proc);

    int lower = world_rank - leader_id < (int)(num_of_proc / 2) ? 1 : 0;
    splitByMedian(median, points, lower);

    //create and broadcast transfer table
    int* table = (int*) malloc(sizeof(int)*num_of_proc);
    table[world_rank-leader_id] = points->num_to_send;
    groupedBcast_table(table, leader_id, world_rank, num_of_proc);

    //transfer elements
    int counter = exchangePoints(points, table, world_rank, leader_id, num_of_proc);
    
    //put points back in original array and recalculate distance
    for(int i = 0; i < counter; i++)
        memcpy(points->points[points->idx_to_send[i]], points->points_to_sent+i*points->dim, sizeof(float)*points->dim);

    //free unecessary resources and restore data state
    free(table);
    free(points->points_to_sent);
    free(points->idx_to_send);
    if(lower)
        distributeByMedian(leader_id, num_of_proc/2, points, pivot_point);
    else
        distributeByMedian(num_of_proc/2, num_of_proc/2, points, pivot_point);
}

int main(int argc, char** argv) {
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
    {
        printf("Name of device: %s, Rank: %d, Total processors: %d \n", processor_name, world_rank, world_size);
    }

    //load points
    char path[] = "./data/file.txt";
    struct data points;
    int verbose = world_rank==0 ? 1 : 0;
    get_points(path, &points, world_rank, world_size, verbose);  

    //pick and broadcast pivot
    int pivot=-1;
    float *pivot_point = (float*)malloc(sizeof(float)*points.dim);
    if(world_rank == 0)
    {
        pivot = rand()%points.num;
        memcpy(pivot_point, points.points[pivot], sizeof(float)*points.dim);
        printf("Pivot is %d\n", pivot);       
    }
    MPI_Bcast(pivot_point, points.dim, MPI_FLOAT, 0, MPI_COMM_WORLD );

    double t1, t2; 
    t1 = MPI_Wtime(); 

    //distribute by median procedure
    distributeByMedian(0, world_size, &points, pivot_point);
    
    calculateDistances(pivot_point, &points);
    MPI_Barrier(MPI_COMM_WORLD);
    t2 = MPI_Wtime(); 

    //autotesting
    transmitDistances(&points, world_rank, 0, world_size);
    if(world_rank == 0)
    {
        int succ = evaluate_result(points.total_dist, points.num, world_size);
        if(succ == 0)
            printf("Distribution failed\n");
        else
            printf("Distribution succeeded in %lf seconds\n", t2 - t1);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return;
}