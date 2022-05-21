#include <iostream>
#include <vector>
#include "math.h"
#include <map>
#include <algorithm>
#include "algo_utils.hpp"
#include "misc_utils.hpp"
#pragma once

using namespace std;

class dckm_utils{

    template <typename TD>
    void find_midpoints(vector<TD> &center1, vector<TD> &center2, 
    vector<TD> &midpoint);

    template <typename TD>
    void find_affine_vector(vector<TD> &midpoint, vector<TD> &center, 
    vector<TD> &affine);

    template <typename TD>
    bool find_context_direction(vector<TD> &centroid_vector, vector<TD> &midpoint,
    vector<TD> &actual_point);

    template <typename TD, typename TI>
    void restore_radius(vector<vector <TD> > &dist_matrix,
    vector<TI> &assigned_clusters, 
    vector<vector <TD> > &cluster_size);

    template <typename TD, typename TI>
    void find_neighbors(vector<vector <TD> > &centroids, 
    vector<vector <TD> > &center_dist_mat, vector<vector <TD> > &cluster_size, 
    vector<vector<TI> > &neighbors, map<string, vector<TD> > &mid_points, 
    map<string, vector<TD> > &affine_vectors);
    
    template <typename TD, typename TI>
    void determine_data_expression(vector<vector<TD> > &dataset, 
    vector<TI> &assigned_clusters, vector<vector<TI> > &neighbors, 
    map<string, vector<TD> > &affine_vectors, 
    map<string, vector<TD> > &mid_points, vector<vector<TI> > &he_data);

    template <typename TD, typename TI>
    void find_HE_data(vector<vector<TD> > &dataset, vector<vector <TD> > &centroids, 
    vector<vector <TD> > &distances, vector<TI> &assigned_clusters,
    vector<vector<TD> > &cluster_size, vector<vector <TD> > &center_dist_mat, 
    map<string, vector<TD> > &mid_points, 
    map<string, vector<TD> > &affine_vectors,
    vector<vector<TI> > &neighbors, 
    vector<vector<TI> > &he_data);

    template <typename TDouble, typename Tint>
    void calculate_HE_distances(const vector<vector<TDouble> > &dataset, 
    vector<vector<TDouble> > &centroids, vector<vector<TDouble> > &dist_mat,
    Tint num_clusters, vector<Tint> &assigned_clusters, 
    vector<vector<TDouble> > &cluster_size, vector<vector<Tint> > &neighbors, 
    vector<vector <Tint> > &he_data);

};

template <typename TD>
void find_midpoints(vector<TD> &center1, vector<TD> &center2, 
vector<TD> &midpoint){

    // print_vector(center1, center1.size(), "Test1");
    // print_vector(center2, center2.size(), "Test2");

    for (int i=0; i<center1.size(); i++){
        midpoint[i] = center1[i] + center2[i];
        midpoint[i] = midpoint[i]/2;
    }

    // print_vector(midpoint, midpoint.size(), "Test3");
}

template <typename TD>
void find_affine_vector(vector<TD> &midpoint, vector<TD> &ot_point, vector<TD> &affine){
    for (int i=0; i<ot_point.size(); i++)
        affine[i] = ot_point[i] - midpoint[i];
}

template <typename TD>
bool find_context_direction(vector<TD> &centroid_vector, vector<TD> &midpoint,
vector<TD> &actual_point){

    int mysize = midpoint.size(); 
    
    vector<TD> cent_point_vec(mysize);
    TD vec_sum = 0.0;
    
    find_affine_vector(midpoint, actual_point, cent_point_vec);

    for (int i=0; i<mysize; i++)
        vec_sum =  vec_sum + (cent_point_vec[i] * centroid_vector[i]);

    if (vec_sum>=0)
        return true;

    // cout << "Inner product: " << vec_sum << "\n";
    // print_vector(cent_point_vec, cent_point_vec.size(), "point vector");
    // print_vector(centroid_vector, cent_point_vec.size(), "Centroid vector");
    
    return false;
}

template <typename TD, typename TI>
void restore_radius(vector<vector <TD> > &dist_matrix,
vector<TI> &assigned_clusters, 
vector<vector <TD> > &cluster_size){

    for (int i=0; i<cluster_size.size(); i++){
        for (int j=0; j< assigned_clusters.size();j ++){
                if ((assigned_clusters[j] == i) && (dist_matrix[j][i] > cluster_size[i][1]))
                    cluster_size[i][1] = dist_matrix[j][i];
        }
    }
}

template <typename TD, typename TI>
void find_neighbors(vector<vector <TD> > &centroids, 
vector<vector <TD> > &center_dist_mat, vector<vector <TD> > &cluster_size, 
vector<vector<TI> > &neighbors, map<string, vector<TD> > &mid_points, 
map<string, vector<TD> > &affine_vectors){

    TD dist = 0;
    TD radius = 0;

    algorithm_utils alg_utils;

    vector<TD> temp_midpoint(centroids[0].size());
    vector<TD> temp_affine(centroids[0].size());
    string key = "";

    // Calculate inter-centroid distances
    for(int curr_center=0; curr_center<centroids.size(); curr_center++){
        
        radius = cluster_size[curr_center][1];
        
        for (int ot_center=0; ot_center<centroids.size(); ot_center++){    
            
            // Do only k calculation :) save some 
            if (center_dist_mat[curr_center][ot_center] == 0){
                dist = alg_utils.calc_euclidean(centroids[curr_center], centroids[ot_center]);
                center_dist_mat[curr_center][ot_center] = (dist/2);
                center_dist_mat[ot_center][curr_center] = (dist/2);
            }

            // Start neighbor finding
            if ((center_dist_mat[curr_center][ot_center] < radius)){
                
                // The following is the neighbor for the current center
                neighbors[curr_center].push_back(ot_center); 
                    
                // Get the mid-point coordinates for this pair of centroids
                find_midpoints(centroids[curr_center], centroids[ot_center], 
                                    temp_midpoint);
                // Determine the affine vector                
                find_affine_vector(temp_midpoint, centroids[ot_center], temp_affine);
                
                // Update the primary containers
                key = std::to_string(curr_center) + std::to_string(ot_center);
                mid_points.insert_or_assign(key, temp_midpoint);
                affine_vectors.insert_or_assign(key, temp_affine);
                // mid_points[std::to_string(curr_center) + std::to_string(ot_center)] = temp_midpoint;
                // affine_vectors[std::to_string(curr_center) + std::to_string(ot_center)] = temp_affine;
            }
        }
    }
}


