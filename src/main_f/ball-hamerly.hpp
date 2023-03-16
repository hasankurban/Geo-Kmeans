// //This version is completed by Yong Zheng(413511280@qq.com), Shuyin Xia（380835019@qq.com）, Xingxin Chen, Junkuan Wang.  2020.5.1

// #include <iostream>
// #include <fstream>
// #include <time.h>
// #include <cstdlib>
// #include <algorithm>
// #include <Eigen/Dense>
// #include <vector>
// #include <float.h>
// #include <chrono>

// using namespace std;
// using namespace Eigen;

// typedef double OurType;

// typedef VectorXd VectorOur;

// typedef MatrixXd MatrixOur;

// typedef vector<vector<OurType> > ClusterDistVector;

// typedef vector<vector<unsigned int> > ClusterIndexVector;

// typedef Array<bool, 1, Dynamic> VectorXb;

// struct Neighbor
//     //Define the "neighbor" structure
// {
//     OurType distance;
//     int index;
// };


// typedef vector<Neighbor> SortedNeighbors;

// MatrixOur load_data(string filename);

// inline MatrixOur update_centroids_ham(MatrixOur &dataset, ClusterIndexVector &clusters_point_index, unsigned int k,
//                                   unsigned int dataset_cols, VectorXb &flag,
//                                   unsigned int iteration_counter, MatrixOur &old_centroids);

// inline void update_radius_ham(MatrixOur &dataset, ClusterIndexVector &clusters_point_index, MatrixOur &new_centroids,
//                           ClusterDistVector &point_center_dist,
//                           VectorOur &the_rs, VectorXb &flag, unsigned int iteration_counter, unsigned long long int &cal_dist_num,
//                           unsigned int the_rs_size);

// inline SortedNeighbors
// get_sorted_neighbors_ham(VectorOur &the_rs, MatrixOur &centers_dist, unsigned int now_ball, unsigned int k,
//                      vector<unsigned int> &now_center_index, vector<unsigned int> &new_in_center_index);

// inline void
// cal_centers_dist_ham(MatrixOur &new_centroids, unsigned int iteration_counter, unsigned int k, VectorOur &the_rs,
//                  VectorOur &delta, MatrixOur &centers_dist);

// inline MatrixOur cal_dist_ham(MatrixOur &dataset, MatrixOur &centroids);

// inline MatrixOur
// cal_ring_dist_ham(unsigned int data_num, unsigned int dataset_cols, MatrixOur &dataset, MatrixOur &now_centers,
//               vector<unsigned int> &now_data_index);

// void initialize_ham(MatrixOur &dataset, MatrixOur &centroids, VectorOur &labels, ClusterIndexVector &clusters_point_index,
//                 ClusterIndexVector &clusters_neighbors_index,
//                 ClusterDistVector &point_center_dist, VectorOur &lx);


// MatrixOur init_centroids(MatrixOur &dataset, int num_cluster);

// bool check_convergence(MatrixOur&new_centroids, 
// MatrixOur&centroids, float threshold);

// bool check_convergence(MatrixOur& new_centroids, 
// MatrixOur&centroids, float threshold){

//     int i =0, j =0;
//     float temp_diff = 0;
//     float diff = 0;

//     if (new_centroids.rows() == centroids.rows()){
        
//         for (i=0; i<new_centroids.rows(); i++){
//             for (j=0; j<new_centroids.cols(); j++)
//                 temp_diff = new_centroids(i, j) - centroids(i, j);
//                 diff = diff + (temp_diff * temp_diff);
//         }
//         diff = sqrt(diff/new_centroids.rows());
//     }
//     else
//         return false;
    
//     if (diff <= threshold)
//         return true;
    
//     return false;
// }


// MatrixOur init_ball_centroids(MatrixOur &dataset, int num_cluster){

//     MatrixOur centroids(num_cluster, dataset.cols());;

//     for(int i=0; i<num_cluster; i++){  
//         for(int j=0; j<dataset.cols(); j++){
//                 centroids(i, j) = dataset(i, j);
//         }   
//     }

//     return centroids;
// }

