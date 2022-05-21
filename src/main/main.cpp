#include <iostream>
#include <string>
#include <tuple>
#include "algo_utils.hpp"
#include "IOutils.hpp"
#include "kmeans.hpp"
#include "dckmeans.hpp"
#include <chrono>

using namespace std;

string basePath = "/Users/schmuck/Documents/Box Sync/Ph.D./DATASETS/";

int main(){

    // Experiment specific path
    // string filePath = basePath + "clustering_data";
    // string filePath = basePath + "dims_data";
    // string filePath = basePath + "scal_data";
    // string filePath = basePath + "sample_data/";
    string filePath = basePath + "real_data/";
    
    // string fileName = "test_500_2_10.csv";
    string fileName = "spambase.csv";

    string somefilePath = filePath + fileName;
    cout << somefilePath << "\n" ;

    std::vector<vector <double> > dataset;
    vector<int> labels;
    
    // Declare variables
    int num_iterations = 100;
    double threshold = 0.001;
    int num_clusters = 10;

    dataIO dataOps;
    algorithm_utils alg_utils; 
    print_utils print_utl;
    DataCentricKmeans dckm;

    // Read in the data
    auto t1 = std::chrono::high_resolution_clock::now();
    std::pair<int, int> p = readSimulatedData(somefilePath, dataset, labels);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    cout << "File reading: " << ms_int.count() << "\n";

    int numRows = p.first-1;
    int numCols = p.second-1;

    vector<vector<double> > km_centers(num_clusters, vector<double>(numCols, 0.0));
    vector<int> km_assign(dataset.size(), 0.0);
    int km_iter = 0;
    
    // Debug - Testing
    // print_2d_vector(dataset, 5, "Dataset");
    // print_vector(labels, 5, "Labels");

    // auto t1 = std::chrono::high_resolution_clock::now();
    // cout << "Test1" << "\n";
    // km_iter = kmeans(dataset, num_clusters, threshold, num_iterations, 
    // numRows, numCols, km_centers, km_assign);
    // auto t2 = std::chrono::high_resolution_clock::now();
    // auto ms_int = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1);
    // std::cout << ms_int.count() << "seconds\n";

    // cout << "Test1" << "\n";
    auto t3 = std::chrono::high_resolution_clock::now();
    km_iter = dckmeans(dataset, num_clusters, threshold, num_iterations, numCols);
    auto t4 = std::chrono::high_resolution_clock::now();
    auto ms_int2 = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3);
    std::cout << "Total DCKmeans time: " << ms_int2.count() << "milliseconds\n";

    // print_2d_vector(km_centers, num_clusters, "Final Centroids");

return 0;
}