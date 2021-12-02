#include <mpi.h>
#include <stdio.h>
#include "utils.h"
#include <time.h>



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
        printf("Name of device: %s, Rank: %d, Total processors: %d \n", processor_name, world_rank, world_size);
    
    //load data for this id
    struct data points;
    char *name = "./data/file";
    get_file(name, world_rank, &points);
    int pivot=-1;
    float *pivot_point = (float*)malloc(sizeof(float)*points.dim);

    //root select pivot
    if(world_rank == 0)
    {
        points.total_dist = (float*) malloc(sizeof(float)*world_size*points.num);
        pivot = rand() % points.num;
        pivot_point = points.points[pivot];        
    }
    else
    {
        points.dist = (float*) malloc(sizeof(float)*points.num);
    }
    //send and recieve pivot point
    MPI_Bcast(pivot_point, points.dim, MPI_FLOAT, 0, MPI_COMM_WORLD );

    //create array with distances
    if(world_rank == 0)
    {
        points_to_dist(pivot_point, &points, points.total_dist);
    }
    else
    {
        points_to_dist(pivot_point, &points, points.dist);
    }

    //collection/send of distances
    if(world_rank == 0)
    {
        MPI_Request *req = malloc(sizeof(MPI_Request)*(world_size-1));
        for(int i = 1; i < world_size; i++)
        {
            MPI_Irecv(points.total_dist + i*points.num, points.num, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &req[i-1]);
        }
        for(int i = 1; i < world_size; i++)
        {
            MPI_Status myStatus;
            MPI_Wait(&req[i-1], &myStatus);
        }
        printf("Root recieved all distances\n");
    }
    else
    {
        MPI_Send(points.dist, points.num, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
        printf("Process %d sent his distances\n", world_rank);
    }

    float median;
    //find and send median distance
    if(world_rank == 0)
    {
        median = quickselect(points.total_dist, 0, points.num*world_size-1, points.num*world_size/2);  
    }
    MPI_Bcast(&median, 1, MPI_FLOAT, 0, MPI_COMM_WORLD );
    printf("Process %d: Median distance is %f\n", world_rank, median);

    //rerder distances
    MPI_Finalize();
}
