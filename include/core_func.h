/**
 * Author: Fotiou Dimitrios
 * @note: AEM:9650
 * 
 * @brief Here there are included some more specific utilities to destribute by median process
 **/

/**
 * @brief calculate the distances from a pivot
 * 
 * @param pivot pivot points from which distances are calculated
 * @param dp data struct to save the distances
 * @return void
 */
void calculateDistances(float *pivot, struct data *dp);

/**
 * @brief this function utlize the transmition of the distances to the leader
 * 
 * @param points the distances are saved in this struct
 * @param world_rank the id of process to call this function
 * @param leader_id the leader process of that group
 * @param num_of_proc the number of process for that group
 * @return void
 */
void transmitDistances(struct data *points, int world_rank, int leader_id, int num_of_proc);

/**
 * @brief process with id=leader_id broadcast the median to next num_of_proc-1 processes
 * 
 * @param median the median to be sent to a group of processes
 * @param leader_id the leader process of that group
 * @param world_rank the id of process to call this function
 * @param num_of_proc the number of process for that group
 * @return void
 */
void groupedBcast_median(float *median, int leader_id, int world_rank, int num_of_proc);

/**
 * @brief splits the points to what belong and that not in a processes based on if the processes 
 * is below the half processes of its group and on the median
 * 
 * @param median float value of the median for which the distribution will be made
 * @param points the data structure of points, where the splited array will be saved to
 * @param lower if 0 then all the points with distance lower than median will be send, else if 1 the opposite.
 * @return void
 */
void splitByMedian(float median, struct data *points, int lower);

/**
 * @brief this function populates the exchange table between the processes. 
 * 
 * @param table the array which will hold the number of points that need to sent each processes. Each process should fill its row before call this fucntion
 * @param leader_id the leader id of the distribution
 * @param world_rank id of the proces that call this function
 * @param num_of_proc the total number of processes
 * @return void
 */
void groupedBcast_table(int* table, int leader_id, int world_rank, int num_of_proc);

/**
 * @brief this function finds the next processes to exchange points.
 * 
 * @param co_idx the current id of process that processes with world_rank id exchanges
 * @param leader_id the leader id of the distribution
 * @param world_rank id of the proces that call this function
 * @param num_of_proc the total number of processes
 * @return void
 */
void getNextCoIdx(int *co_idx, int leader_id, int world_rank, int num_of_proc);

/**
 * @brief this function makes the actual exchange of points based on a table
 * 
 * @param points the distances are saved in this struct
 * @param table the exchange table
 * @param world_rank the id of process to call this function
 * @param leader_id the leader process of that group
 * @param num_of_proc the number of process for that group
 * @return void
 */
void exchangePoints(struct data* points, int *table, int world_rank, int leader_id, int num_of_proc);

/**
 * @brief this function makes the exchange of the remaining points that have not be sent in phase one
 * This function is activated only when median is part of array distances. Two processes that have sent all 
 * their main points can't exchange between them
 * 
 * @param points the distances are saved in this struct
 * @param table the exchange table
 * @param world_rank the id of process to call this function
 * @param leader_id the leader process of that group
 * @param num_of_proc the number of process for that group
 * @param balander_table this table shows which processes have not sent their points, 1: have send, 0:have not sent
 * @return void
 */
void exchangeMedians(struct data* points, int *table, int world_rank, int leader_id, int num_of_proc, int* balander_table);

/**
 * @brief this function update the table of exchanges based on a step
 * 
 * @param table the exchange table
 * @param process the process
 * @param step the exchange step
 * @return void
 */
void updateTable(int* table, int process, int step);

/**
 * @brief this function update the table of exchanges based on a step, and used in median exchanges phase
 * 
 * @param table the exchange table
 * @param process the process
 * @param step the exchange step
 * @return void
 */
void updateTablev2(int* table, int n_process, int step, int* balander);