// // The following function is taken from the following question thread.
// // https://stackoverflow.com/questions/20734774/random-array-generation-with-no-duplicates
// void get_ball_ranodm_indices(int *arr, size_t size, int seed)
// {
//     if (size > 1) 
//     {
//         size_t i;
//         srand(seed);
//         for (i = 0; i < size - 1; i++) 
//         {
//           size_t j = i + rand() / (RAND_MAX / (size - i) + 1);
//           int t = arr[j];
//           arr[j] = arr[i];
//           arr[i] = t;
//         }
//     }
// }


// void extract_ball_data(MatrixOur &dataset, 
// MatrixOur &extracted_data, int data_prop, int num_points, int num_cluster, int seed)
// {

//     int i = 0, j = 0, size = dataset.rows();
//     int test_array[size];

//     for (i = 0; i<size ; i++){
//         test_array[i] = i;
//     }

//     get_ball_ranodm_indices(test_array, size, seed);

//     for(i=0; i<num_points; i++){  
//         for(j=0; j<dataset.cols(); j++){
//             extracted_data(i, j) = dataset(test_array[i], j);
//         }   
//     }
// }

// output_data run_ham(MatrixOur& dataset, MatrixOur& centroids, bool detail, 
// double thres= 0.001, int iters = 100, int time_limit = 60000) {

//     double start_time, end_time;
//     int count = 0;
//     bool judge = true;

//     const unsigned int dataset_rows = dataset.rows();
//     const unsigned int dataset_cols = dataset.cols();
//     const unsigned int k = centroids.rows();

//     OurType stable_field_dist = 0;
//     OurType max_delta = 0;
//     OurType sub_delta;
//     OurType sub_d;

//     ClusterIndexVector temp_clusters_point_index;
//     ClusterIndexVector clusters_point_index;
//     ClusterIndexVector clusters_neighbors_index;

//     ClusterDistVector point_center_dist;

//     MatrixOur new_centroids(k, dataset_cols);
//     MatrixOur old_centroids = centroids;
//     MatrixOur centers_dist(k, k);

//     VectorXb flag(k);
//     VectorXb old_flag(k);

//     VectorOur labels(dataset_rows);
//     VectorOur delta(k);

//     VectorXi t_d(k);


//     vector<unsigned int> now_data_index;
//     vector<unsigned int> old_now_index;
//     vector<unsigned int> new_in_center_index;

//     VectorOur the_rs(k);
//     VectorOur lx(dataset_rows);

//     unsigned int now_centers_rows;
//     unsigned int iteration_counter;
//     unsigned int num_of_neighbour;
//     unsigned int neighbour_num;
//     unsigned long long int cal_dist_num;
//     unsigned int data_num;

//     //bool key = true;

//     MatrixOur::Index minCol;
//     new_centroids.setZero();
//     iteration_counter = 0;
//     num_of_neighbour = 0;
//     cal_dist_num = 0;
//     flag.setZero();

//     output_data res;

//     auto start = std::chrono::high_resolution_clock::now();

//     //initialize_ham clusters_point_index and point_center_dist
//     initialize_ham(dataset, centroids, labels, clusters_point_index, clusters_neighbors_index, point_center_dist, lx);

//     temp_clusters_point_index.assign(clusters_point_index.begin(), clusters_point_index.end());

//     start_time = clock();

//     // while (true) {

//     while (iteration_counter < iters) {

//         old_flag = flag;
//         //record clusters_point_index from the previous round
//         clusters_point_index.assign(temp_clusters_point_index.begin(), temp_clusters_point_index.end());
//         iteration_counter += 1;


//         //update the matrix of centroids
//         new_centroids = update_centroids_ham(dataset, clusters_point_index, k, dataset_cols, flag, iteration_counter,
//                                          old_centroids);

//         // if (new_centroids != old_centroids) {

//         if (check_convergence(new_centroids, old_centroids, thres) == false) {
//             //delta: distance between each center and the previous center
//             MatrixOur::Index max_change_cluster;
//             delta = (((new_centroids - old_centroids).rowwise().squaredNorm())).array().sqrt();
//             //todo sub_delta || max_delta
//             //max_delta = delta.maxCoeff(&max_change_cluster);