template <typename TD, typename TI>
void determine_data_expression(vector<vector<TD> > &dataset, 
vector<vector <TD> > &centroids, vector<TI> &assigned_clusters, 
vector<vector<TI> > &neighbors, 
map<string, vector<TD> > &affine_vectors, 
map<string, vector<TD> > &mid_points, 
vector<vector<TI> > &he_data){
    
    TI my_cluster=0;
    algorithm_utils alg_utils;

    //reinit he container
    alg_utils.reinit(he_data);
    string key = "";
    bool status = false;

    for (int i = 0; i < dataset.size(); i++){

        vector<TI> temp;
        my_cluster = assigned_clusters[i];
        status = false;

        for (int j=0; j<neighbors[my_cluster].size(); j++){
            
            key = std::to_string(my_cluster) + std::to_string(neighbors[my_cluster][j]);
            
            if ( (my_cluster != neighbors[my_cluster][j]) && find_context_direction(affine_vectors[key], 
            mid_points[key], dataset[i])){
                
                // cout << "Same direction" << "\n" ;
                if (temp.size() == 0)
                    temp.push_back(i);
                    temp.push_back(my_cluster);
                
                temp.push_back(neighbors[my_cluster][j]);
                status = true;
            }
        }
        if (status){
            he_data.push_back(temp);
        }
    }
}


template <typename TD, typename TI>
void find_HE_data(vector<vector<TD> > &dataset, vector<vector <TD> > &centroids, 
vector<vector <TD> > &distances, vector<TI> &assigned_clusters,
vector<vector<TD> > &cluster_size, vector<vector <TD> > &center_dist_mat, 
map<string, vector<TD> > &mid_points, 
map<string, vector<TD> > &affine_vectors,
vector<vector<TI> > &neighbors, 
vector<vector<TI> > &he_data){

        // Find neighbors
        find_neighbors(centroids, center_dist_mat, cluster_size, 
        neighbors, mid_points, affine_vectors);

        // Calculate data expression
        determine_data_expression(dataset, centroids, assigned_clusters, 
        neighbors, affine_vectors, mid_points, he_data);

        // cout << "Number of HE points: " << he_data.size() << "\n";
}


template <typename TDouble, typename Tint>
void calculate_HE_distances(const vector<vector<TDouble> > &dataset, 
vector<vector<TDouble> > &centroids, vector<vector<TDouble> > &dist_mat,
Tint num_clusters, vector<Tint> &assigned_clusters, 
vector<vector<TDouble> > &cluster_size, vector<vector<Tint> > &neighbors, 
vector<vector <Tint> > &he_data){

    Tint current_center = 0;
    vector<TDouble> temp_dist (num_clusters);
    TDouble temp = 0.0;

    // vector<Tint> * refer;
    Tint refer_size = 0;
    // algorithm_utils alg_utils;

    // print_2d_vector(neighbors, neighbors.size(), "Neighbors");

    for (int i=0; i < he_data.size(); i++){

        // refer = &he_data[i];
        refer_size = he_data[i].size();

        for (int j = 1; j<refer_size; j++){
            temp = alg_utils.calc_euclidean(dataset[he_data[i][0]], centroids[he_data[i][j]]);
            dist_mat[he_data[i][0]][he_data[i][j]] = temp;

            // cout << "Point: " << he_data[i][0] << " previous center" << assigned_clusters[he_data[i][0]] << " curr calcu: " << he_data[i][j] << "\n";
            // cout << "previous dist: " << dist_mat[he_data[i][0]][assigned_clusters[he_data[i][0]]] << 
            // "curr calc: " << temp << "\n";
            
            // cout << he_data[i][0] << "<-->" << he_data[i][j] << ": " << temp << "\n";    

            if(temp < dist_mat[he_data[i][0]][assigned_clusters[he_data[i][0]]]){
            // if(temp < dist_mat[he_data[i][0]][he_data[i][j]]){
                dist_mat[he_data[i][0]][he_data[i][j]] = temp;
                cluster_size[assigned_clusters[he_data[i][0]]][0] = cluster_size[assigned_clusters[he_data[i][0]]][0] - 1;
                
                // cout << "Point: " << he_data[i][0] << "\t" << assigned_clusters[he_data[i][0]] << "-->" << he_data[i][j] << "\n";
                
                cluster_size[assigned_clusters[he_data[i][0]]][1] = 0.0;
                assigned_clusters[he_data[i][0]] = he_data[i][j];
                cluster_size[he_data[i][j]][0] = cluster_size[he_data[i][j]][0] + 1; 
            } 
        }
    }

}

