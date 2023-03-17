#include <iostream>
#include <string>
#include <tuple>
#include "data_holder.hpp"
#include "IOutils.hpp"
#include "algo_utils.hpp"
#include "kmeans.hpp"
#include "dckmeans.hpp"
#include "ball_kmeans++_xf.hpp"
#include <map>
#include <iomanip>
#include <chrono>

using namespace std;

string basePath = "/u/parishar/scratch/DATASETS/real_data/";


void doubling_prop(){

       string input_path = basePath;     
       string centroid_path = basePath + "/comma_seperated_centroids/";
       string out_path = basePath;
       
       // Declare variables
       vector<string> file_list = {"Breastcancer.csv", "CustomerSaleRecords.csv" , "CreditRisk.csv",
            "magic.csv", "spambase.csv", "crop.csv", "Twitter.csv", "birch.csv"};

       vector<string> out_list = {"BreastcancerCentroids", "CustomerSaleRecordsCentroids", "CreditRiskCentroids",
            "magicCentroids", "spambaseCentroids", "cropCentroids", "TwitterCentroids", "birchCentroids"};

       vector<string> data_list = {"Breastcancer", "CustomerSaleRecords", "CreditRisk",
            "magic", "spambase", "crop", "Twitter", "birch"};

    //    vector<string> file_list = {"Twitter.csv"};
    //    vector<string> out_list = {"TwitterCentroids"};
    //    vector<string> data_list = {"Twitter"};

        int num_iters = 1000;
        float threshold = 0.001;
        vector<int> num_clusters = {5, 8, 10, 12, 25};  
        
       
       string inputfilePath = "", centroidFilePath = "";
       bool run_stat = false;
       
       vector<int> labels;
       
       output_data km_res;
       output_data dckm_res;
       output_data ballkm_res;
       
       int time_limit = 1800000;

       ofstream avgresFile;
       string outFile = out_path + "benchmark_real_data.csv" ;
       
       avgresFile.open(outFile, ios::trunc);
       avgresFile << "Algorithm,Data,Clusters,Iters,Runtime,Runtime_per_Iter,Runtime_speedup,Distances,Dist_speed_up,Timeout";
       avgresFile.close();
       string alg = "";
       vector<string> algorithm = {"Kmeans", "DataCentric-Kmeans", "Ball-Kmeans"};


        for(int i=0; i < file_list.size(); i++){
              
                cout << "\n%%%%%%%%%%%%%%%%%%%%%%%" << endl;
                cout << "Processing " << file_list[i] << endl;
                cout << "%%%%%%%%%%%%%%%%%%%%%%%\n" << endl;

                inputfilePath = input_path + file_list[i];
                vector<vector <float> > dataset;
                
                std::pair<int, int> p = readSimulatedData(inputfilePath, dataset, labels, false, false);
                int numRows = p.first;
                int numCols = p.second;

                // Load data in Eigen format for Ball KMeans
                MatrixOur BallK_dataset = load_data(inputfilePath);

                cout << file_list[i] << " " << dataset.size() << endl;


                for (int j = 0; j< num_clusters.size(); j++){
            
                    int clus = num_clusters[j];
                        
                        vector<vector<float> > centroids(clus, vector<float>(numCols, 0.0));

                        string km_timeout = "no";
                        string dckm_timeout = "no";
                        string ballkm_timeout = "no";

                        // Read centroids  
                        centroidFilePath = centroid_path + out_list[i] + "_" + to_string(clus) + "_.txt";

                        //####################
                        // KMeans
                        //####################
                        read_kplus_plus_centroids(centroidFilePath, centroids, clus);
                        
                        km_res = kmeans(dataset, centroids, clus, threshold, num_iters, numCols, time_limit);
                        
                        if (km_res.timeout == true){
                            km_timeout = "yes";
                            cout << "Timeout: Kmeans time: " << km_res.runtime << " milliseconds" << endl;
                        }
                        else{
                            std::cout << "Kmeans time: " << km_res.runtime << " milliseconds" << endl;
                        }

                        //####################
                        // KMeans-DataCentric
                        //####################
                        cout << "Algo: DCKM" << endl; 
                        read_kplus_plus_centroids(centroidFilePath, centroids, clus);
                        
                        
                        dckm_res = dckmeans(dataset, centroids, clus, threshold, num_iters, numCols, time_limit);
                        
                        if (dckm_res.timeout == true){
                            dckm_timeout = "yes";
                            cout << "Timeout: DCKmeans time: " << dckm_res.runtime << " milliseconds" << endl;
                        }
                        else{
                            cout << "Total DCKmeans time: " << dckm_res.runtime << " milliseconds" << endl;
                        }

                        
                        //####################
                        // Ball-KMeans
                        //####################
                        MatrixOur ballKm_centroids = load_centroids(centroidFilePath, clus, numCols);

                        ballkm_res = ball_k_means_Ring(BallK_dataset, ballKm_centroids, false, threshold, num_iters, time_limit);

                        if (ballkm_res.timeout == true){
                            ballkm_timeout = "yes";
                            cout << "Timeout: BallKmeans time: " << ballkm_res.runtime << " milliseconds" << endl;
                        }
                        else{
                            cout << "Total BallKmeans time: " << ballkm_res.runtime << " milliseconds" << endl;
                        }

                        avgresFile.open(outFile, ios::app);

                        avgresFile << "\nKMeans" << "," << data_list[i] << "," << to_string(clus) 
                        << "," << std::setprecision(2) << to_string(km_res.loop_counter) <<  "," << 
                        std::setprecision(2) << to_string(km_res.runtime) << "," << std::setprecision(6) <<
                        to_string(float(km_res.runtime/km_res.loop_counter)) << "," << std::setprecision(2) << to_string(0)
                            << "," << std::setprecision(2) << to_string(km_res.num_he) <<
                        "," << std::setprecision(2) << to_string(0) << "," << km_timeout;

                            avgresFile << "\nDataCentric-KMeans" << "," << data_list[i] << "," << to_string(clus) 
                        << "," << std::setprecision(2) << to_string(dckm_res.loop_counter) <<  "," << 
                        std::setprecision(2) << to_string(dckm_res.runtime) << "," << std::setprecision(6) <<
                        to_string(float(dckm_res.runtime/dckm_res.loop_counter)) << "," << std::setprecision(2) << to_string(float(km_res.runtime/dckm_res.runtime))
                            << "," << std::setprecision(2) << to_string(dckm_res.num_he) <<
                        "," << std::setprecision(2) << to_string(float(km_res.num_he/dckm_res.num_he)) << "," << dckm_timeout;

                        avgresFile << "\nBall-Kmeans" << "," << data_list[i] << "," << to_string(clus) 
                        << "," << std::setprecision(2) << to_string(ballkm_res.loop_counter) <<  "," << 
                        std::setprecision(2) << to_string(ballkm_res.runtime) << "," << std::setprecision(6) <<
                        to_string(float(ballkm_res.runtime/ballkm_res.loop_counter)) << "," << std::setprecision(2) << to_string(float(km_res.runtime/ballkm_res.runtime))
                            << "," << std::setprecision(2) << to_string(ballkm_res.num_he) <<
                        "," << std::setprecision(2) << to_string(float(km_res.num_he/ballkm_res.num_he)) << "," << ballkm_timeout;    

                        avgresFile.close();
                    
                }

            }

       cout << "Completed benchmarks" << endl;
       return 0;
 }
