#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>
#include "utils.h"

void groupedBcast_median(float *median, int leader_id, int world_rank, int num_of_proc)
{
    if(world_rank == leader_id)
    {
        for(int i = 1; i < num_of_proc; i++)
        {
            MPI_Request req;
            MPI_Isend(median, 1, MPI_FLOAT, i+leader_id, 0, MPI_COMM_WORLD, &req);
        }
    }
    else
    {
        MPI_Request req;
        MPI_Recv(median, 1, MPI_FLOAT, leader_id, 0, MPI_COMM_WORLD, NULL);
    }
}

void splitByMedian(float median, struct data *points, int lower)
{
    points->points_to_sent = (float*) malloc(sizeof(float*)*points->num*points->dim);
    points->points_recieved = (float*) malloc(sizeof(float*)*points->num*points->dim);
    points->idx_to_send = (int*) malloc(sizeof(int)*points->num);
    int *temp_median_idx = (int*) malloc(sizeof(int)*points->num);
    float *temp_median_points = (float*) malloc(sizeof(float*)*points->num*points->dim);
    int counter = 0;
    int median_counter = 0;
    for(int i = 0; i < points->num; i++)
    {
        if(((points->dist[i] > median) && (lower == 1)) || ((points->dist[i] < median) && (lower == 0)))
        {
            memcpy(points->points_to_sent+counter*points->dim, points->points[i], sizeof(float)*points->dim);
            points->idx_to_send[counter] = i;
            counter++;
        }
        //this is used to handle median elemtns as balander
        if(points->dist[i] == median)
        {
            memcpy(temp_median_points+median_counter*points->dim, points->points[i], sizeof(float)*points->dim);
            temp_median_idx[median_counter] = i;
            median_counter++;
        }

    }
    points->num_to_send = counter;
    points->num_of_median = median_counter;
    //this is used to handle median elemtns as balander
    for(int i = 0 ; i < median_counter; i++)
    {
        memcpy(points->points_to_sent+counter*points->dim, temp_median_points, sizeof(float)*points->dim*median_counter);
        memcpy(points->idx_to_send+counter, temp_median_idx, sizeof(int)*median_counter);
    }
    free(temp_median_idx);
    free(temp_median_points);
}

void transmitDistances(struct data *points, int world_rank, int leader_id, int num_of_proc)
{
    if(world_rank == leader_id)
    {
        points->total_dist = (float*) malloc(sizeof(float) * num_of_proc * points->num);
        for(int i = 0; i < points->num; i++)
        {
            points->total_dist[i] = points->dist[i];
        }

        MPI_Request *req = malloc(sizeof(MPI_Request)*(num_of_proc-1));
        for(int i = 1; i < num_of_proc; i++)
        {
            MPI_Irecv(points->total_dist + i * points->num, points->num, MPI_FLOAT, i + leader_id, 0, MPI_COMM_WORLD, &req[i-1]);
        }
        for(int i = 1; i < num_of_proc; i++)
        {
            MPI_Status myStatus;
            MPI_Wait(&req[i-1], &myStatus);
        }
        free(req);
    }
    else
    {
        MPI_Send(points->dist, points->num, MPI_FLOAT, leader_id, 0, MPI_COMM_WORLD);
    }
    
}

int exchangePoints(struct data* points, int *table, int world_rank, int leader_id, int num_of_proc)
{
    int step = 0;
    int co_idx = world_rank-leader_id < num_of_proc/2 ? num_of_proc/2 + world_rank: world_rank - num_of_proc/2;
    while(1)
    {
        int num_to_change = min(table[world_rank-leader_id], table[co_idx-leader_id]);
        if(num_to_change!=0){
            //printf("Process %d: num to send %d in step %d\n", world_rank, num_to_change, step);
            MPI_Request req;
            MPI_Isend(points->points_to_sent+points->num_sent*points->dim, num_to_change*points->dim, MPI_FLOAT, co_idx, 0, MPI_COMM_WORLD, &req);
            //printf("Process %d: what sent %f\n", world_rank, points->points_to_sent[0]);
            MPI_Recv(points->points_recieved+points->num_sent*points->dim, num_to_change*points->dim, MPI_FLOAT, co_idx, 0, MPI_COMM_WORLD, NULL);
            //printf("Process %d: what recieved %f\n", world_rank, points->points_to_sent[0][0]);
            MPI_Status myStatus;
            MPI_Wait(&req, &myStatus);
        }
        updateTable(table, num_of_proc, step);
        step++;
        points->num_sent += num_to_change;
        points->num_to_send -= num_to_change;

        if(sum(table, num_of_proc/2) == 0 || sum(table+num_of_proc/2, num_of_proc/2) == 0)
            break;
        getNextCoIdx(&co_idx, leader_id, world_rank, num_of_proc);
    }
}

