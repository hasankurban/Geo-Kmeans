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
    Tint num_clusters, Tdouble threshold, Tint num_iterations, Tint numCols);
};


template <typename Tdouble, typename Tint>
Tint dckmeans(vector<vector <Tdouble> > &dataset, Tint num_clusters, 
Tdouble threshold, Tint num_iterations, Tint numCols){

    Tint loop_counter = 0;
    vector<vector<Tdouble> > centroids(num_clusters, vector<Tdouble>(numCols));
    vector<vector<Tdouble> > new_centroids(num_clusters, vector<Tdouble>(numCols));
    vector<vector <Tdouble> > dist_matrix(dataset.size(), vector<Tdouble>(num_clusters));
    
    vector<Tint> assigned_clusters(dataset.size());
    
    vector<vector<Tdouble> > cluster_size(num_clusters, vector<Tdouble>(2));  
    vector<vector <Tdouble> > center_dist_mat (num_clusters, vector<Tdouble>(num_clusters));
    vector<vector<Tint> > neighbors(num_clusters);

    
    vector<vector<Tint> > he_data;

    // vector<vector<vector <Tdouble> > > mid_points(num_clusters);
    // vector<vector<vector<Tdouble> > > affine_vectors(num_clusters);

    map<string, vector<Tdouble> > mid_points;
    map<string, vector<Tdouble> > affine_vectors;

    map<Tint, vector<Tint> > assign_dict;

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
    auto t1 = std::chrono::high_resolution_clock::now();

    alg_utils.calculate_distances(dataset, centroids, dist_matrix,
    num_clusters, assigned_clusters, cluster_size);

    auto t2 = std::chrono::high_resolution_clock::now();
    auto temp1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    cout << "Time for first center-point distance calc: " << temp1.count() << "\n";

    // Print dist_matrix
    // print_2d_vector(dist_matrix, 5, "Distance matrix");
    // print_vector(assigned_clusters, 5, "Assigned clusters");
    // print_2d_vector(cluster_size, 5, "Cluster Size and Radious");

    auto cent_time = 0;
    auto ne_time = 0;
    auto he_time = 0;
    auto dist_time = 0;
    auto misc_time = 0;

    while (loop_counter < num_iterations){

        loop_counter++;
        
        // Calculate new centroids
        auto t3 = std::chrono::high_resolution_clock::now();
        
        alg_utils.update_centroids(dataset, new_centroids, assigned_clusters, 
        cluster_size, numCols);
        
        auto t4 = std::chrono::high_resolution_clock::now();
        cent_time = cent_time + std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count();
        
        // Check Convergence
        if (alg_utils.check_convergence(new_centroids, centroids, threshold)){
                cout << "Convergence at iteration: " << loop_counter << "\n";
                break;
        }

        
        // get_assign_dict(assign_dict, assigned_clusters);
        // print_map(assign_dict, assign_dict.size(), "Initial Assignments");

        auto t5 = std::chrono::high_resolution_clock::now();
        
        find_neighbors(new_centroids, center_dist_mat, cluster_size, 
        neighbors, mid_points, affine_vectors);
        
        auto t6 = std::chrono::high_resolution_clock::now();
        ne_time = ne_time + std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count();

        // print_2d_vector(neighbors, neighbors.size(), "Neighbors: ");
        

        auto t7 = std::chrono::high_resolution_clock::now();
        
        determine_data_expression(dataset, new_centroids, assigned_clusters, 
        neighbors, affine_vectors, mid_points, he_data);
        
        auto t8 = std::chrono::high_resolution_clock::now();
        he_time = he_time + std::chrono::duration_cast<std::chrono::milliseconds>(t8 - t7).count();

        // cout << "No. HE Data: " << he_data.size() << "\n";
        // print_2d_vector(he_data, he_data.size(), "HE Data");
        
        // Re-calculate distances
        auto t9 = std::chrono::high_resolution_clock::now();
        
        calculate_HE_distances(dataset, new_centroids, dist_matrix,
                                        num_clusters, assigned_clusters, 
                                        cluster_size, assign_dict, neighbors, he_data);
        
        auto t10 = std::chrono::high_resolution_clock::now();
        dist_time = dist_time + std::chrono::duration_cast<std::chrono::milliseconds>(t10 - t9).count();

        he_data.clear();

        // print_2d_vector(neighbors, neighbors.size(), "Neighbors");

        // Print for testing
        // print_2d_vector(cluster_size, cluster_size.size(), "After HE calc: Cluster Sizes");
        // print_2d_vector(new_centroids, centroids.size(), "Updated Centroids");
        // print_2d_vector(dist_matrix, 5, "After: Distance matrix");
        // print_vector(assigned_clusters, 5, "Assigned clusters");

        // Move the new centroids to older
        centroids = new_centroids;

        auto t11 = std::chrono::high_resolution_clock::now();
        // Register the max radius
        restore_radius(dist_matrix, assigned_clusters, cluster_size);

        // reset centroids
        alg_utils.reinit(new_centroids);
        auto t12 = std::chrono::high_resolution_clock::now();
        misc_time = misc_time + std::chrono::duration_cast<std::chrono::milliseconds>(t12 - t11).count();
    }

    cout << "Total time for centroid updation calc: " << cent_time << " milliseconds. \n";
    cout << "Total time for neighbors calc: " << ne_time << " milliseconds. \n";
    cout << "Total time for HE calc: " << he_time << " milliseconds. \n";
    cout << "Total time for distance calc: " << dist_time << " milliseconds. \n";
    cout << "Total time for misc calc: " << misc_time << " milliseconds. \n";

    // print_2d_vector(new_centroids, new_centroids.size(), "Final centroids");

    return loop_counter;
    // return std::make_tuple(new_centroids, assigned_clusters, loop_counter);
}