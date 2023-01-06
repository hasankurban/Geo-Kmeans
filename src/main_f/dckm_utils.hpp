#include <iostream>
#include <vector>
#include "math.h"
#include <map>
#include <algorithm>
#include "algo_utils.hpp"
#include "misc_utils.hpp"
#include <numeric>
#pragma once

using namespace std;

class dckm_utils{

    template <typename TD>
    void find_midpoints(const vector<TD> &center1, const vector<TD> &center2, 
    vector<vector<vector<TD> > > &midpoint, vector<vector<vector<TD> > > &affine, int &, int &);

    template <typename TD>
    bool find_context_direction(const vector<TD> &actual_point,
    const vector<TD> &centroid_vector, const vector<TD> &midpoint);

    template <typename TD, typename TI>
    void restore_radius(vector<vector <TD> > &dist_matrix,
    vector<TI> &assigned_clusters, 
    vector<vector <TD> > &cluster_size);

    template <typename TD, typename TI>
    void find_neighbors(vector<vector <TD> > &centroids, 
    vector<vector <TD> > &center_dist_mat, vector<vector <TD> > &cluster_size, 
    vector<vector<TI> > &neighbors, vector<vector<vector <TD> > > &mid_points, 
    vector<vector<vector <TD> > > &affine_vectors,  vector<TD> &temp, 
    vector<vector<TD> > &temp_master, vector<TD> &temp_midpoint,
    vector<TD> &temp_affine, vector<vector<TD> > &midpoint_holder, 
    vector<vector<TD> > &affine_holder);

    template <typename TD, typename TI>
    void determine_data_expression(vector<vector<TD> > &dataset, 
    vector<vector <TD> > &centroids, vector<vector<TD> > &cluster_size, 
    vector<vector <TD> > &center_dist_mat, vector<vector<TD> > &dist_mat,
    vector<TI> &assigned_clusters, vector<vector<TI> > &neighbors,
    vector<vector<vector <TD> > > &affine_vectors, 
    vector<vector<vector <TD> > > &mid_points, 
    vector<vector <TI> > &he_data, int &inner_loop_time, int &affine_time, vector<TI> &temp,
    TI &my_cluster, TI &i, TI &j, TD &vec_sum);

    template <typename Tfloat, typename Tint>
    void calculate_HE_distances(const vector<vector<Tfloat> > &dataset, 
    vector<vector<Tfloat> > &centroids, vector<vector<Tfloat> > &dist_mat,
    Tint num_clusters, vector<Tint> &assigned_clusters, 
    vector<vector<Tfloat> > &cluster_size,
    vector<vector <Tint> > &he_data);

    int chk_sign(float &val1);

    bool chk_validity(vector<float> &mid_points, vector<float> &centroid, 
    vector<float> &actual_point, vector<float> &affine_vec);

};


template <typename TD>
inline void find_midpoints(const vector<TD> &center1, const vector<TD> &center2, 
vector<vector<vector<TD> > > &midpoint, vector<vector<vector<TD> > > &affine, int &curr_center, int &ot_cen){

    for (int i=0; i<center1.size(); i++){
        midpoint[curr_center][ot_cen][i] = (center1[i] + center2[i])/2;
        affine[curr_center][ot_cen][i] = center2[i] - midpoint[curr_center][ot_cen][i];
    }
}


template <typename TD>
inline bool find_context_direction(const vector<TD> &actual_point, 
const vector<TD> &centroid_vector, const vector<TD> &midpoint, TD &vec_sum){

    vec_sum = 0.0;
    
    for (int i=0; i<midpoint.size(); i++){
        // temp = actual_point[i] - midpoint[i];
        // res_temp = res_temp + (temp * temp);
        vec_sum = vec_sum + ((actual_point[i] - midpoint[i]) * centroid_vector[i]);
    }

    if (vec_sum>0)
        return true;

    return false;
}


template <typename TD, typename TI>
inline void restore_radius(vector<vector <TD> > &dist_matrix,
vector<TI> &assigned_clusters, 
vector<vector <TD> > &cluster_size){

    for (int i=0; i<cluster_size.size(); i++){
        for (int j=0; j< assigned_clusters.size(); j++){
                if (assigned_clusters[j] == i){
                    if(dist_matrix[j][i] > cluster_size[i][1]){
                            cluster_size[i][1] = dist_matrix[j][i];
                    }
            }
        }
    }
}



