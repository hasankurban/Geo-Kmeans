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

string basePath = "/u/parishar/nobackup/DATASETS/";


int main(){

       string clus_input_path = basePath + "clustering_data/"; 
       string clus_output_path = "/u/parishar/scratch/DATASETS/real_data/experiment_data/benchmark_clus.csv";
       
       // Declare variables
       vector<string> clus_file_list = {"20_clusters.csv", "50_clusters.csv", "100_clusters.csv", 
       "200_clusters.csv", "500_clusters.csv"};

        vector<int> num_clusters = {20, 50, 100, 200, 500};

    //  vector<string> clus_file_list = {"100_clusters.csv"};
    //  vector<int> num_clusters = {100};

     int num_iters = 1000;
     float threshold = 0.01;
     
     // 50 minutes cutoff for running
     int time_limit = 3000000, clus = 0;
     vector<int> labels;
        
       
       string inputfilePath = "";
       output_data dckm_res;
       output_data ballkm_res;
    
       ofstream clusoutFile;
       clusoutFile.open(clus_output_path, ios::trunc);
       clusoutFile << "Algorithm,Clusters,Runtime,Runtime_per_Iter,Runtime_speedup,Distances,Dist_speed_up,Timeout";
       clusoutFile.close();

       algorithm_utils alg_utils;

        ///////
        // Clustering experiments
        ///////

        for(int i = 0; i < num_clusters.size(); i++){

                clus = num_clusters[i];
                inputfilePath = clus_input_path + clus_file_list[i];
              
                cout << "\n%%%%%%%%%%%%%%%%%%%%%%%" << endl;
                cout << "Processing " << clus_file_list[i] << endl;
                cout << "%%%%%%%%%%%%%%%%%%%%%%%\n" << endl;
                
                vector<vector <float> > dataset;
                
                std::pair<int, int> p = readSimulatedData(inputfilePath, dataset, labels, false, false);
                int numRows = p.first;
                int numCols = p.second;
                    
                vector<vector<float> > centroids(clus, vector<float>(numCols, 0.0));

                string km_timeout = "no";
                string dckm_timeout = "no";
                string ballkm_timeout = "no";


                //####################
                // KMeans-DataCentric
                //####################

                cout << "Algo: DCKM" << endl; 
                alg_utils.init_centroids(centroids, dataset, clus);               
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
                
                // Load data in Eigen format for Ball KMeans
                MatrixOur BallK_dataset = load_data(inputfilePath);
                MatrixOur ballKm_centroids = init_ball_centroids(BallK_dataset, clus);

                ballkm_res = ball_k_means_Ring(BallK_dataset, ballKm_centroids, false, threshold, num_iters, time_limit);

                if (ballkm_res.timeout == true){
                    ballkm_timeout = "yes";
                    cout << "Timeout: BallKmeans time: " << ballkm_res.runtime << " milliseconds" << endl;
                }
                else{
                    cout << "Total BallKmeans time: " << ballkm_res.runtime << " milliseconds" << endl;
                }


                clusoutFile.open(clus_output_path, ios::app);

                clusoutFile << "\nDataCentric-KMeans" << "," << to_string(clus) 
                << "," << 
                std::setprecision(2) << to_string(dckm_res.runtime) << "," << std::setprecision(6) <<
                to_string(float(dckm_res.runtime/dckm_res.loop_counter)) << "," << std::setprecision(2) << to_string(float(ballkm_res.runtime/dckm_res.runtime))
                    << "," << std::setprecision(2) << to_string(dckm_res.num_he) <<
                "," << std::setprecision(2) << to_string(float(ballkm_res.num_he/dckm_res.num_he)) << "," << dckm_timeout;

                clusoutFile << "\nBall-Kmeans" << "," << to_string(clus) 
                << "," << 
                std::setprecision(2) << to_string(ballkm_res.runtime) << "," << std::setprecision(6) <<
                to_string(float(ballkm_res.runtime/ballkm_res.loop_counter)) << "," << std::setprecision(2) << to_string(1)
                    << "," << std::setprecision(2) << to_string(ballkm_res.num_he) <<
                "," << std::setprecision(2) << to_string(1) << "," << ballkm_timeout;    

                clusoutFile.close();

            }

       cout << "Completed Clustering benchmarks" << endl;














       return 0;
 }