//             old_centroids = new_centroids;

//             //get the radius of each centroids
//             update_radius_ham(dataset, clusters_point_index, new_centroids, point_center_dist, the_rs, flag,
//                           iteration_counter, cal_dist_num, k);
            
//             //Calculate distance between centers
//             cal_centers_dist_ham(new_centroids, iteration_counter, k, the_rs, delta, centers_dist);

//             flag.setZero();


//             //nowball;
//             unsigned int now_num = 0;
//             for (unsigned int now_ball = 0; now_ball < k; now_ball++) {
//                 new_in_center_index.clear();
//                 SortedNeighbors neighbors = get_sorted_neighbors_ham(the_rs, centers_dist, now_ball, k,
//                                                                  clusters_neighbors_index[now_ball],
//                                                                  new_in_center_index);

//                 now_num = point_center_dist[now_ball].size();
//                 if (the_rs(now_ball) == 0) continue;

//                 //Get the coordinates of the neighbors and neighbors of the current ball
//                 old_now_index.clear();
//                 old_now_index.assign(clusters_neighbors_index[now_ball].begin(),
//                                      clusters_neighbors_index[now_ball].end());
//                 clusters_neighbors_index[now_ball].clear();
//                 neighbour_num = neighbors.size();
//                 MatrixOur now_centers(neighbour_num, dataset_cols);

//                 sub_delta = 0;
//                 t_d.setZero();
//                 int t_index = 0;
//                 for (unsigned int i = 0; i < neighbour_num; i++) {
//                     t_index = neighbors[i].index;
//                     t_d(t_index) = 1;
//                     clusters_neighbors_index[now_ball].push_back(t_index);
//                     now_centers.row(i) = new_centroids.row(neighbors[i].index);
//                     //todo sub_delta || max_delta
//                     if (t_index != now_ball && sub_delta < delta(t_index)) sub_delta = delta(t_index);
//                 }


//                 num_of_neighbour += neighbour_num;

//                 now_centers_rows = now_centers.rows();

//                 judge = true;

//                 if (clusters_neighbors_index[now_ball] != old_now_index)
//                     judge = false;
//                 else {
//                     for (int i = 0; i < clusters_neighbors_index[now_ball].size(); i++) {
//                         if (old_flag(clusters_neighbors_index[now_ball][i]) != false) {
//                             judge = false;
//                             break;
//                         }
//                     }
//                 }

//                 if (judge) {
//                     continue;
//                 }

//                 now_data_index.clear();

//                 stable_field_dist = the_rs(now_ball);

//                 for (unsigned int j = 1; j < neighbour_num; j++) {
//                     stable_field_dist = min(stable_field_dist,
//                                             centers_dist(clusters_neighbors_index[now_ball][j], now_ball) / 2);
//                 }

//                 OurType min_val = DBL_MAX;
//                 OurType temp_val = 0;
//                 OurType temp_d = DBL_MAX;

//                 for (auto it : new_in_center_index) {
//                     temp_val = centers_dist(now_ball, it) - stable_field_dist;
//                     temp_d = min(temp_d, centers_dist(now_ball, it) - the_rs(now_ball));
//                     if (temp_val < min_val) min_val = temp_val;
//                 }


//                 for (unsigned int i = 0; i < now_num; i++) {
//                     lx(clusters_point_index[now_ball][i]) -= sub_delta;
//                     if (point_center_dist[now_ball][i] > stable_field_dist &&
//                         point_center_dist[now_ball][i] <= lx(clusters_point_index[now_ball][i])) {

//                         lx(clusters_point_index[now_ball][i]) = min(temp_d, lx(clusters_point_index[now_ball][i]));

