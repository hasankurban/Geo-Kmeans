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
    vector<TD> &midpoint, vector<TD> &affine);

    template <typename TD>
    void find_affine_vector(vector<TD> &midpoint, vector<TD> &center, 
    vector<TD> &affine);

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
    vector<vector<TI> > &neighbors, vector<vector<TI> > &neighbor_indices, vector<vector<vector <TD> > > &midpoints, 
    vector<vector<vector <TD> > > &affine_vectors);
    
    // template <typename TD, typename TI>
    // void determine_data_expression(vector<vector<TD> > &dataset,
    // vector<vector <TD>> &centroids,
    // vector<vector <TD> > &dist_mat, vector<vector<TD> > &cluster_size, 
    // vector<TI> &assigned_clusters, vector<vector<TI> > &neighbors,
    // vector<vector<vector <TD> > > &affine_vectors, 
    // vector<vector<vector <TD> > > &mid_points,
    // vector<vector <TI> > &he_data, int &inner_loop_time, int &affine_time);

    template <typename TD, typename TI>
    void determine_data_expression(vector<vector<TD> > &dataset, 
    vector<vector <TD> > &centroids, vector<vector<TD> > &cluster_size, 
    vector<TI> &assigned_clusters, vector<TI> &neighbors, vector<vector<TI> > &neighbor_indices,
    vector<vector<vector <TD> > > &affine_vectors, vector<vector<vector <TD> > > &mid_points, 
    vector<vector <TI> > &he_data, int &inner_loop_time, int &affine_time);

    template <typename TDouble, typename Tint>
    void calculate_HE_distances(const vector<vector<TDouble> > &dataset, 
    vector<vector<TDouble> > &centroids, vector<vector<TDouble> > &dist_mat,
    Tint num_clusters, vector<Tint> &assigned_clusters, 
    vector<vector<TDouble> > &cluster_size,
    vector<vector <Tint> > &he_data);

    int chk_sign(double &val1);

    bool chk_validity(vector<double> &mid_points, vector<double> &centroid, 
    vector<double> &actual_point, vector<double> &affine_vec);

};


template <typename TD>
void find_midpoints(const vector<TD> &center1, const vector<TD> &center2, 
vector<TD> &midpoint, vector<TD> &affine){

    for (int i=0; i<center1.size(); i++){
        midpoint[i] = (center1[i] + center2[i])/2;
        affine[i] = center2[i] - midpoint[i];
    }
}


template <typename TD>
bool find_context_direction(const vector<TD> &actual_point, 
const vector<TD> &centroid_vector, const vector<TD> &midpoint){

    int mysize = midpoint.size(); 
    TD vec_sum = 0.0;
    TD temp = 0.0;
    
    for (int i=0; i<mysize; i++){
        temp = actual_point[i] - midpoint[i];
        vec_sum = vec_sum + (temp * centroid_vector[i]);
    }

    if (vec_sum>0)
        return true;

    return false;
    // return false;
}


template <typename TD, typename TI>
void restore_radius(vector<vector <TD> > &dist_matrix,
vector<TI> &assigned_clusters, 
vector<vector <TD> > &cluster_size){

    for (int i=0; i<cluster_size.size(); i++){
        for (int j=0; j< assigned_clusters.size(); j ++){
                if (assigned_clusters[j] == i){
                    if(dist_matrix[j][i] > cluster_size[i][1]){
                            cluster_size[i][1] = dist_matrix[j][i];
                    }
            }
        }
    }
}


// template <typename TD, typename TI>
// void find_neighbors(vector<vector <TD> > &centroids, 
// vector<vector <TD> > &center_dist_mat, vector<vector <TD> > &cluster_size, 
// vector<vector<TI> > &neighbors, vector<vector<vector <TD> > > &mid_points, 
// vector<vector<vector <TD> > > &affine_vectors){

//     TD dist = 0;
//     TD radius = 0;
//     algorithm_utils alg_utils;

//     vector<TD> temp(3);
//     vector<vector<TD> > temp_master;
//     vector<TD> temp_midpoint(centroids[0].size());
//     vector<TD> temp_affine(centroids[0].size());

//     vector<vector<TD> > midpoint_holder;
//     vector<vector<TD> > affine_holder;

//     neighbors.clear();
//     mid_points.clear();
//     affine_vectors.clear();

//     int ot_center = 0;
//     int curr_center = 0;

//     // Calculate inter-centroid distances
//     for(curr_center=0; curr_center<centroids.size(); curr_center++){
        
//         radius = cluster_size[curr_center][1];
//         vector<TI> temp1;
//         int cnt = 0;
        
