struct data
{
    float** points;     //M points of N dimension (MxN matrix)
    float* dist;        //distances of correspoding process
    float* total_dist; //this is used only by root
    int dim;        //dimension of points
    int num;        //num of points (for this process)
    int proc;       //num of processes
};

//process gets its initial data
void get_file(char *name, int id, struct data *dp);

//given a pivot and data structure, find the distance from pivot point
void points_to_dist(float *pivot, struct data *dp, float* dists);

//quick select implementation 
float quickselect(float *arr, int l, int r, int k);
int partition(float *arr, int l, int r);
void swap(float *arr, int pos1, int pos2);