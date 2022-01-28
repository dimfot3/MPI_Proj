
/**
 * @file distributeByMedian.h
 * @author: Fotiou Dimitrios
 * @note: 	AEM:9650
 * @brief Here is the main recursive process of destributeByMedain
 **/


/**
 * @brief this is the main recursive distribute by median process
 * 
 * @param leader the leader of the group that will exchange points
 * @param num_of_proc the number of processes in group
 * @param points the points of function that call this functions
 * @param pivot_point the pivot from which we measure the distances
 * @return void
 */
void distributeByMedian(int leader_id, int num_of_proc, struct data* points, float* pivot_point);