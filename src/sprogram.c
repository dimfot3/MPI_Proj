/**
 * Author: Fotiou Dimitrios
 * AEM: 9650
 * Here is the main function and the distribute by median function
 **/


#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "core_func.h"

void distributeByMedian(int leader_id, int num_of_proc, struct data* points, float* pivot_point)
{
    //termination term for recursive calls
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
    //printf("Median is %f\n", median);

    //lower = 1 if process id < num of processes/2 else 0
    int lower = world_rank - leader_id < (int)(num_of_proc / 2) ? 1 : 0;
    splitByMedian(median, points, lower);

    //create and broadcast transfer table
    int* table = (int*) malloc(sizeof(int)*num_of_proc);
    table[world_rank-leader_id] = points->num_to_send;
    groupedBcast_table(table, leader_id, world_rank, num_of_proc);
    points->num_sent = 0;
    //transfer elements
    exchangePoints(points, table, world_rank, leader_id, num_of_proc);
    
    //this block is used when median is included in distances one or more times
    if(sum(table, num_of_proc) != 0)
    {   
        //balander table keeps track which processes have not sent their elements in first round
        int* balander_table = (int*) malloc(sizeof(int)*num_of_proc);
        for(int i = 0; i < num_of_proc; i++)
        {
            if(table[i] == 0)
                balander_table[i] = 1;
            else
                balander_table[i] = 0;
        }
        table[world_rank-leader_id] = points->num_to_send != 0 ? points->num_to_send : points->num_of_median;
        groupedBcast_table(table, leader_id, world_rank, num_of_proc);
        exchangeMedians(points, table, world_rank, leader_id, num_of_proc, balander_table);
        free(balander_table);
    }
    
    //put points back in original array
    for(int i = 0; i < points->num_sent; i++)
    {
        memcpy(points->points[points->idx_to_send[i]], points->points_recieved+i*points->dim, sizeof(float)*points->dim);
    }

    //free unecessary resources
    free(table);
    free(points->points_to_sent);
    free(points->points_recieved);
    free(points->idx_to_send);
    free(points->dist);
    //recursirve call

    if(lower)
        distributeByMedian(leader_id, num_of_proc/2, points, pivot_point);
    else
        distributeByMedian(num_of_proc/2+leader_id, num_of_proc/2, points, pivot_point);
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
    int dt_opt = 0;
    char *path = argv[1];
    //load points
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