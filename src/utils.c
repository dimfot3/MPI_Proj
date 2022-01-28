#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "utils.h"
#include "float.h"

void get_points(char *path, struct data *dp, int id, int total_proc, int verbose)
{
    //opten a file that containes the points
    FILE *fp;
    float info[2];
    fp = fopen(path, "rb");
    if ( fp == NULL )
    {
        printf( "Error loading the file with paht %s\n",  path) ;
        exit(0);
    }
    fread(info,sizeof(float),2,fp); // read 10 bytes to our buffer
    dp->num = info[0]/total_proc;
    dp->dim = info[1];
    dp->proc = total_proc;
    if(verbose)
    {
        printf("Loading file %s ...\n", path);
        printf("Number of points: %d, Point dimensions: %d, Num of processes: %d\n", (int)info[0], dp->dim, dp->proc);
        printf("Number of points per process %d\n", dp->num);
    }
    //saved in data structure the points (only the ones that belong in this process)
    dp->points = (float**) malloc(sizeof(float*)*dp->num);
    for(int i = 0; i < (int)info[0]; i++)
    {
        if(i >= dp->num*(id + 1))
            break;
        float temp[dp->dim];
        fread(temp, sizeof(float),dp->dim, fp);
        if(i < dp->num * id)
            continue;
        dp->points[i - dp->num * id] = (float*) malloc(dp->dim*sizeof(float));
        memcpy(dp->points[i - dp->num * id], temp, dp->dim*sizeof(float));
    }
    fclose(fp);
}
 
void swap(float* a, float* b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}

float partition(float *arr, int l, int r)
{
    float lst = arr[r];
    int i = l, j = l;
    while (j < r) {
        if (arr[j] < lst) {
            swap(&arr[i], &arr[j]);
            i++;
        }
        j++;
    }
    swap(&arr[i], &arr[r]);
    return i;
}

float quickselect(float* arr, int left, int right, int k)
{
    double b = -1, a = -1;
    while (left <= right) {
        int pivotIndex = partition(arr, left, right);
        if (pivotIndex == k)
            b = arr[pivotIndex];
        else if(pivotIndex == k-1)
            a = arr[pivotIndex];
        if (pivotIndex >= k)
            right = pivotIndex - 1;
        else
            left = pivotIndex + 1;
        if(b != -1 && a != -1)
            break;
    }
    //if the number of elements is odd return the k-1 element
    if((right-left+1)%2 == 1)
    {
        return a;
    }
    //if the number of elements is even return the mean of k-1 and k.
    return (b+a)/2;
}

float getMin(float *arr, int len)
{
    float min_el = FLT_MAX;
    for(int i = 0; i < len; i++)
    {
        if(arr[i] < min_el)
        {
            min_el = arr[i];
        }
    }
    return min_el;
}

float getMax(float *arr, int len)
{
    float max_el = FLT_MIN;
    for(int i = 0; i < len; i++)
    {
        if(arr[i] > max_el)
        {
            max_el = arr[i];
        }
    }
    return max_el;
}

int sum(int* arr, int len)
{
    int sum_arr = 0;
    for(int i = 0; i < len; i++)
    {
        sum_arr += arr[i];
    }
    return sum_arr;
}

int evaluate_result(float* arr, int len, int num_of_proc)
{
    int suc = 1;
    float min_arr[num_of_proc], max_arr[num_of_proc];
    //find the min max of every process
    for(int i = 0; i < num_of_proc; i++)
    {
        min_arr[i] = getMin(arr+i*len, len);
        max_arr[i] = getMax(arr+i*len, len);
        printf("Process %d: [%f %f]\n", i, min_arr[i], max_arr[i]);
    }
    //checks if the max or i process is smaller than the min of i+1
    for(int i = 0; i < num_of_proc-1; i++)
    {
        if(max_arr[i] > min_arr[i+1])
            suc *= 0;
    }
    return suc;
}

void save_time(struct data *points, int num_of_proc, float time)
{
    FILE *fp;
    fp = fopen("out/res.txt", "a+");
    fprintf(fp, "%d, %d, %d, %f \n", points->num*num_of_proc, points->dim,num_of_proc, time);
    fclose(fp);
}