void exchangeMedians(struct data* points, int *table, int world_rank, int leader_id, int num_of_proc, int* balander_table)
{
    int step = 0;
    int co_idx = world_rank-leader_id < num_of_proc/2 ? num_of_proc/2 + world_rank: world_rank - num_of_proc/2;
    while(1)
    {
        int num_to_change;
        if((balander_table[world_rank-leader_id] + balander_table[co_idx-leader_id]) == 2)
        {
            num_to_change = 0;
        }
        else
        {
            num_to_change = min(table[world_rank-leader_id], table[co_idx-leader_id]);
        }
        if(num_to_change!=0){
            //printf("Process %d: num to send %d in step %d\n", world_rank, num_to_change, step);
            MPI_Request req;
            MPI_Isend(points->points_to_sent+points->num_sent*points->dim, num_to_change*points->dim, MPI_FLOAT, co_idx, 0, MPI_COMM_WORLD, &req);
            //printf("Process %d: what sent %f\n", world_rank, points->points_to_sent[0]);
            MPI_Recv(points->points_recieved+points->num_sent*points->dim, num_to_change*points->dim, MPI_FLOAT, co_idx, 0, MPI_COMM_WORLD, NULL);
            //printf("Process %d: what recieved %f\n", world_rank, points->points_to_sent[0][0]);
            MPI_Status myStatus;
            MPI_Wait(&req, &myStatus);
        }
        updateTable(table, num_of_proc, step);
        step++;
        points->num_sent += num_to_change;
        if(balander_table[world_rank-leader_id]==1)
            points->num_of_median -= num_to_change;
        else
            points->num_to_send -= num_to_change;

        if(step>num_of_proc+1)
            break;
        getNextCoIdx(&co_idx, leader_id, world_rank, num_of_proc);
    }
    points->num_sent = points->num_sent;
}

void updateTable(int* table, int n_process, int step)
{
    for(int i = 0; i < n_process/2; i++)
    {
        int co_idx = (i + step)%(n_process/2) + n_process/2;
        int temp_change = min(table[i], table[co_idx]);
        table[i] -= temp_change;
        table[co_idx] -= temp_change;
    }
}

void updateTablev2(int* table, int n_process, int step, int* balander)
{
    for(int i = 0; i < n_process/2; i++)
    {
        int co_idx = (i + step)%(n_process/2) + n_process/2;
        int temp_change = 0;
        if(balander[i] + balander[co_idx] != 2)
            temp_change = min(table[i], table[co_idx]); 
        table[i] -= temp_change;
        table[co_idx] -= temp_change;
    }
}

void getNextCoIdx(int *co_idx, int leader_id, int world_rank, int num_of_proc)
{
    if(world_rank < num_of_proc/2+leader_id)
    {
        if(*co_idx == num_of_proc+leader_id-1)
            *co_idx = num_of_proc/2 + leader_id;
        else
            (*co_idx)++;
    }
    else
    {
        if(*co_idx == leader_id)
            *co_idx = num_of_proc/2+leader_id-1;
        else
            (*co_idx)--;
    }
}

void groupedBcast_table(int* table, int leader_id, int world_rank, int num_of_proc)
{
    for(int i = 0; i < num_of_proc; i++)
    {
        //to avoid sending to own process
        if(i+leader_id == world_rank)
            continue;
        MPI_Request req;
        MPI_Isend(table+world_rank-leader_id, 1, MPI_INT, i+leader_id, 0, MPI_COMM_WORLD, &req);
    }
    for(int i = 0; i < num_of_proc; i++)
    {
        //to avoid sending to own process
        if(i+leader_id == world_rank)
            continue;
        MPI_Request req;
        MPI_Recv(table+i, 1, MPI_INT, i+leader_id, 0, MPI_COMM_WORLD, NULL);
    }
}