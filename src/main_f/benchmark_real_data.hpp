#include <iostream>
#include <string>
#include <tuple>
#include "algo_utils.hpp"
#include "IOutils.hpp"
#include "data_holder.hpp"
#include "kmeans.hpp"
#include "dckmeans.hpp"
#include "ball_kmeans++_xf.hpp"
#include <time.h>
#include <map>

#include <chrono>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;


class run_clustering_benchmark{
    template <typename Tfloat, typename Tint>
    void run_benchmark(Tfloat threshold, Tfloat num_iterations, 
    string input_path, string output_path);

    template <typename Tfloat, typename Tint>
    void write_result(ofstream &resFile, Tint i, Tint j, string DataName, string algorithm, 
    Tint clusters, Tfloat time, Tint iters, Tint distanceCalc);
};


template <typename Tfloat, typename Tint>
void write_result(ofstream &resFile, Tint i, Tint j, string DataName, string algorithm, Tint clusters, 
Tfloat time, Tint iters, Tint distanceCalc){

    if(resFile.is_open()){
        if (i == 0 & j ==0){
            resFile << "Data, Algorithm, Clusters, Time, Iters, DistanceCalc\n" ;
        }
        resFile << DataName << "," << algorithm << "," << std::to_string(clusters) << "," << std::to_string(time) << "," << to_string(iters)
        << "," << std::to_string(distanceCalc) << "\n";
    }
}


template <typename Tfloat, typename Tint>
void run_clustering_benchmark(Tfloat threshold, Tint num_iterations, 
string input_path, string output_path){

    // Set the parameters
    vector<Tint> num_clusters = {5, 10, 15};
    int num_rep  = 1;
    // vector<string> file_names = {"Breastcancer.csv", "CustomerSaleRecords.csv", "CreditRisk.csv"
    // "SeedData.csv", "Kegg.csv", "Census.csv", "UserLocation.csv",
    // "crop.csv", "Twitter.csv", "XorData.csv"};

    // vector<string> labelNames = {"BreastCancer", "CustomerSaleRecords", "CreditRisk", 
    // "Kegg", "Census", "UserLocation",
    // "crop", "Twitter", "XorData"};

    // vector<string> centroidFiles = {"BreastCancerCentroids.txt", "CustomerSaleRecordsCentroids.txt", "CreditRiskCentroids.txt",
    // "KeggCentroids.txt", "CensusCentroids.txt", "UserLocationCentroids.txt",
    // "cropCentroids.txt", "TwitterCentroids.txt", "XorDataCentroids.txt"}


    vector<string> file_names = {"Kegg.csv"};
    vector<string> labelNames = {"Kegg"};
    vector<string> centroidFiles = {"KeggCentroids.txt"};

    Tint iters = 0, numRows = 0, numCols = 0, clus = 0;
    double start_time = 0, end_time = 0, runTime=0;

    std::vector<vector <Tfloat> > dataset;
    vector<Tint> labels;

    string somefilePath = "", outFile = "", centroidFilePath = "";
    string fileName = "";
    ofstream resFile;


    cout << "#################################\n" ;
    cout << "Starting Experiments on Real Data\n" ;
    cout << "#################################\n\n" ;

    outFile = output_path + "Real_data_all_runs.txt" ;
    fs::path filePath(outFile);
    std::error_code ec;

    if (fs::exists(outFile, ec) && !ec){
        resFile.open(outFile, ios::out | ios::trunc);
    }


    for (int i = 0; i<file_names.size(); i++){

        fileName = file_names[i];
        somefilePath = input_path + fileName;

        std::pair<int, int> p = readSimulatedData(somefilePath, dataset, labels, false, false);

        numRows = p.first;
        numCols = p.second;

        centroidFilePath = input_path + centroidFiles[i];

        for(int j = 0; j < num_clusters.size(); j++){
            
            clus = num_clusters[j];

            vector<vector<float> > centroids(clus, vector<float>(numCols, 0.0));

            output_data res;
            
            read_kplus_plus_centroids(centroidFilePath, centroids, clus);

            //####################
            // KMeans
            //####################
            cout << "\nAlgo: KMeans," << " Data: " << fileName << " Clusters: " << clus << ", Threshold: " << threshold << endl;
            
            start_time = clock();
            res = kmeans(dataset, centroids, clus, threshold, num_iterations, numCols);
            runTime = (clock() - start_time)/CLOCKS_PER_SEC;

            if (runTime < 0){
                runTime = runTime*1000;
            }

            // Write to output file
            write_result(resFile, i, j, labelNames[i], "KMeans", clus, 
            runTime, res.loop_counter, res.num_he);

            
            //####################
            // KMeans
            //####################
            cout << "\nAlgo: KMeans-DC," << " Data: " << fileName << " Clusters: " << clus << ", Threshold: " << threshold << endl;
            
            read_kplus_plus_centroids(centroidFilePath, centroids, clus);
            runTime = 0;
            start_time = clock();
            res = dckmeans(dataset, centroids, clus, threshold, num_iterations, numCols);
            runTime = (clock() - start_time)/CLOCKS_PER_SEC;

            if (runTime < 0){
                runTime = runTime*1000;
            }

            // Write to output file
            write_result(resFile, 1, 1, labelNames[i], "KMeans-DC", clus, 
            runTime, res.loop_counter, res.num_he);


            //####################
            // Ball Kmeans
            //####################

            cout << "\nAlgo: Ball-KMeans," << " Data: " << fileName << " Clusters: " << clus << ", Threshold: " << threshold << endl;

            // Load data in Eigen format for Ball KMeans
            MatrixOur BallK_dataset = load_data(somefilePath);
            MatrixOur ballKm_centroids = load_data(centroidFilePath);

            start_time = clock();
            res = ball_k_means_Ring(BallK_dataset, ballKm_centroids, false, threshold, num_iterations);
            runTime = (clock() - start_time)/CLOCKS_PER_SEC;

            if (runTime < 0){
                runTime = runTime*1000;
            }

            // Write to output file
            write_result(resFile, 1, 1, labelNames[i], "Ball-Kmeans", clus, 
            runTime, res.loop_counter, res.num_he);
           
        }

        // Clear the contents of the dataset
        dataset.clear();
    }
    resFile.close(); 
}