//         for (ot_center=0; ot_center<centroids.size(); 
//         ot_center++){    
            
//             // Do only k calculation :) save some 
//             if (center_dist_mat[curr_center][ot_center] == 0){
//                 dist = alg_utils.calc_euclidean(centroids[curr_center], centroids[ot_center]);
//                 center_dist_mat[curr_center][ot_center] = dist/2;
//                 center_dist_mat[ot_center][curr_center] = center_dist_mat[curr_center][ot_center];
//             }

//             // Start neighbor finding
//             if ((curr_center != ot_center) && 
//             (center_dist_mat[curr_center][ot_center] < radius)){

//                 // Create an object of neighbor holder structure
//                 // and populate the fields inside it.

//                 temp[0] = center_dist_mat[curr_center][ot_center];
//                 temp[1] = ot_center;
//                 temp[2] = cnt;
//                 temp_master.push_back(temp);

//                 // temp.push_back(ot_center);
           
//                 // Get the mid-point coordinates for this pair of centroids
//                 find_midpoints(centroids[curr_center], centroids[ot_center], 
//                                     temp_midpoint, temp_affine);
                
//                 midpoint_holder.push_back(temp_midpoint);
//                 affine_holder.push_back(temp_affine);

//                 cnt++;
//             }
//         }   

//             if (temp_master.size()>1){
//                 // vector<int> sorted_index(temp_master.size());
//                 vector<vector<double> > temp123;
//                 vector<vector<double> > temp234;

//                 // iota(sorted_index.begin(), sorted_index.end(), 0);
                
//                 sort(temp_master.begin(), temp_master.end(), [](const std::vector<TD>& a, const std::vector<TD>& b) {
//                     return a[0] < b[0];});

//                 // print_2d_vector(temp_master, temp_master.size(), "Sorted");

//                 // sort(sorted_index.begin(), sorted_index.end(), 
//                 // [&](int i, int j){return temp_master[i][0] < temp_master[j][0];} );

//                 for(int i = 0; i<temp_master.size(); i++){
//                     temp1.push_back(trunc(temp_master[i][1]));
//                     temp123.push_back(midpoint_holder[temp_master[i][2]]);
//                     temp234.push_back(affine_holder[temp_master[i][2]]);
//                 }

//                 neighbors.push_back(temp1);
//                 mid_points.push_back(temp123);
//                 affine_vectors.push_back(temp234);

//                 // temp.clear();
//                 midpoint_holder.clear();
//                 affine_holder.clear();
//                 temp_master.clear();
//             }

//             else if (temp_master.size() == 1){
//                 temp1.push_back(temp_master[0][1]);
//                 neighbors.push_back(temp1);
//                 affine_vectors.push_back(affine_holder);
//                 mid_points.push_back(midpoint_holder);

//                 // temp.clear();
//                 temp_master.clear();
//                 midpoint_holder.clear();
//                 affine_holder.clear();
//             }
//     }
// }