//                         if (point_center_dist[now_ball][i] >=
//                             max(lx(clusters_point_index[now_ball][i]), stable_field_dist)) {
//                             now_data_index.push_back(clusters_point_index[now_ball][i]);
//                         }
//                     } else {
//                         lx(clusters_point_index[now_ball][i]) = min(min_val, lx(clusters_point_index[now_ball][i]));
//                         if (point_center_dist[now_ball][i] >=
//                             max(lx(clusters_point_index[now_ball][i]), stable_field_dist)) {
//                             now_data_index.push_back(clusters_point_index[now_ball][i]);
//                         }
//                     }
//                 }


//                 data_num = now_data_index.size();

//                 if (data_num == 0) {
//                     continue;
//                 }

//                 MatrixOur temp_distance = cal_ring_dist_ham(data_num, dataset_cols, dataset, now_centers, now_data_index);

//                 cal_dist_num += (data_num * now_centers_rows);


//                 OurType firVal, secVal, temp;
//                 unsigned int new_label;
//                 int firIndex, secIndex;

//                 for (unsigned int i = 0; i < data_num; i++) {
//                     firVal = DBL_MAX;
//                     secVal = DBL_MAX;
//                     firIndex = 0;

//                     for (int j = 0; j < temp_distance.cols(); j++) {
//                         if (firVal > temp_distance(i, j)) {
//                             secVal = firVal;
//                             firVal = temp_distance(i, j);
//                             firIndex = j;
//                         } else if (secVal > temp_distance(i, j)) {
//                             secVal = temp_distance(i, j);
//                         }
//                     }

// //                    if (secVal < sub_d) lx(now_data_index[i]) = sqrt(secVal);
//                     lx(now_data_index[i]) = sqrt(secVal);

//                     new_label = clusters_neighbors_index[now_ball][firIndex];

//                     if (labels[now_data_index[i]] != new_label) {
//                         flag(now_ball) = true;
//                         flag(new_label) = true;

//                         //Update localand global labels
//                         auto it = (temp_clusters_point_index[labels[now_data_index[i]]]).begin();
//                         while ((it) != (temp_clusters_point_index[labels[now_data_index[i]]]).end()) {
//                             if (*it == now_data_index[i]) {
//                                 it = (temp_clusters_point_index[labels[now_data_index[i]]]).erase(it);
//                                 break;
//                             } else {
//                                 ++it;
//                             }
//                         }
//                         temp_clusters_point_index[new_label].push_back(now_data_index[i]);
//                         labels[now_data_index[i]] = new_label;
//                     }
//                 }
//             }

//         } else {
//             break;
//         }

//         auto temp_end = std::chrono::high_resolution_clock::now();
//         auto temptime = std::chrono::duration_cast<std::chrono::milliseconds>(temp_end - start);

//         if (temptime.count() >= time_limit){
//             res.loop_counter = iteration_counter;
//             res.num_he = cal_dist_num;
//             res.runtime = float(temptime.count());
//             res.timeout = false;
//             cout << "Ball-Kmeans Timed Out :(" << endl;
//             return res;
//         }
//     }
//     end_time = clock();

//     if (detail == true) {
//         cout << "ball-k-means with dividing ring:" << endl;
//         cout << "k                :                  ||" << k << endl;
//         cout << "iterations       :                  ||" << iteration_counter << endl;
//         cout << "The number of calculating distance: ||" << cal_dist_num << endl;
//         cout << "The number of neighbors:            ||" << num_of_neighbour << endl;
//         cout << "Time per round:                     ||"
//              << (double)(end_time - start_time) / CLOCKS_PER_SEC * 1000 / iteration_counter << endl;

//     }
//     auto end = std::chrono::high_resolution_clock::now();
//     auto Totaltime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

//     res.loop_counter = iteration_counter;
//     res.num_he = cal_dist_num;
//     res.runtime = float(Totaltime.count());
//     res.timeout = false;

//     return res;

// }

// MatrixOur load_data(string filename) {
//     /*

//     *Summary: Read data through file path

//     *Parameters:

//     *     filename: file path.*    

//     *Return : Dataset in eigen matrix format.

//     */

