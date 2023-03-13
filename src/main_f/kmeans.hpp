#include <iostream>
#include <vector>
#include "misc_utils.hpp"
#include "algo_utils.hpp"
#include <chrono>

using namespace std;

class conv_kmeans{
    template <typename Tfloat, typename Tint>
    output_data kmeans(vector<vector <Tfloat> > &dataset, vector<vector<Tfloat> > &centroids,
    Tint num_clusters, Tfloat threshold, Tint num_iterations, Tint numCols, Tint time_limit);

};


template <typename Tfloat, typename Tint>
inline output_data kmeans(vector<vector <Tfloat> > &dataset, vector<vector<Tfloat> > &centroids,
Tint num_clusters, Tfloat threshold, Tint num_iterations, Tint numCols, Tint time_limit){

    Tint loop_counter = 0;
    vector<vector<Tfloat> > new_centroids(num_clusters, vector<Tfloat>(numCols));
    vector<vector <Tfloat> > dist_matrix(dataset.size(), vector<Tfloat>(num_clusters));
    vector<vector<Tfloat> > cluster_size(num_clusters, vector<Tfloat>(2));  

    vector<Tint> assigned_clusters(dataset.size());
 
    // Create objects
    algorithm_utils alg_utils;
    print_utils pu;

    int i =0, j =0 ;
    float temp_diff =0, diff = 0;
    unsigned long long int he_counter = 0;


    output_data result;

    // Start time counter 
    auto start = std::chrono::high_resolution_clock::now();
    
    // Initialize centroids
    alg_utils.init_centroids_sequentially(centroids, dataset, num_clusters);

    alg_utils.calculate_distances(dataset, centroids, dist_matrix, 
    num_clusters, assigned_clusters, cluster_size, he_counter);

        // Check for empty clusters and return
//    for (i=0; i<num_clusters; i++){
        
//         if(cluster_size[i][0] == 0){
//             cout << "Empty cluster found after initialization, safe exiting" << endl;
//             result.loop_counter = 1;
//             result.num_he = 0;
//             result.runtime = 0;
//             result.timeout = false;
//             return result;
//         }
//     }

   
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

            // Check for empty clusters and return
        //   for (i=0; i<num_clusters; i++){
        
        //     if(cluster_size[i][0] == 0){
        //         cout << "Empty cluster found during iterations, safe exiting" << endl;
        //         result.loop_counter = 1;
        //         result.num_he = 0;
        //         result.runtime = 0;
        //         result.timeout = false;
        //         return result;
        //     }
        // }

        auto temp_end = std::chrono::high_resolution_clock::now();
        auto temptime = std::chrono::duration_cast<std::chrono::milliseconds>(temp_end - start);

        if (temptime.count() >= time_limit){
            result.loop_counter = loop_counter;
            result.num_he = he_counter;
            result.assigned_labels = assigned_clusters;
            result.runtime = float(temptime.count());
            result.timeout = true;
            cout << "Kmeans Timed Out :(" << endl;
            return result;
        }
        
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto Totaltime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    result.loop_counter = loop_counter;
    result.num_he = he_counter;
    result.assigned_labels = assigned_clusters;
    result.runtime = float(Totaltime.count());
    result.timeout = false;

    return result;

}