template <typename TD, typename TI>
void find_neighbors(vector<vector <TD> > &centroids, 
vector<vector <TD> > &center_dist_mat, vector<vector <TD> > &cluster_size, 
vector<TI> &neighbors, vector<vector<TI> > &neighbor_indices, vector<vector<vector <TD> > > &mid_points, 
vector<vector<vector <TD> > > &affine_vectors){

    TD dist = 0;
    TD radius = 0;
    algorithm_utils alg_utils;

    vector<TI> temp(2);
    vector<TD> temp_midpoint(centroids[0].size());
    vector<TD> temp_affine(centroids[0].size());

    vector<vector<TD> > midpoint_holder;
    vector<vector<TD> > affine_holder;

    neighbors.clear();
    neighbor_indices.clear();
    mid_points.clear();
    affine_vectors.clear();
    
    int ot_center = 0;
    int curr_center = 0;
    int cnt = 0;

    // cout << "Neighbor check " << "\n";

    // Calculate inter-centroid distances
    for(curr_center=0; curr_center<centroids.size(); curr_center++){
        
        radius = cluster_size[curr_center][1];
        vector<TI> temp1;
        cnt = 0;

        // cout << "curr center: " << curr_center << "\n";
        
        for (ot_center=0; ot_center<centroids.size(); 
        ot_center++){    
            
            // Do only k calculation :) save some 
            if (center_dist_mat[curr_center][ot_center] == 0){
                dist = alg_utils.calc_euclidean(centroids[curr_center], centroids[ot_center]);
                center_dist_mat[curr_center][ot_center] = dist/2;
                center_dist_mat[ot_center][curr_center] = center_dist_mat[curr_center][ot_center];
            }

            // Start neighbor finding
            if ((curr_center != ot_center) && 
            (center_dist_mat[curr_center][ot_center] < radius)){
           
                // Get the mid-point coordinates for this pair of centroids
                find_midpoints(centroids[curr_center], centroids[ot_center], 
                                    temp_midpoint, temp_affine);
                
                neighbors.push_back(ot_center);
                midpoint_holder.push_back(temp_midpoint);
                affine_holder.push_back(temp_affine);

                cnt++;
                // cout << "other center: " << ot_center << "\n";
            }  
        }       

            if (cnt > 0){

                if (curr_center == 0){
                    temp[0] = curr_center; 
                }
                else{
                    temp[0] = neighbor_indices[curr_center-1][1];   
                }

                temp[1] = cnt + temp[0];
                cluster_size[curr_center][2] = cnt;
                neighbor_indices.push_back(temp);

                mid_points.push_back(midpoint_holder);
                affine_vectors.push_back(affine_holder);

                midpoint_holder.clear();
                affine_holder.clear();
            }
            
            else{
                
                cout << "\n\n\nNo neighbors for: " << curr_center << "\n\n\n" << endl;

                temp[0] = 0;
                temp[1] = 0;
                cluster_size[curr_center][2] = 0;
                neighbor_indices.push_back(temp);

                mid_points.push_back(midpoint_holder);
                affine_vectors.push_back(affine_holder);

                // midpoint_holder.clear();
                // affine_holder.clear();

            }

            // print_2d_vector(neighbor_indices, neighbor_indices.size(), "NIndices");
    }

}




// template <typename TD, typename TI>
// void determine_data_expression(vector<vector<TD> > &dataset, 
// vector<vector <TD> > &centroids,
// vector<vector <TD> > &dist_mat, vector<vector<TD> > &cluster_size, 
// vector<TI> &assigned_clusters, 
// vector<vector<TI> > &neighbors,
// vector<vector<vector <TD> > > &affine_vectors, 
// vector<vector<vector <TD> > > &mid_points, 
// vector<vector <TI> > &he_data, int &inner_loop_time, int &affine_time){
    
//     TI my_cluster = 0;
//     vector<TI> temp;
//     algorithm_utils alg_utils;
//     bool stat = false;
//     double temp_dist = 0.0;
//     int j=0;
//     int i = 0;

//     for (i = 0; i < assigned_clusters.size(); i++){

//         my_cluster = assigned_clusters[i];

//         // auto t3 = std::chrono::high_resolution_clock::now();
//         for (j=0; j<neighbors[my_cluster].size(); j++){ 
            
//             // auto t5 = std::chrono::high_resolution_clock::now();
//             if (find_context_direction(dataset[i], affine_vectors[my_cluster][j], 
//             mid_points[my_cluster][j])){

                
//                 if (temp.size() == 0){
//                     temp.push_back(i);
//                     temp.push_back(my_cluster);
//                 }
//                 temp.push_back(neighbors[my_cluster][j]);

//             }
//             // auto t6 = std::chrono::high_resolution_clock::now();
//             // affine_time = affine_time + std::chrono::duration_cast<std::chrono::microseconds>(t6 - t5).count();
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
void determine_data_expression(vector<vector<TD> > &dataset, 
vector<vector <TD> > &centroids, vector<vector<TD> > &cluster_size, 
vector<TI> &assigned_clusters, vector<TI> &neighbors, vector<vector<TI> > &neighbor_indices,
vector<vector<vector <TD> > > &affine_vectors, vector<vector<vector <TD> > > &mid_points, 
vector<vector <TI> > &he_data, int &inner_loop_time, int &affine_time){
    
    TI my_cluster = 0;
    TI num_my_neighbors = 0;
    
    TI j = 0;
    TI i = 0;
    TI start_index = 0;
    TI end_index = 0;
    TI curr_neighbor = 0;

    vector<TI> temp;
    algorithm_utils alg_utils;

    for (i = 0; i < assigned_clusters.size(); i++){

        my_cluster = assigned_clusters[i];
        num_my_neighbors = cluster_size[my_cluster][2];
        // start_index = neighbor_indices[my_cluster][0];
        end_index = neighbor_indices[my_cluster][1];
        j = 0;

        // cout << "Point: " << i  << " Cluster: " << my_cluster << " Num neighboirs: " << num_my_neighbors << endl;

        if (num_my_neighbors > 0){

            // auto t3 = std::chrono::high_resolution_clock::now();
            for (start_index = neighbor_indices[my_cluster][0]; start_index<end_index; start_index++){ 

                curr_neighbor = neighbors[start_index];

                // cout << "curr neighbor: " << curr_neighbor << endl;

                // print_2d_vector(affine_vectors[my_cluster], affine_vectors[my_cluster].size(), "Affine");
                // print_2d_vector(mid_points[my_cluster], mid_points[my_cluster].size(), "Midpoint");
                
                // auto t5 = std::chrono::high_resolution_clock::now();
                if (find_context_direction(dataset[i], affine_vectors[my_cluster][j], 
                mid_points[my_cluster][j])){

                    if (temp.size() == 0){
                        temp.push_back(i);
                        temp.push_back(my_cluster);
                    }
                    temp.push_back(curr_neighbor); 
                }
                // auto t6 = std::chrono::high_resolution_clock::now();
                // affine_time = affine_time + std::chrono::duration_cast<std::chrono::microseconds>(t6 - t5).count();
                // cout << "Check 3" << endl;
                j++;
            }
            // auto t4 = std::chrono::high_resolution_clock::now();
            // inner_loop_time = inner_loop_time + std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();

            // cout << "Check 3" << endl;
        }

        // cout << "Check 4" << endl;
        if (temp.size()>0){
            he_data.push_back(temp);
            temp.clear();
        }
        
    }      
}