//     int x = 0, y = 0;  // x: rows  ，  y/x: cols
//     ifstream inFile(filename, ios::in);
//     string lineStr;
//     while (getline(inFile, lineStr)) {
//         stringstream ss(lineStr);
//         string str;
//         while (getline(ss, str, ','))
//             y++;
//         x++;
//     }
//     MatrixOur data(x, y / x);
//     ifstream inFile2(filename, ios::in);
//     string lineStr2;
//     int i = 0;
//     while (getline(inFile2, lineStr2)) {
//         stringstream ss2(lineStr2);
//         string str2;
//         int j = 0;
//         while (getline(ss2, str2, ',')) {
//             data(i, j) = atof(const_cast<const char*>(str2.c_str()));
//             j++;
//         }
//         i++;
//     }
//     return data;
// }

// MatrixOur load_centroids(string filename, int num_clusters, int numCols) {
//     /*

//     *Summary: Read data through file path

//     *Parameters:

//     *     filename: file path.*    

//     *Return : Dataset in eigen matrix format.

//     */
    
//     MatrixOur data(num_clusters, numCols);
//     ifstream inFile2(filename, ios::in);
//     string lineStr2;

//     int counter = 0, i =0;
    
//     while (getline(inFile2, lineStr2)) {
//         if (counter < num_clusters){
//             stringstream ss2(lineStr2);
//             string str2;
//             int j = 0;
//             while (getline(ss2, str2, ',')) {
//                 data(i, j) = atof(const_cast<const char*>(str2.c_str()));
//                 j++;
//             }
//             i++;
//         }
//         else{
//             break;
//         }
//         counter += 1;
//     }
//     return data;
// }

// inline MatrixOur update_centroids_ham(MatrixOur &dataset, ClusterIndexVector &clusters_point_index, unsigned int k,
//                                   unsigned int dataset_cols, VectorXb &flag, unsigned int iteration_counter,
//                                   MatrixOur &old_centroids) {
//     /*

//     *Summary: Update the center point of each cluster

//     *Parameters:

//     *     dataset: dataset in eigen matrix format.*   

//     *     clusters_point_index: global position of each point in the cluster.* 

//     *     k: number of center points.*  

//     *     dataset_cols: data set dimensions*  

//     *     flag: judgment label for whether each cluster has changed.*  

//     *     iteration_counter: number of iterations.*  

//     *     old_centroids: center matrix of previous round.*  

//     *Return : updated center matrix.

//     */

//     unsigned int cluster_point_index_size = 0;
//     unsigned int temp_num = 0;
//     MatrixOur new_c(k, dataset_cols);
//     VectorOur temp_array(dataset_cols);

//     for (unsigned int i = 0; i < k; i++) {
//         temp_num = 0;
//         temp_array.setZero();
//         cluster_point_index_size = clusters_point_index[i].size();
//         if (flag(i) != 0 || iteration_counter == 1) {
//             for (unsigned int j = 0; j < cluster_point_index_size; j++) {
//                 temp_array += dataset.row(clusters_point_index[i][j]);
//                 temp_num++;
//             }
//             new_c.row(i) = temp_array / temp_num;
//         } else new_c.row(i) = old_centroids.row(i);
//     }
//     return new_c;
// }

// inline void update_radius_ham(MatrixOur &dataset, ClusterIndexVector &clusters_point_index, MatrixOur &new_centroids,
//                           ClusterDistVector &point_center_dist, VectorOur &the_rs, VectorXb &flag,
//                           unsigned int iteration_counter, unsigned long long int &cal_dist_num, unsigned int the_rs_size) {
//     /*

//     *Summary: Update the radius of each cluster

//     *Parameters:

//     *     dataset: dataset in eigen matrix format.*   

//     *     clusters_point_index: global position of each point in the cluster.* 

//     *     new_centroids: updated center matrix.*  

//     *     point_center_dist: distance from point in cluster to center*  

//     *     the_rs: The radius of each cluster.*  

//     *     flag: judgment label for whether each cluster has changed.*  

//     *     iteration_counter: number of iterations.*  

//     *     cal_dist_num: distance calculation times.* 

//     *     the_rs_size: number of clusters.* 

//     */


//     OurType temp = 0;
//     unsigned int cluster_point_index_size = 0;

