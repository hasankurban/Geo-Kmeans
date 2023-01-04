#include <iostream>
#include <vector>
#include "math.h"
#include <map>
#include <algorithm>
#pragma once

using namespace std;

class algorithm_utils{
    public:
    
    template <typename T1, typename T2>
    void init_centroids(vector<vector<T1> > &, vector<vector<T1> > &, T2);
    
    template <typename T1, typename T2>
    void calculate_distances(const vector<vector<T1> > &dataset, 
    vector<vector<T1> > &centroids, vector<vector<T1> > &dist_mat,
    T2 num_clusters, vector<T2> &assigned_clusters, 
    vector<vector<T1> > &cluster_size);

    template <typename T1>
    float calc_euclidean(const vector<T1> &, const vector<T1> &);

    template <typename T1, typename T2>
    void update_centroids(vector<vector <T1> > &dataset, 
    vector<vector<T1> > &new_centroids, vector<T2> &assigned_clusters, 
    vector<vector<T1> > &cluster_size, T2 numCols);

    template <typename T1>
    bool check_convergence(vector<vector <T1> > &new_centroids, 
    vector<vector <T1> > &centroids, T1 threshold, float &diff, float &temp_diff, int &i, int &j);

    // bool check_convergence(vector<vector <T1> > &, vector<vector <T1> > &, T1);

    template <typename T1>
    void reinit(vector<vector<T1> > &);
    
};


template <typename T1>
void algorithm_utils::reinit(vector<vector<T1> > &container){

    for(int i=0;i<container.size(); i++){
        container[i].assign(container[i].size(), 0);
    }
}

template <typename T1, typename T2>
void algorithm_utils::init_centroids(vector<vector <T1> > &centroids, 
vector<vector <T1> > &dataset, T2 num_cluster){

    vector<T1> temp1 (dataset[0].size());
    // cout << "Init:" << "\n";

    for(int i=0; i<num_cluster; i++){  
        for(int j=0; j<dataset[i].size(); j++){
                temp1[j] = dataset[i][j];
        }   
        centroids[i] = temp1;
    }
}

template <typename T1>
inline float algorithm_utils::calc_euclidean(const vector<T1> &point, 
const vector<T1> &center){
    
    T1 dist = 0.0;
    T1 temp = 0.0;
    
    // cout << point.size() << "\n";

    for (int i=0; i < point.size(); i++){
        temp = point[i] - center[i];
        dist = dist + (temp*temp);
    }

    dist = sqrt(dist);
    return dist;
}


template <typename T1, typename T2>
inline void algorithm_utils::calculate_distances(const vector<vector<T1> > &dataset, 
vector<vector<T1> > &centroids, vector<vector<T1> > &dist_mat,
T2 num_clusters, vector<T2> &assigned_clusters, vector<vector<T1> > &cluster_size){

    T2 current_center = 0;
    vector<T1> temp_dist (num_clusters);
    float temp = 0.0;
    float shortestDist2 = 0.0;
    int i =0, j =0;

    assigned_clusters.assign(assigned_clusters.size(), 0);
    algorithm_utils::reinit(cluster_size);

    // Calculate the distance of points to nearest center
    for (i=0; i < dataset.size(); i++){

        // cout << "inside dist :" << "\n";
        
        shortestDist2 = std::numeric_limits<float>::max();
        // cout << i << "\n";
        
        for (j=0; j < centroids.size(); j++){ 
            
            temp = calc_euclidean(dataset[i], centroids[j]);
            temp_dist[j] = temp;

            // cout << "hello" << "n";
            
            if (temp < shortestDist2){
                shortestDist2 = temp;
                current_center = j;
            }
        }
        
        dist_mat[i] = temp_dist;
        assigned_clusters[i] = current_center;

        // Increase the size of the cluster
        cluster_size[current_center][0] = cluster_size[current_center][0] + 1;
        
        // Store the max so far
        if (shortestDist2 > cluster_size[current_center][1])
            cluster_size[current_center][1] = shortestDist2;
    }
}


template <typename T1, typename T2>
inline void algorithm_utils::update_centroids(vector<vector <T1> > &dataset, 
vector<vector<T1> > &new_centroids, vector<T2> &assigned_clusters, 
vector<vector<T1> > &cluster_size, T2 numCols){

    int point_index = 0, curr_center = 0, index = 0, k = 0, j =0;

    for (index=0; index<dataset.size(); index++){
        curr_center = assigned_clusters[index];
        
        for (j = 0; j<numCols; j++){
            new_centroids[curr_center][j] = new_centroids[curr_center][j] + dataset[index][j];
        }
    }

    for(int i=0; i<new_centroids.size();i++){
        k = cluster_size[i][0];

        for (j = 0; j < new_centroids[i].size(); j++){
            if (k > 0)
                new_centroids[i][j] = new_centroids[i][j]/k;
            else
                new_centroids[i][j] = 0.0;
        }
    } 

}



template <typename T1>
inline bool algorithm_utils::check_convergence(vector<vector <T1> > &new_centroids, 
vector<vector <T1> > &centroids, T1 threshold, float &diff, float &temp_diff, int &i, int &j){

    temp_diff = 0;
    diff = 0;

    if (new_centroids.size() == centroids.size()){
        
        for (i=0; i<new_centroids.size(); i++){
            for (j=0; j< new_centroids[i].size(); j++)
                temp_diff = new_centroids[i][j] - centroids[i][j];
                diff = diff + (temp_diff * temp_diff);
        }
        diff = sqrt(diff);
    }
    else
        return false;

    if (diff <= threshold)
        return true;
    return false;
}
