#include <iostream>
#include <vector>
#include "misc_utils.hpp"
#include "algo_utils.hpp"
#include <chrono>

using namespace std;

class conv_kmeans{
    template <typename Tfloat, typename Tint>
    output_data kmeans(vector<vector <Tfloat> > &dataset, vector<vector<Tfloat> > &centroids,
    Tint num_clusters, Tfloat threshold, Tint num_iterations, Tint numCols);

};

template <typename Tfloat, typename Tint>
inline output_data kmeans(vector<vector <Tfloat> > &dataset, vector<vector<Tfloat> > &centroids,
Tint num_clusters, Tfloat threshold, Tint num_iterations, Tint numCols){

    Tint loop_counter = 0;
    // vector<vector<Tfloat> > centroids(num_clusters, vector<Tfloat>(numCols));
    vector<vector<Tfloat> > new_centroids(num_clusters, vector<Tfloat>(numCols));
    vector<vector <Tfloat> > dist_matrix(dataset.size(), vector<Tfloat>(num_clusters));
    vector<vector<Tfloat> > cluster_size(num_clusters, vector<Tfloat>(2));  

    vector<Tint> assigned_clusters(dataset.size());
 
    // Create objects
    algorithm_utils alg_utils;
    print_utils pu;

    int i =0, j =0, he_counter = 0;
    float temp_diff =0, diff = 0;
    
    // Initialize centroids
    // alg_utils.init_centroids(centroids, dataset, num_clusters);

    alg_utils.calculate_distances(dataset, centroids, dist_matrix, 
    num_clusters, assigned_clusters, cluster_size, he_counter);

   
    while (loop_counter < num_iterations){

        loop_counter++;

        // Calculate new centroids
        alg_utils.update_centroids(dataset, new_centroids, assigned_clusters, 
        cluster_size, numCols);
        

        // Check Convergence
        if (alg_utils.check_convergence(new_centroids, centroids, threshold, diff, temp_diff, i, j)){
                cout << "Convergence at iteration: " << loop_counter << "\n";
                break;
        }

        // Re-calculate distances
        alg_utils.calculate_distances(dataset, new_centroids, dist_matrix,
                                    num_clusters, assigned_clusters, cluster_size, he_counter);

        // Move the new centroids to older
        centroids = new_centroids;
        
        // reset centroids
        alg_utils.reinit(new_centroids);
        
    }

    // cout << "Total time for centroid updation: " << cent_int << " milliseconds. \n";
    // cout << "Total time for distance calc: " << dist_int << " milliseconds. \n";

    output_data result;

    result.loop_counter = loop_counter;
    result.num_he = he_counter;
    result.assigned_labels = assigned_clusters;

    return result;

}