//     for (unsigned int i = 0; i < the_rs_size; i++) {
//         cluster_point_index_size = clusters_point_index[i].size();
//         if (flag(i) != 0 || iteration_counter == 1) {
//             the_rs(i) = 0;
//             point_center_dist[i].clear();
//             for (unsigned int j = 0; j < cluster_point_index_size; j++) {
//                 cal_dist_num++;
//                 temp = sqrt((new_centroids.row(i) - dataset.row(clusters_point_index[i][j])).squaredNorm());
//                 point_center_dist[i].push_back(temp);
//                 if (the_rs(i) < temp) the_rs(i) = temp;
//             }
//         }
//     }
// };

// bool LessSort_ham(Neighbor a, Neighbor b) {
//     return (a.distance < b.distance);
// }

// inline SortedNeighbors
// get_sorted_neighbors_ham(VectorOur &the_rs, MatrixOur &centers_dist, unsigned int now_ball, unsigned int k,
//                      vector<unsigned int> &now_center_index, vector<unsigned int> &new_in_center_index) {
//     /*

//     *Summary: Get the sorted neighbors

//     *Parameters:

//     *     the_rs: the radius of each cluster.*   

//     *     centers_dist: distance matrix between centers.* 

//     *     now_ball: current ball label.*  

//     *     k: number of center points*  

//     *     now_center_index: nearest neighbor label of the current ball.*  

//     */

//     VectorXi flag = VectorXi::Zero(k);
//     SortedNeighbors neighbors;

//     Neighbor temp;
//     temp.distance = 0;
//     temp.index = now_ball;
//     neighbors.push_back(temp);
//     flag(now_ball) = 1;


//     for (unsigned int j = 1; j < now_center_index.size(); j++) {
//         if (centers_dist(now_ball, now_center_index[j]) == 0 ||
//             2 * the_rs(now_ball) - centers_dist(now_ball, now_center_index[j]) < 0) {
//             flag(now_center_index[j]) = 1;
//         } else {
//             flag(now_center_index[j]) = 1;
//             temp.distance = centers_dist(now_ball, now_center_index[j]);
//             temp.index = now_center_index[j];
//             neighbors.push_back(temp);
//         }
//     }


//     for (unsigned int j = 0; j < k; j++) {
//         if (flag(j) != 1 && centers_dist(now_ball, j) != 0 && 2 * the_rs(now_ball) - centers_dist(now_ball, j) >= 0) {
//             new_in_center_index.push_back(j);
//             temp.distance = centers_dist(now_ball, j);
//             temp.index = j;
//             neighbors.push_back(temp);
//         }

//     }


//     return neighbors;
// }


// inline void
// cal_centers_dist_ham(MatrixOur &new_centroids, unsigned int iteration_counter, unsigned int k, VectorOur &the_rs,
//                  VectorOur &delta, MatrixOur &centers_dist) {
//     /*

//     *Summary: Calculate the distance matrix between center points

//     *Parameters:

//     *     new_centroids: current center matrix.*   

//     *     iteration_counter: number of iterations.* 

//     *     k: number of center points.*  

//     *     the_rs: the radius of each cluster*  

//     *     delta: distance between each center and the previous center.*  

//     *     centers_dist: distance matrix between centers.*  

//     */

//     if (iteration_counter == 1) centers_dist = cal_dist_ham(new_centroids, new_centroids).array().sqrt();
//     else {
//         for (unsigned int i = 0; i < k; i++) {
//             for (unsigned int j = 0; j < k; j++) {
//                 if (centers_dist(i, j) >= 2 * the_rs(i) + delta(i) + delta(j))
//                     centers_dist(i, j) = centers_dist(i, j) - delta(i) - delta(j);
//                 else {
//                     centers_dist(i, j) = sqrt((new_centroids.row(i) - new_centroids.row(j)).squaredNorm());
//                 }
//             }
//         }
//     }
// }

// inline MatrixOur cal_dist_ham(MatrixOur &dataset, MatrixOur &centroids) {
//     /*

//     *Summary: Calculate distance matrix between dataset and center point

//     *Parameters:

//     *     dataset: dataset matrix.*   