template <typename TD, typename TI>
inline void find_neighbors(vector<vector <TD> > &centroids, 
vector<vector <TD> > &center_dist_mat, vector<vector <TD> > &cluster_size, 
vector<vector<TI> > &neighbors, vector<vector<vector <TD> > > &mid_points, 
vector<vector<vector <TD> > > &affine_vectors,  vector<TD> &temp, vector<vector<TD> > &temp_master, vector<TD> &temp_midpoint,
vector<TD> &temp_affine, vector<vector<TD> > &midpoint_holder, vector<vector<TD> > &affine_holder){

    TD dist = 0;
    TD radius = 0;
    algorithm_utils alg_utils;
    vector<TI> temp1;

    int curr_center = 0, ot_center = 0, cnt = 0;

    // Calculate inter-centroid distances
    for(curr_center=0; curr_center<centroids.size(); curr_center++){
        
        radius = cluster_size[curr_center][1];
        cnt = 0;
        
        for (ot_center=0; ot_center<centroids.size(); 
        ot_center++){    
            
            // Do only k calculation :) save some 
            if (curr_center < ot_center){
                dist = alg_utils.calc_euclidean(centroids[curr_center], centroids[ot_center]);
                center_dist_mat[curr_center][ot_center] = dist/2;
                center_dist_mat[ot_center][curr_center] = center_dist_mat[curr_center][ot_center];
            }

            // Start neighbor finding
            if ((curr_center != ot_center) && 
            (center_dist_mat[curr_center][ot_center] < radius)){

                // Create an object of neighbor holder structure
                // and populate the fields inside it.
                // cout << "Curr_center: " << curr_center << " other: " << ot_center << endl;

                temp[0] = center_dist_mat[curr_center][ot_center];
                temp[1] = ot_center;
                temp[2] = cnt;
                temp_master.push_back(temp);
           
                // Get the mid-point coordinates for this pair of centroids
                find_midpoints(centroids[curr_center], centroids[ot_center], mid_points, affine_vectors, curr_center, ot_center);
                cnt++;
            }
        }   

            if (cnt>1){
                // cout << "Curr_center: " << curr_center << " other: " << ot_center << endl;
                
                sort(temp_master.begin(), temp_master.end(), [](const std::vector<TD>& a, const std::vector<TD>& b) {
                    return a[0] < b[0];});

                    // print_2d_vector(temp_master, temp_master.size(), " Sorted neigh: ");
                    // print_2d_vector(midpoint_holder, midpoint_holder.size(), "Midpoint: ");

                for(int i = 0; i<temp_master.size(); i++){
                    temp1.push_back(trunc(temp_master[i][1]));
                }

                neighbors[curr_center] = temp1;
            }

            else if (cnt == 1){
                // print_2d_vector(temp_master, temp_master.size(), " Sorted: ");
                temp1.push_back(temp_master[0][1]);
                neighbors[curr_center] = temp1;
            }

            else if(cnt == 0){
                temp1.push_back(-100);
                neighbors[curr_center] = temp1;
            }

            cluster_size[curr_center][2] = cnt;
            temp_master.clear();
            temp1.clear();
    }
}

// template <typename TD, typename TI>
// inline void determine_data_expression(vector<vector<TD> > &dataset, 
// vector<vector <TD> > &centroids, vector<vector<TD> > &cluster_size, vector<vector <TD> > &center_dist_mat, 
// vector<TI> &assigned_clusters, 
// vector<vector<TI> > &neighbors,
// vector<vector<vector <TD> > > &affine_vectors, 
// vector<vector<vector <TD> > > &mid_points, 
// vector<vector <TI> > &he_data, int &inner_loop_time, int &affine_time, 
// vector<TI> &temp, TI &my_cluster, TI &i, TI &j, TD &vec_sum){
    

//     algorithm_utils alg_utils;

//     for (i = 0; i < assigned_clusters.size(); i++){

//         my_cluster = assigned_clusters[i];

//         // auto t3 = std::chrono::high_resolution_clock::now();
//         if (cluster_size[my_cluster][2] > 0){

//             // cout << my_cluster << "\t" << neighbors[my_cluster][0] << endl; 

//             if(alg_utils.calc_euclidean(dataset[i], centroids[my_cluster]) < center_dist_mat[my_cluster][neighbors[my_cluster][0]]){
//                     continue;
//                 }

//             for (j=0; j<neighbors[my_cluster].size(); j++){ 
                
