#ifndef UTILS_H
#define UTILS_H
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

struct data
{
    float** points;     //M points of N dimension (MxN matrix)
    float* dist;        //distances of correspoding process
    float* total_dist; //this is used only by root
    float* points_to_sent;     //elements that need to be sent, they will be stored in contigious memory!
    int* idx_to_send;     //idx of elements that will be sent in the points array
    int num_to_send; //number of points to send
    int dim;        //dimension of points
    int num;        //num of points (for this process)
    int proc;       //num of processes
};

//process gets data corresponding to a process
void get_points(char *path, struct data *dp, int id, int total_proc, int verbose);

//given a pivot and data structure, find the distance from pivot point
void calculateDistances(float *pivot, struct data *dp);

//quick select implementation 
double quickselect(float* arr, int left, int right, int k);
float partition(float *arr, int l, int r);
void swap(float* a, float* b);

float getMin(float *arr, int len);
float getMax(float *arr, int len);
int sum(int* arr, int len);
int evaluate_result(float* arr, int len, int num_of_proc);


#endif