//     *     centroids: centroids matrix.* 

//     *Return : distance matrix between dataset and center point.

//     */

//     return (((-2 * dataset * (centroids.transpose())).colwise() + dataset.rowwise().squaredNorm()).rowwise() +
//             (centroids.rowwise().squaredNorm()).transpose());
// }

// inline MatrixOur
// cal_ring_dist_ham(unsigned int data_num, unsigned int dataset_cols, MatrixOur &dataset, MatrixOur &now_centers,
//               vector<unsigned int> &now_data_index) {
//     /*

//     *Summary: Calculate the distance matrix from the point in the ring area to the corresponding nearest neighbor

//     *Parameters:

//     *     data_num: number of points in the ring area.*   

//     *     dataset_cols: data set dimensions.* 

//     *     dataset: dataset in eigen matrix format.* 

//     *     now_centers: nearest ball center matrix corresponding to the current ball.* 

//     *     now_data_index: labels for points in the ring.* 

//     *Return : distance matrix from the point in the ring area to the corresponding nearest neighbor.

//     */

//     MatrixOur data_in_area(data_num, dataset_cols);

//     for (unsigned int i = 0; i < data_num; i++) {
//         data_in_area.row(i) = dataset.row(now_data_index[i]);
//     }

//     return (((-2 * data_in_area * (now_centers.transpose())).colwise() +
//              data_in_area.rowwise().squaredNorm()).rowwise() + (now_centers.rowwise().squaredNorm()).transpose());
// }

// void initialize_ham(MatrixOur &dataset, MatrixOur &centroids, VectorOur &labels, ClusterIndexVector &clusters_point_index,
//                 ClusterIndexVector &clusters_neighbors_index, ClusterDistVector &point_center_dist, VectorOur &lx) {
//     /*

//     *Summary: initialize_ham related variables

//     *Parameters:

//     *     dataset: dataset in eigen matrix format.*   

//     *     centroids: dcentroids matrix.* 

//     *     labels: the label of the cluster where each data is located.* 

//     *     clusters_point_index: two-dimensional vector of data point labels within each cluster.* 

//     *     clusters_neighbors_index: two-dimensional vector of neighbor cluster labels for each cluster.* 

//     *     point_center_dist: distance from point in cluster to center.* 

//     */

//     MatrixOur::Index minCol;
//     for (int i = 0; i < centroids.rows(); i++) {
//         clusters_point_index.push_back(vector<unsigned int>());
//         clusters_neighbors_index.push_back(vector<unsigned int>());
//         point_center_dist.push_back(vector<OurType>());
//     }
//     MatrixOur M = cal_dist_ham(dataset, centroids);
//     OurType secVal = -1;
//     for (int i = 0; i < dataset.rows(); i++) {
//         M.row(i).minCoeff(&minCol);
//         labels[i] = minCol;
//         secVal = -1;
//         for (int j = 0; j < M.cols(); j++) {
//             if (j == minCol) continue;
//             if (secVal == -1) secVal = M(i, j);
//             else if (M(i, j) < secVal) secVal = M(i, j);
//         }
//         lx(i) = sqrt(secVal);
//         clusters_point_index[minCol].push_back(i);
//     }
// }


// // int main(int argc, char *argv[]) {
// //     MatrixOur dataset = load_data(
// //             R"(/Users/schmuck/Documents/Box Sync/Ph.D./ball-k-means/data+centers(1)/dataset/ijcnn.csv)");
// //     MatrixOur centroids = load_data(
// //             R"(/Users/schmuck/Documents/Box Sync/Ph.D./ball-k-means/data+centers(1)/centroids/ijcnn6.csv)");
    
// //     cout << "Running Hamerly" << "\n";

// //     auto t3 = std::chrono::high_resolution_clock::now();
// //     run(dataset, centroids);
    
// //     auto t4 = std::chrono::high_resolution_clock::now();
// //     auto km_int = std::chrono::duration_cast<std::chrono::seconds>(t4 - t3);
// //     std::cout << "\nTotal time: " << km_int.count() << " seconds\n";
// // }