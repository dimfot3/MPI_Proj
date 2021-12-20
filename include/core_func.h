/**
 * Author: Fotiou Dimitrios
 * AEM: 9650
 * Here there are included some more specific to destribute by median process
 **/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>
#include "utils.h"


/**
 * @brief calculate the distances from a pivot
 * 
 * @param pivot pivot points from which distances are calculated
 * @param dp data struct to save the distances
 * @return void
 */
void calculateDistances(float *pivot, struct data *dp);

//broadcast the median (on specific groups)
void groupedBcast_median(float *median, int leader_id, int world_rank, int num_of_proc);

//split points based on median
void splitByMedian(float median, struct data *points, int lower);

//send distances to leader
void transmitDistances(struct data *points, int world_rank, int leader_id, int num_of_proc);

//exchange procedure between processes
void exchangePoints(struct data* points, int *table, int world_rank, int leader_id, int num_of_proc);

//this used only when median is included in distances
void exchangeMedians(struct data* points, int *table, int world_rank, int leader_id, int num_of_proc, int* balander_table);

//update the exchange table based on step
void updateTable(int* table, int process, int step);

//update the exchange table based on step on median exchanges phase
void updateTablev2(int* table, int n_process, int step, int* balander);

//find who is the next available for exchange points
void getNextCoIdx(int *co_idx, int leader_id, int world_rank, int num_of_proc);

//broadcast the echange table (on specific groups)
void groupedBcast_table(int* table, int leader_id, int world_rank, int num_of_proc);




