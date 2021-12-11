#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <mpi.h>
#include "utils.h"


void line_to_info(char *line, int * arr, int len)
{
    int pos = 0;
    for(int i = 0; i < len-1; i++)
    {
        sscanf(line+=pos, "%d%n", &(arr[i]), &pos);
    }
    sscanf(line+=pos, "%d%n", &(arr[len-1]), &pos);
}

void line_to_arr(char *line, float * arr, int len)
{
    int pos = 0;
    for(int i = 0; i < len-1; i++)
    {
        sscanf(line+=pos, "%f%n", &(arr[i]), &pos);
    }
    sscanf(line+=pos, "%f%n", &(arr[len-1]), &pos);
}

void get_points(char *path, struct data *dp, int id, int total_proc, int verbose)
{
    //finds the name of the file with the id
    FILE *fp;
    char info_buff[50];
    fp = fopen(path, "r");
    fgets(info_buff, 50, (FILE*)fp);

    int *info  = (int*)malloc(2 * sizeof(int));
    line_to_info(info_buff, info, 2);
    assert(total_proc % 2 == 0);
    assert((int)info[0] % total_proc == 0);
    dp->num = info[0]/total_proc;
    dp->dim = info[1];
    dp->proc = total_proc;
    dp->points = (float**) malloc(sizeof(float*)*dp->num);
    if(verbose)
    {
        printf("Loading file %s ...\n", path);
        printf("Number of points: %d, Point dimensions: %d, Num of processes: %d\n", info[0], dp->dim, dp->proc);
        printf("Number of points per process %d\n", dp->num);
    }
    for(int i = 0; i < (int)info[0]; i++)
    {
        if(i >= dp->num*(id + 1))
            break;
        int line_width = dp->dim*(4+5);
        char buff2[line_width];
        fgets(buff2, line_width, (FILE*)fp);
        if(i < dp->num * id)
            continue;
        dp->points[i - dp->num * id] = (float*) malloc(sizeof(float)*dp->dim);
        line_to_arr(buff2, dp->points[i - dp->num * id], dp->dim);
    }
    fclose(fp);
}

void calculateDistances(float *pivot, struct data *dp)
{
    free(dp->dist);
    dp->dist = (float*)malloc(sizeof(float)*dp->num);
    for(int i = 0; i < dp->num; i++)
    {
        float temp = 0;
        for(int j = 0; j < dp->dim; j++)
        {
            float diff = pivot[j] - dp->points[i][j];
            temp += diff*diff;
        }
        dp->dist[i] = (float) sqrt((float)temp);
    }
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
    return (b+a)/2;
}

float getMin(float *arr, int len)
{
    float min = 10000000000;
    for(int i = 0; i < len; i++)
    {
        if(arr[i]<min)
        {
            min = arr[i];
        }
    }
    return min;
}

float getMax(float *arr, int len)
{
    float max = -1000000;
    for(int i = 0; i < len; i++)
    {
        if(arr[i]>max)
        {
            max = arr[i];
        }
    }
    return max;
}

int sum(int* arr, int len)
{
    int sum = 0;
    for(int i = 0; i < len; i++)
    {
        sum += arr[i];
    }
    return sum;
}

int evaluate_result(float* arr, int len, int num_of_proc)
{
    int suc = 1;
    float min_arr[num_of_proc], max_arr[num_of_proc];
    for(int i = 0; i < num_of_proc; i++)
    {
        min_arr[i] = getMin(arr+i*len, len);
        max_arr[i] = getMax(arr+i*len, len);
        printf("Process %d: [%f %f]\n", i, min_arr[i], max_arr[i]);
    }
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
    fprintf(fp, "%d, %d, %f \n", points->num*num_of_proc, num_of_proc, time);
    fclose(fp);
}