//                 // if (i == 5){
//                 //   cout << "Point: "  << i << " cluster: " << my_cluster << " first closet neigh: " << neighbors[my_cluster][j] << endl;
//                 //   print_vector(dataset[i], dataset[i].size(), " Coordinataes");
//                 //   print_vector(centroids[my_cluster], centroids[my_cluster].size(), " Centroid:");
//                 //   print_vector(centroids[neighbors[my_cluster][0]], 2, "Cnetroid of 1st neighbor");
//                 //   print_vector(centroids[neighbors[my_cluster][1]], 2, "Cnetroid of 2nd neighbor");
//                 //   print_2d_vector(affine_vectors[my_cluster], affine_vectors[my_cluster].size(), "Affine: ");  
//                 //   print_2d_vector(mid_points[my_cluster], mid_points[my_cluster].size(), " Midpoints");  
//                 // }

//                 // auto t5 = std::chrono::high_resolution_clock::now();
//                 if (find_context_direction(dataset[i], affine_vectors[my_cluster][neighbors[my_cluster][j]], 
//                 mid_points[my_cluster][neighbors[my_cluster][j]], vec_sum)){

//                     if (temp.size() == 0){
//                         temp.push_back(i);
//                         temp.push_back(my_cluster);
//                     }
//                     temp.push_back(neighbors[my_cluster][j]);
//                 }

//                 // auto t6 = std::chrono::high_resolution_clock::now();
//                 // affine_time = affine_time + std::chrono::duration_cast<std::chrono::microseconds>(t6 - t5).count();
//             }

//         }
//         // auto t4 = std::chrono::high_resolution_clock::now();
//         // inner_loop_time = inner_loop_time + std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();

//         if (temp.size()>0){
//             he_data.push_back(temp);
//             temp.clear();
//         }
        
//     }      
// }


template <typename TD, typename TI>
inline void determine_data_expression(vector<vector<TD> > &dataset, 
vector<vector <TD> > &centroids, vector<vector<TD> > &cluster_size, 
vector<vector <TD> > &center_dist_mat, vector<vector<TD> > &dist_mat,
vector<TI> &assigned_clusters, 
vector<vector<TI> > &neighbors,
vector<vector<vector <TD> > > &affine_vectors, 
vector<vector<vector <TD> > > &mid_points, 
vector<vector <TI> > &he_data, int &inner_loop_time, int &affine_time, 
vector<TI> &temp, TI &my_cluster, TI &i, TI &j, TD &temp_result){
    

    algorithm_utils alg_utils;

    for (i = 0; i < assigned_clusters.size(); i++){

        my_cluster = assigned_clusters[i];

        // auto t3 = std::chrono::high_resolution_clock::now();
        if (cluster_size[my_cluster][2] > 0){

            // cout << my_cluster << "\t" << neighbors[my_cluster][0] << endl; 
            
            temp_result = alg_utils.calc_euclidean(dataset[i], centroids[my_cluster]);
            if(temp_result < center_dist_mat[my_cluster][neighbors[my_cluster][0]]){
                    continue;
            }

            dist_mat[i][my_cluster] = temp_result;

            // if(temp_result > cluster_size[neighbors[my_cluster][0]][1])
            //     cluster_size[neighbors[my_cluster][0]][1] = temp_result;

            // if(temp_result < dist_mat[i][assigned_clusters[i]]){
            //     cluster_size[my_cluster][0] -= 1;
            //     assigned_clusters[i] = neighbors[my_cluster][0];
            //     cluster_size[neighbors[my_cluster][0]][0] += 1;
                        
            // } 

            for (j=0; j<neighbors[my_cluster].size(); j++){ 
                
                // if (i == 9){
                //   cout << "Point: "  << i << " cluster: " << my_cluster << " first closet neigh: " << neighbors[my_cluster][j] << endl;
                //   print_vector(dataset[i], dataset[i].size(), " Coordinataes");
                //   print_vector(centroids[my_cluster], centroids[my_cluster].size(), " Centroid:");
                //   print_vector(centroids[neighbors[my_cluster][0]], 2, "Cnetroid of 1st neighbor");
                //   print_vector(centroids[neighbors[my_cluster][1]], 2, "Cnetroid of 2nd neighbor");
                //   print_2d_vector(affine_vectors[my_cluster], affine_vectors[my_cluster].size(), "Affine: ");  
                //   print_2d_vector(mid_points[my_cluster], mid_points[my_cluster].size(), " Midpoints");  
                // }

                // auto t5 = std::chrono::high_resolution_clock::now();
                if (find_context_direction(dataset[i], affine_vectors[my_cluster][neighbors[my_cluster][j]], 
                mid_points[my_cluster][neighbors[my_cluster][j]], temp_result)){
                    
                    // if (i == 17){
                    //     cout << "Old cluster: " << my_cluster << " probable nei: " << neighbors[my_cluster][j] << endl; 
                    //     cout << "Current Distance: " << dist_mat[i][assigned_clusters[i]] << " New dist: " 
                    //     << temp_result << " Distance to neighbor:" << dist_mat[i][neighbors[my_cluster][j]] << endl;

                    //     // cout << "Point: " << he_data[i][0] << " Old cluster: " << assigned_clusters[he_data[i][0]] << " New cluster: " << he_data[i][j] << endl; 
                    //     // cout << "Distance: " << temp << " Current distance to it's own centroid " << dist_mat[he_data[i][0]][assigned_clusters[he_data[i][0]]] << endl;
                    // }

                    temp_result = alg_utils.calc_euclidean(dataset[i], centroids[neighbors[my_cluster][j]]);
                    dist_mat[i][neighbors[my_cluster][j]] = temp_result;

                    if(temp_result > cluster_size[neighbors[my_cluster][j]][1])
                        cluster_size[neighbors[my_cluster][j]][1] = temp_result;

                    if(temp_result < dist_mat[i][assigned_clusters[i]]){
                        cluster_size[assigned_clusters[i]][0] -= 1;
                        assigned_clusters[i] = neighbors[my_cluster][j];
                        cluster_size[neighbors[my_cluster][j]][0] += 1;
                    } 
                    
                    // if (temp.size() == 0){
                    //     temp.push_back(i);
                    //     temp.push_back(my_cluster);
                    // }
                    // temp.push_back(neighbors[my_cluster][j]);
                }

                // auto t6 = std::chrono::high_resolution_clock::now();
                // affine_time = affine_time + std::chrono::duration_cast<std::chrono::microseconds>(t6 - t5).count();
            }

        }
        // auto t4 = std::chrono::high_resolution_clock::now();
        // inner_loop_time = inner_loop_time + std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();

        // if (temp.size()>0){
        //     he_data.push_back(temp);
        //     temp.clear();
        // }
        
    }      
}



