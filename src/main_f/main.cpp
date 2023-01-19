#include <iostream>
#include <string>
#include <tuple>
#include "algo_utils.hpp"
#include "IOutils.hpp"
#include "kmeans.hpp"
#include "dckmeans.hpp"
#include <chrono>

using namespace std;

//  string basePath = "/nobackup/parishar/DATASETS/";
string basePath = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/";
// string basePath = "/Users/schmuck/Documents/OneDrive - Indiana University/Box Sync/PhD/Data-Centric-KMeans/";

int main(){

    // Experiment specific path
    string filePath = basePath + "clustering_data/";
    // string filePath = basePath + "dims_data/";
    // string filePath = basePath + "scal_data/";
//    string filePath = basePath + "sample_data/";
    //  string filePath = basePath + "real_data/";
    
//    string fileName = "50_2_10.csv";
    string fileName = "10_clusters.csv";

    string somefilePath = filePath + fileName;
    cout << somefilePath << "\n" ;

    std::vector<vector <float> > dataset;
    vector<int> labels;

    // cout << dataset.max_size() << endl;
    
    // Declare variables
    int num_iterations = 100;
    float threshold = 0.01;
    int num_clusters = 10;

    // Read in the data
    auto t1 = std::chrono::high_resolution_clock::now();
    
    std::pair<int, int> p = readSimulatedData(somefilePath, dataset, labels, true, true);

    cout << "Data size: " << dataset.size() << " X " << dataset[0].size() << endl;
    
    auto t2 = std::chrono::high_resolution_clock::now();
    auto file_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    cout << "File reading: " << file_int.count() << " MS\n";
    
    int numRows = p.first;
    int numCols = p.second;

    vector<vector<float> > km_centers(num_clusters, vector<float>(numCols, 0.0));
    vector<int> km_assign(dataset.size());
    int km_iter = 0;
    
    // Debug - Testing
    cout << "\nAlgo: DCKM," << " Clusters: " << num_clusters << ", Threshold: " << threshold << endl;
    auto t5 = std::chrono::high_resolution_clock::now();
    km_iter = dckmeans(dataset, num_clusters, threshold, num_iterations, numCols);
    auto t6 = std::chrono::high_resolution_clock::now();
    auto ms_int2 = std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5);
    std::cout << "Total DCKmeans time: " << ms_int2.count() << " milliseconds\n";

    // cout << "\nAlgo: KMeans," << " Clusters: " << num_clusters << ", Threshold: " << threshold << endl;
    // auto t3 = std::chrono::high_resolution_clock::now();
    // km_iter = kmeans(dataset, num_clusters, threshold, num_iterations, 
    // numRows, numCols, km_centers, km_assign);
    // auto t4 = std::chrono::high_resolution_clock::now();
    // auto km_int = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3);
    // std::cout << "Total KMeans time: " << km_int.count() << " milliseconds\n";


    // for(int i=2; i<=20; i++){

    //     num_clusters = i;
    //     vector<vector<float> > km_centers(num_clusters, vector<float>(numCols, 0.0));
    //     vector<int> km_assign(dataset.size());

    //     cout << "\nAlgo: DCKM," << " Clusters: " << num_clusters << ", Threshold: " << threshold << endl;
    //     auto t5 = std::chrono::high_resolution_clock::now();
    //     km_iter = dckmeans(dataset, num_clusters, threshold, num_iterations, numCols);
    //     auto t6 = std::chrono::high_resolution_clock::now();
    //     auto ms_int2 = std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5);
    //     std::cout << "Total DCKmeans time: " << ms_int2.count() << " milliseconds\n";

    //     cout << "\nAlgo: KMeans," << " Clusters: " << num_clusters << ", Threshold: " << threshold << endl;
    //     auto t3 = std::chrono::high_resolution_clock::now();
    //     km_iter = kmeans(dataset, num_clusters, threshold, num_iterations, 
    //     numRows, numCols, km_centers, km_assign);
    //     auto t4 = std::chrono::high_resolution_clock::now();
    //     auto km_int = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3);
    //     std::cout << "Total KMeans time: " << km_int.count() << " milliseconds\n";

    // }

return 0;
}