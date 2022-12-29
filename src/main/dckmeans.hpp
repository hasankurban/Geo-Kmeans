#include <iostream>
#include <vector>
#include <map>
#include "misc_utils.hpp"
#include "algo_utils.hpp"
#include "dckm_utils.hpp"
#include <chrono>

using namespace std;

class DataCentricKmeans{
    // T1 would be double or float
    // T2 would be an integer
    template <typename Tdouble, typename Tint>
    Tint dckmeans(vector<vector <Tdouble> > &dataset, 
    Tint num_clusters, Tdouble threshold, Tint num_iterations, 
    Tint numCols);
};


template <typename Tdouble, typename Tint>
Tint dckmeans(vector<vector <Tdouble> > &dataset, Tint num_clusters, 
Tdouble threshold, Tint num_iterations, Tint numCols){

    Tint loop_counter = 0;
    vector<vector<Tdouble> > centroids(num_clusters, vector<Tdouble>(numCols));
    vector<vector<Tdouble> > new_centroids(num_clusters, vector<Tdouble>(numCols));
    vector<vector<Tdouble> > dist_matrix(dataset.size(), vector<Tdouble>(num_clusters));
    vector<Tint> assigned_clusters(dataset.size());
    
    vector<vector<Tdouble> > cluster_size(num_clusters, vector<Tdouble>(2));  
    vector<vector <Tdouble> > center_dist_mat (num_clusters, vector<Tdouble>(num_clusters));
    vector<vector<Tint> > neighbors;

    vector<vector<vector <Tdouble> > > mid_points;
    vector<vector<vector <Tdouble> > > affine_vectors;
    map<Tint, vector<Tint> > assign_dict;
    vector<vector <Tint> > he_data;

    // Create objects
    algorithm_utils alg_utils;
    dckm_utils dc_utils;

    // auto t1 = std::chrono::high_resolution_clock::now();
    
    // Initialize centroids
    alg_utils.init_centroids(centroids, dataset, num_clusters);

    // auto t2 = std::chrono::high_resolution_clock::now();
    // auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    // cout << "Time for centroid init: " << ms.count() << "\n";

    // Print centroids
    // print_2d_vector(centroids, centroids.size(), "Initial Centroids");

    // Assign data to nearest center
    alg_utils.calculate_distances(dataset, centroids, dist_matrix,
    num_clusters, assigned_clusters, cluster_size);

    // Print dist_matrix
    // print_2d_vector(dist_matrix, 5, "Distance matrix");
    // print_vector(assigned_clusters, 5, "Assigned clusters");
    // print_2d_vector(cluster_size, 5, "Cluster Size and Radious");

    auto cent_time = 0;
    auto ne_time = 0;
    auto he_time = 0;
    auto dist_time = 0;
    auto misc_time = 0;
    auto inner_loop_time = 0;
    auto affine_calc_time = 0;
    auto rand_time = 0;
    int my_cluster = 0;


    while (loop_counter < num_iterations){

        loop_counter++;

        // cout << "Iter: " << loop_counter <<"\n";
        
        // Calculate new centroids
        // auto t3 = std::chrono::high_resolution_clock::now();
        
        alg_utils.update_centroids(dataset, new_centroids, assigned_clusters, cluster_size, numCols);
        
        // auto t4 = std::chrono::high_resolution_clock::now();
        // cent_time = cent_time + std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count();
        
        // print_2d_vector(new_centroids, new_centroids.size(), "Updated Centroids");

        // Check Convergence
        if (alg_utils.check_convergence(new_centroids, centroids, threshold)){
                cout << "Convergence at iteration: " << loop_counter << "\n";
                break;
        }

        // auto t5 = std::chrono::high_resolution_clock::now();
        
        find_neighbors(new_centroids, center_dist_mat, cluster_size, neighbors, mid_points, affine_vectors);
        
        // auto t6 = std::chrono::high_resolution_clock::now();
        // ne_time = ne_time + std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count();
        // print_2d_vector(neighbors, neighbors.size(), "Neighbors: ");
        
        // auto t7 = std::chrono::high_resolution_clock::now();
        
        determine_data_expression(dataset, new_centroids, dist_matrix, cluster_size,
        assigned_clusters, neighbors, affine_vectors, mid_points, 
        he_data, inner_loop_time, affine_calc_time);

        // auto t8 = std::chrono::high_resolution_clock::now();
        // he_time = he_time + std::chrono::duration_cast<std::chrono::milliseconds>(t8 - t7).count();

        // cout << "No. HE Data: " << he_data.size() << "\n";
        // print_2d_vector(he_data, he_data.size(), "HE Data");
        
        // Re-calculate distances
        // auto t9 = std::chrono::high_resolution_clock::now();
        calculate_HE_distances(dataset, new_centroids, dist_matrix,
                                        num_clusters, assigned_clusters, 
                                        cluster_size, assign_dict, neighbors, he_data);
        // auto t10 = std::chrono::high_resolution_clock::now();
        // dist_time = dist_time + std::chrono::duration_cast<std::chrono::milliseconds>(t10 - t9).count();


        // auto t11 = std::chrono::high_resolution_clock::now();
        // Move the new centroids to older
        centroids = new_centroids;
        he_data.clear();
        
        // reset centroids
        alg_utils.reinit(new_centroids);
        // auto t12 = std::chrono::high_resolution_clock::now();
        // misc_time = misc_time + std::chrono::duration_cast<std::chrono::milliseconds>(t12 - t11).count();
    }

    // int some_r = 0;
    // int some_s = 0;
    // int some_p = 0;
    // auto t11 = std::chrono::high_resolution_clock::now();
    // while (some_r < 10){
    // for (some_p = 0; some_p < assigned_clusters.size(); some_p++){
    //     for (some_s=0; some_s<neighbors[assigned_clusters[some_p]].size(); some_s++){
    //     }
    // }
    // some_r++;
    // }
    // auto t12 = std::chrono::high_resolution_clock::now();
    // rand_time = rand_time + std::chrono::duration_cast<std::chrono::milliseconds>(t12 - t11).count();

    // cout << "Total time for random calc: " << rand_time << " milliseconds. \n";

    // cout << "Total time for centroid updation calc: " << cent_time << " milliseconds. \n";
    // cout << "Total time for distance calc: " << dist_time << " milliseconds. \n";
    // cout << "Total time for neighbors calc: " << ne_time << " milliseconds. \n";
    // cout << "Affine calc time: " << affine_calc_time << "\n";
    // cout << "Inner loop: " << inner_loop_time << "\n";
    // cout << "Total time for HE calc: " << he_time << " milliseconds. \n";
    
    return loop_counter;
}