template <typename Tfloat, typename Tint>
inline void calculate_HE_distances(const vector<vector<Tfloat> > &dataset, 
vector<vector<Tfloat> > &centroids, vector<vector<Tfloat> > &dist_mat,
Tint num_clusters, vector<Tint> &assigned_clusters, 
vector<vector<Tfloat> > &cluster_size, 
vector<vector <Tint> > &he_data){

    Tfloat temp = 0.0;
    Tint refer_size = 0;
    algorithm_utils alg_utils;

    for (int i=0; i < he_data.size(); i++){

        refer_size = he_data[i].size();

        for (int j = 1; j<refer_size; j++){
            
            temp = alg_utils.calc_euclidean(dataset[he_data[i][0]], centroids[he_data[i][j]]);
            dist_mat[he_data[i][0]][he_data[i][j]] = temp;

            if(temp > cluster_size[he_data[i][j]][1])
                    cluster_size[he_data[i][j]][1] = temp; 

            if(temp < dist_mat[he_data[i][0]][assigned_clusters[he_data[i][0]]]){
                cluster_size[assigned_clusters[he_data[i][0]]][0] = cluster_size[assigned_clusters[he_data[i][0]]][0] - 1;
                
                //cluster_size[assigned_clusters[he_data[i][0]]][1] = 0.0;
                // cout << "Point: " << he_data[i][0] << " Old cluster: " << assigned_clusters[he_data[i][0]] << " New cluster: " << he_data[i][j] << endl; 
                // cout << "Distance: " << temp << " Current distance to it's own centroid " << dist_mat[he_data[i][0]][assigned_clusters[he_data[i][0]]] << endl;
                assigned_clusters[he_data[i][0]] = he_data[i][j];
                cluster_size[he_data[i][j]][0] = cluster_size[he_data[i][j]][0] + 1; 
                
            } 

            //  if (he_data[i][0] == 20){
            //         cout << "Old cluster: " << assigned_clusters[he_data[i][0]] << " Nei: " << he_data[i][j] << endl; 
            //         cout << "Distance: " << temp_result << " Current distance to it's own centroid " 
            //             << dist_mat[i][assigned_clusters[i]] << endl;
            //     }
        }
    }

}
            
