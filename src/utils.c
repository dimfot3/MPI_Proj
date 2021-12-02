#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "utils.h"

void line_to_arr(char *line, float * arr, int len)
{
    char* pend = line;
    for(int i = 0; i < len; i++)
    {
        arr[i] = strtof(pend, &pend);
    }
}

void get_file(char *name, int id, struct data *dp)
{
    //finds the name of the file with the id
    char c_id = id + '0';
    char path[30]= "";
    char ext[] = ".txt";
    strcat(path, name);
    strncat(path, &c_id, 1);
    strcat(path,ext);
    printf("Loading file %s ...\n", path);

    FILE *fp;
    char buff[255];
    fp = fopen(path, "r");
    fgets(buff, 255, (FILE*)fp);

    float *info  = (float*)malloc(3 * sizeof(float));;
    line_to_arr(buff, info, 3);
    dp->num = info[0]/info[1];
    dp->dim = info[2];
    dp->proc = (int)info[1];
    printf("Number of points(per process): %d, Point dimensions: %d, Processes:%d\n", dp->num, dp->dim, dp->proc);
    dp->points = (float**) malloc(sizeof(float*)*dp->num);
    for(int i = 0; i < dp->num; i++)
    {
        char buff2[50];
        dp->points[i] = (float*) malloc(sizeof(float)*dp->dim);
        fgets(buff2, 50, (FILE*)fp);
        line_to_arr(buff2, dp->points[i], dp->dim);
    }
    fclose(fp);
}

void points_to_dist(float *pivot, struct data *dp, float* dists)
{
    for(int i = 0; i < dp->num; i++)
    {
        float temp = 0;
        for(int j = 0; j < dp->dim; j++)
        {
            float diff = pivot[j] - dp->points[i][j];
            temp += diff*diff;
        }
        dists[i] = (float) sqrt((float)temp);
    }
}
 
void swap(float *arr, int pos1, int pos2)
{
    float temp = arr[pos1];
    arr[pos1] = arr[pos2];
    arr[pos2] = temp;
}

int partition(float *arr, int l, int r)
{
    int x = arr[r], i = l;
    for (int j = l; j <= r - 1; j++) {
        if (arr[j] <= x) {
            swap(arr, i, j);
            i++;
        }
    }
    swap(arr, i, r);
    return i;
}

float quickselect(float* a, int left, int right, int k)
{
    while (left <= right) {
        int pivotIndex = partition(a, left, right);
        if (pivotIndex == k - 1)
            return a[pivotIndex];
        else if (pivotIndex > k - 1)
            right = pivotIndex - 1;
        else
            left = pivotIndex + 1;
    }
    return -1;
}