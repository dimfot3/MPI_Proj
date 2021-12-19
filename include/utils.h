/**
 * @file utils.h
 * @author: Fotiou Dimitrios
 * @note: 	AEM:9650
 * @brief Here there are included some generic utilities used in distribute by median process
 **/


#ifndef UTILS_H
#define UTILS_H
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })


/**
 * @brief basic data structue for points
 * 
 */

struct data
{
    float** points;     //M points of N dimension (MxN matrix)
    float* dist;        //distances of correspoding process
    float* total_dist; //this is used only by root
    float* points_to_sent;     //elements that need to be sent, they will be stored in contigious memory!
    float* points_recieved;     //elements that have been recieved, they will be stored in contigious memory!
    int* idx_to_send;     //idx of elements that will be sent in the points array
    int num_to_send; //number of points to send
    int num_sent; //number of points that have been sent already
    int dim;        //dimension of points
    int num;        //num of points (for this process)
    int proc;       //num of processes
    int* num_of_median;     //number of medians, this used to handle the situation where median is included in distances array
};

/**
 * @brief gets a string of integer elements and return the integer elements
 * 
 * @param line the line that contains the info of data(the first line)
 * @param arr array where they will be saved the info
 * @param len number of integer that contained in line
 * @return void
 */
void line_to_info(char *line, int * arr, int len);

/**
 * @brief gets a string of floats elements and return the float elements
 * 
 * @param line the line that contains the info of data(the first line)
 * @param arr array where they will be saved the float numbers
 * @param len number of floats that contained in line
 * @return void
 */
void line_to_arr(char *line, float * arr, int len);

/**
 * @brief gets data corresponding to a process
 * 
 * @param path absolut path to the data
 * @param dp data struct to save the points
 * @param id the current process id (each process load different part of the data)
 * @param total_proc the total number of points
 * @param verbose if 1 prints out some data info
 * @return void
 */
void get_points(char *path, struct data *dp, int id, int total_proc, int verbose);

/**
 * @brief calculate the distances from a pivot
 * 
 * @param pivot pivot points from which distances are calculated
 * @param dp data struct to save the distances
 * @return void
 */
void calculateDistances(float *pivot, struct data *dp);

/**
 * @brief quick select that finds the kth bigger element in a float array
 * 
 * @param arr array to find the kth bigger element
 * @param left first index of the array
 * @param right last index of array
 * @param k kth bigger elemtn to search
 * @return the kth bigger element
 */
float quickselect(float* arr, int left, int right, int k);

/**
 * @brief consider last element as pivot and moves all the smaller to left and greater to right
 * 
 * @param arr array to find the kth bigger element
 * @param left first index of the array
 * @param right last index of array
 * @return the final position of pivot
 */
float partition(float *arr, int l, int r);

/**
 * @brief swap two elements
 * 
 * @param a first element
 * @param b second element
 * @return void
 */
void swap(float* a, float* b);

/**
 * @brief returns the minimum of a float array
 * 
 * @param arr float array to search the minimum
 * @param len length of array
 * @return the minimum of the array
 */
float getMin(float *arr, int len);

/**
 * @brief returns the maximum of a float array
 * 
 * @param arr float array to search the maximum
 * @param len length of array
 * @return the maximum of the array
 */
float getMax(float *arr, int len);

/**
 * @brief returns the sum of a float array
 * 
 * @param arr float array sum its elements
 * @param len length of array
 * @return the sum of the array
 */
int sum(int* arr, int len);

/**
 * @brief evaluates if the distribute by median done correctly
 * 
 * @param arr float array of the distances (from all processes)
 * @param len length of array
 * @param num_of_proc number of processes
 * @return 0 if failed and 1 if succeeded
 */
int evaluate_result(float* arr, int len, int num_of_proc);


/**
 * @brief save the execution times in ./out/res.txt along with some info for the dataset and the number of processes
 * 
 * @param points the data structure of points
 * @param num_of_proc the number of processes
 * @param time the measured exerution time
 * @return void
 */
void save_time(struct data *points, int num_of_proc, float time);
#endif
