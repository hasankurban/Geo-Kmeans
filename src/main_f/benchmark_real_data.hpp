#include <iostream>
#include <string>
#include <tuple>
#include "algo_utils.hpp"
#include "IOutils.hpp"
#include "data_holder.hpp"
#include "kmeans.hpp"
#include "dckmeans.hpp"
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
        resFile << DataName << "," << algorithm << "," << std::to_string(clusters) << "," << std::to_string(0) << "," << to_string(iters)
        << "," << std::to_string(distanceCalc) << "\n";
    }
}


template <typename Tfloat, typename Tint>
void run_clustering_benchmark(Tfloat threshold, Tint num_iterations, 
string input_path, string output_path){

    // Set the parameters
    vector<Tint> num_clusters = {5, 10, 15, 20, 25, 30, 35};
    
    // vector<string> file_names = {"Breastcancer.csv", "CustomerSaleRecords.csv",
    // "SeedData.csv", "Kegg.csv", "EpilepticDetection.csv", "UserLocation.csv",
    // "crop.csv", "Twitter.csv", "XorData.csv"};

    // vector<string> labelNames = {"BreastCancer", "CustomerSaleRecords", "SeedData", 
    // "Kegg", "EpilepticDetection", "UserLocation",
    // "crop", "Twitter", "XorData"};


    vector<string> file_names = {"Breastcancer.csv", "CustomerSaleRecords.csv", "SeedData.csv"};
    vector<string> labelNames = {"Breastcancer", "CustomerSaleRecords", "SeedData"};

    Tint iters = 0, numRows = 0, numCols = 0, clus = 0;

    std::vector<vector <Tfloat> > dataset;
    vector<Tint> labels;

    string somefilePath = "", outFile = "";
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

        std::pair<int, int> p = readSimulatedData(somefilePath, dataset, labels, true, true);

        numRows = p.first;
        numCols = p.second;

        for(int j = 0; j < num_clusters.size(); j++){

            clus = num_clusters[j];
            output_data res;

            cout << "\nAlgo: KMeans," << " Data: " << fileName << " Clusters: " << clus << ", Threshold: " << threshold << endl;
            auto km_start_time = std::chrono::high_resolution_clock::now();
            res = kmeans(dataset, clus, threshold, num_iterations, numCols);
            auto km_end_time = std::chrono::high_resolution_clock::now();
            auto km_runTime = std::chrono::duration_cast<std::chrono::seconds>(km_end_time - km_start_time);

            // Write to output file
            write_result(resFile, i, j, labelNames[i], "KMeans", clus, 
            0, res.loop_counter, res.num_he);

            cout << "\nAlgo: DCKM," << " Data: " << fileName << " Clusters: " << clus << ", Threshold: " << threshold << endl;
            auto kmdc_start_time = std::chrono::high_resolution_clock::now();
            res = dckmeans(dataset, clus, threshold, num_iterations, numCols);
            auto kmdc_end_time = std::chrono::high_resolution_clock::now();
            auto kmdc_runTime = std::chrono::duration_cast<std::chrono::seconds>(kmdc_end_time - kmdc_start_time);

            // Write to output file
            write_result(resFile, 1, 1, labelNames[i], "KMeans-DC", clus, 
            0, res.loop_counter, res.num_he);

            // if(resFile.is_open()){
            //     if (i == 0 & j ==0){
            //         resFile << "Data, Algorithm, Clusters, Time, Iters, DistanceCalc\n" ;
            //     }
            //     resFile << labelNames[i] << "," << "KMeansDC," << std::to_string(clus) << "," << std::to_string(0) << "," << to_string(res.loop_counter)
            //     << "," << std::to_string(res.num_he) << "\n" ;
            // }
           
        }

        // Clear the contents of the dataset
        dataset.clear();

    }

    resFile.close(); 
    
    }