// template <typename TDouble, typename Tint>
// void calculate_HE_distances(const vector<vector<TDouble> > &dataset, 
// vector<vector<TDouble> > &centroids, vector<vector<TDouble> > &dist_mat,
// Tint num_clusters, vector<Tint> &assigned_clusters, 
// vector<vector<TDouble> > &cluster_size, vector<vector<Tint> > &neighbors, 
// vector<vector <Tint> > &he_data){

//     TDouble temp = 0.0;
//     Tint refer_size = 0;
//     algorithm_utils alg_utils;
//     int j = 1;

//     for (int i=0; i < he_data.size(); i++){

//         refer_size = he_data[i].size();

//         for (j = 1; j<refer_size; j++){
            
//             temp = alg_utils.calc_euclidean(dataset[he_data[i][0]], centroids[he_data[i][j]]);
//             dist_mat[he_data[i][0]][he_data[i][j]] = temp;

//             if(temp > cluster_size[he_data[i][j]][1])
//                     cluster_size[he_data[i][j]][1] = temp; 

//             if(temp < dist_mat[he_data[i][0]][assigned_clusters[he_data[i][0]]]){
//                 cluster_size[assigned_clusters[he_data[i][0]]][0] = cluster_size[assigned_clusters[he_data[i][0]]][0] - 1;
                
//                 //cluster_size[assigned_clusters[he_data[i][0]]][1] = 0.0;

//                 assigned_clusters[he_data[i][0]] = he_data[i][j];
//                 cluster_size[he_data[i][j]][0] = cluster_size[he_data[i][j]][0] + 1; 
//             } 
//         }
//     }

// }


template <typename TDouble, typename Tint>
void calculate_HE_distances(const vector<vector<TDouble> > &dataset, 
vector<vector<TDouble> > &centroids, vector<vector<TDouble> > &dist_mat,
Tint num_clusters, vector<Tint> &assigned_clusters, 
vector<vector<TDouble> > &cluster_size, 
vector<vector <Tint> > &he_data){

    TDouble temp = 0.0;
    Tint refer_size = 0;
    algorithm_utils alg_utils;
    int j = 1;

    for (int i=0; i < he_data.size(); i++){

        refer_size = he_data[i].size();

        for (j = 1; j<refer_size; j++){
            
            temp = alg_utils.calc_euclidean(dataset[he_data[i][0]], centroids[he_data[i][j]]);
            dist_mat[he_data[i][0]][he_data[i][j]] = temp;

            if(temp > cluster_size[he_data[i][j]][1])
                    cluster_size[he_data[i][j]][1] = temp; 

            if(temp < dist_mat[he_data[i][0]][assigned_clusters[he_data[i][0]]]){
                cluster_size[assigned_clusters[he_data[i][0]]][0] = cluster_size[assigned_clusters[he_data[i][0]]][0] - 1;
                
                //cluster_size[assigned_clusters[he_data[i][0]]][1] = 0.0;

                assigned_clusters[he_data[i][0]] = he_data[i][j];
                cluster_size[he_data[i][j]][0] = cluster_size[he_data[i][j]][0] + 1; 
            } 
        }
    }

}
            
