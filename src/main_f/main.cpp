#include <iostream>
#include <string>
#include <tuple>
#include "data_holder.hpp"
#include "algo_utils.hpp"
#include "IOutils.hpp"
#include "kmeans.hpp"
#include "dckmeans.hpp"
#include <chrono>
// #include "mlpack.hpp"

#include <filesystem>

using namespace std;
// using namespace mlpack;
// namespace fs = std::filesystem;

//  string basePath = "/nobackup/parishar/DATASETS/";
string basePath = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/";
// string basePath = "/Users/schmuck/Documents/OneDrive - Indiana University/Box Sync/PhD/Data-Centric-KMeans/";

int main(){

    // Experiment specific path
    // string filePath = basePath + "clustering_data/";
    // string filePath = basePath + "dims_data/";
    // string filePath = basePath + "scal_data/";
//    string filePath = basePath + "sample_data/";
     basePath = basePath + "real_data/experiment_data/";
    
//    string fileName = "50_2_10.csv";
    // string fileName = "crop.csv";
    // string somefilePath = filePath + fileName;
    
    // string centroidFilePath = filePath + "cropCentroids_30.txt";
    string centroidFilePath = "";
    string somefilePath = "";


    std::vector<vector <float> > dataset;
    vector<int> labels;

    // cout << dataset.max_size() << endl;
    
    // Declare variables
    int num_iterations = 500;
    float threshold = 0.0;
    // int num_clusters = 30;

    // vector<string> file_list = {"fourclass.csv", "codrna.csv", "Breastcancer.csv",
    // "crop.csv", "Census.csv"};

    // vector<string> out_list = {"fourclassCentroids", "codrnaCentroids", "BreastcancerCentroids",
    // "cropCentroids", "CensusCentroids"};


    vector<string> file_list = {"Census.csv"};
    vector<string> out_list = {"CensusCentroids"};
    int num_clusters = 5;

    somefilePath = basePath + "comma_seperated_files/" + file_list[0];
    // Read in the data
    auto t1 = std::chrono::high_resolution_clock::now();
    std::pair<int, int> p = readSimulatedData(somefilePath, dataset, labels, false, false);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto file_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    
    cout << "File reading: " << file_int.count() << " MS\n";
    cout << "Data size: " << dataset.size() << " X " << dataset[0].size() << endl;
    
    int numRows = p.first;
    int numCols = p.second;

    output_data res;
    
    // Read the centroids
    vector<vector<float> > centroids(num_clusters, vector<float>(numCols, 0.0));
    
    centroidFilePath = basePath + "comma_seperated_centroids/" + out_list[0] + "_" + std::to_string(5) + "_0" + ".txt" ;
    cout << centroidFilePath << endl;
    
    read_kplus_plus_centroids(centroidFilePath, centroids, num_clusters);

    // Debug - Testing
    cout << "\nAlgo: KMeans," << " Clusters: " << num_clusters << ", Threshold: " << threshold << endl;
    auto t3 = std::chrono::high_resolution_clock::now();
    res = kmeans(dataset, centroids, num_clusters, threshold, num_iterations, numCols);
    auto t4 = std::chrono::high_resolution_clock::now();
    auto km_int = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3);
    std::cout << "Total KMeans time: " << km_int.count() << " milliseconds\n";
    

    vector<vector<float> > rcentroids(num_clusters, vector<float>(numCols, 0.0));
    read_kplus_plus_centroids(centroidFilePath, rcentroids, num_clusters);

    cout << "\nAlgo: DCKM," << " Clusters: " << num_clusters << ", Threshold: " << threshold << endl;
    auto t5 = std::chrono::high_resolution_clock::now();
    res = dckmeans(dataset, rcentroids, num_clusters, threshold, num_iterations, numCols);
    auto t6 = std::chrono::high_resolution_clock::now();
    auto ms_int2 = std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5);
    std::cout << "Total DCKmeans time: " << ms_int2.count() << " milliseconds\n";

    
    
    // ofstream resFile;
    // string outFile = basePath + "Real_data_all_runs.txt" ;
    // fs::path filePath(outFile);
    // std::error_code ec;

    // if (fs::exists(outFile, ec) && !ec){
    //     resFile.open(outFile, ios::out | ios::trunc);
    // }

    // for(int i=0; i<=file_list.size(); i++){

    //     somefilePath = basePath + file_list[i];
    //     cout << "\nData: " << file_list[i] << endl;

    //     std::pair<int, int> p = readSimulatedData(somefilePath, dataset, labels, false, false);

    //     int numRows = p.first;
    //     int numCols = p.second;

    //     for (int j = 0; j< num_clusters.size(); j++){
            
    //         int clus = num_clusters[j];
    //         vector<vector<float> > centroids(clus, vector<float>(numCols, 0.0));

    //         centroidFilePath = basePath + "centroids/" +out_list[i] + "_" + std::to_string(clus) + ".txt" ;
    //         cout << "Centroid File: " << centroidFilePath << endl;

    //         cout << "\nAlgo: DCKM," << " Clusters: " << clus << ", Threshold: " << threshold << endl;
    //         read_kplus_plus_centroids(centroidFilePath, centroids, clus);
            
    //         auto t5 = std::chrono::high_resolution_clock::now();
    //         res = dckmeans(dataset, centroids, clus, threshold, num_iterations, numCols);
    //         auto t6 = std::chrono::high_resolution_clock::now();
    //         auto ms_int2 = std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5);
            
    //         std::cout << "Total DCKmeans time: " << ms_int2.count() << " milliseconds\n";

    //         arma::mat mat1;
    //         size_t clusters;
    //         arma::mat mat2;
    //         arma::Row<size_t> assignments;

    //         KMeans<metric::MyEuclideanDistance, SampleInitialization, MaxVarianceNewCluster,
    //            ElkanKMeans> k(100);

    //         data::Load(somefilePath, mat1);
    //         data::Load(centroidFilePath, mat2);

    //         auto ml_t1 = std::chrono::high_resolution_clock::now();
    //         k.Cluster(mat1, clus, assignments, mat2, false, true);
    //         auto ml_t2 = std::chrono::high_resolution_clock::now();
    //         auto ml_km = std::chrono::duration_cast<std::chrono::milliseconds>(ml_t2 - ml_t1);
            
    //         std::cout << "Total Mlpack KMeans time: " << ml_km.count() << " milliseconds\n";

    //         if(resFile.is_open()){
    //             resFile << out_list[i] << "," << clus << "," << to_string(ms_int2.count()) << endl;
    //             resFile << out_list[i] << "," << clus << "," << to_string(ml_km.count()) << endl;
    //             resFile.close();
    //         }

    //     }

    // }

    // resFile.close();



    // for(int i=2; i<=20; i++){

    //     num_clusters = i;
    //     vector<vector<float> > km_centers(num_clusters, vector<float>(numCols, 0.0));
    //     vector<int> km_assign(dataset.size());
    //     output_data res;

    //     cout << "\nAlgo: DCKM," << " Clusters: " << num_clusters << ", Threshold: " << threshold << endl;
    //     auto t5 = std::chrono::high_resolution_clock::now();
    //     res = dckmeans(dataset, num_clusters, threshold, num_iterations, numCols);
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
