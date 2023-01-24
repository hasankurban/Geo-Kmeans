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
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

string basePath = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/real_data/";


output_data run_km_algo(vector<vector<float> > &dataset, vector<vector<float>> &centroids, int clus, 
float threshold, int num_iters, int numCols, int time_limit){

    output_data res;

    auto t1 = std::chrono::high_resolution_clock::now();
        res = kmeans(dataset, centroids, clus, threshold, num_iters, numCols, time_limit);
        auto t2 = std::chrono::high_resolution_clock::now();
        auto km_time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

    return res;
}


output_data run_dckm_algo(vector<vector<float> > &dataset, vector<vector<float>> &centroids, int clus, 
float threshold, int num_iters, int numCols, int time_limit){

    output_data res;
    auto t1 = std::chrono::high_resolution_clock::now();
    res = dckmeans(dataset, centroids, clus, threshold, num_iters, numCols, time_limit);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto km_time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

    return res;
}


int main(){

       string input_path = basePath + "experiment_data/comma_seperated_files/";     
       string output_path = basePath + "experiment_data/";
       string centroid_path = basePath + "experiment_data/comma_seperated_centroids/";
       string out_path = basePath + "experiment_data/";
       
       // Declare variables
       vector<string> file_list = {"Breastcancer.csv", "CustomerSaleRecords.csv" , "CreditRisk.csv",
            "magic.csv", "spambase.csv",
            "crop.csv", "Twitter.csv", "birch.csv"};

       vector<string> out_list = {"BreastcancerCentroids", "CustomerSaleRecordsCentroids", "CreditRiskCentroids",
            "magicCentroids", "spambaseCentroids",
            "cropCentroids", "TwitterCentroids", "birchCentroids"};

       vector<string> data_list = {"Breastcancer", "CustomerSaleRecords", "CreditRisk",
            "magic", "spambase", "crop", "Twitter", "birch"};


    //    vector<string> file_list = {"Breastcancer.csv"};
    //    vector<string> out_list = {"BreastcancerCentroids"};
    //    vector<string> data_list = {"Breastcancer"};

        int num_iters = 500;
        float threshold = 0.001;
        int num_rep = 1; 
        vector<int> num_clusters = {5, 10, 20, 30};  
        
       
       string inputfilePath = "", centroidFilePath = "";
       bool run_stat = false;
       
       vector<int> labels;
      
       output_data ballkm_res;
       int time_limit = 350000;

       ofstream avgresFile;
       string outFile = out_path + "benchmark_out.csv" ;
       
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

                cout << file_list[i] << " " << dataset.size() << endl;
                for (int j = 0; j< num_clusters.size(); j++){
            
                    int clus = num_clusters[j];
                    

                        for (int rep = 0 ; rep < num_rep ; rep++){

                             
                            vector<vector<float> > centroids(clus, vector<float>(numCols, 0.0));

                        string km_timeout = "no";
                        string dckm_timeout = "no";
                        string ballkm_timeout = "no";

                        
                        // Read centroids  
                        centroidFilePath = centroid_path + out_list[i] + "_" + to_string(clus) + "_" + to_string(rep) +".txt";

                        // cout << "Clus: " << clus << " rep: " << rep << endl;
                        // cout << inputfilePath << endl;
                        // cout <<  centroidFilePath << endl;

                        //####################
                        // KMeans
                        //####################
                        // read_kplus_plus_centroids(centroidFilePath, centroids, clus);
                        output_data km_res;
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
                        // read_kplus_plus_centroids(centroidFilePath, centroids, clus);
                        output_data dckm_res;
                        
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

                        // cout << "Algo: Ball-Kmeans" << endl;
                        
                        // // Load data in Eigen format for Ball KMeans
                        // MatrixOur BallK_dataset = load_data(inputfilePath);
                        // MatrixOur ballKm_centroids = load_centroids(centroidFilePath, clus, numCols);

                        // auto t7 = std::chrono::high_resolution_clock::now();
                        // res = ball_k_means_Ring(BallK_dataset, ballKm_centroids, false, threshold, num_iters, time_limit);
                        // auto t8 = std::chrono::high_resolution_clock::now();
                        // auto ball_time = std::chrono::duration_cast<std::chrono::milliseconds>(t8 - t7);

                        // if (res.timeout == true){
                        //     ballkm_timeout = "yes";
                        //     cout << "Timeout: BallKmeans time: " << res.runtime << " milliseconds" << endl;
                        // }
                        // else{
                        //     cout << "Total BallKmeans time: " << res.runtime << " milliseconds" << endl;
                        // }


                        avgresFile.open(outFile, ios::app);

                        cout << km_res.runtime << " " << to_string(km_res.runtime/km_res.loop_counter) << " " << km_res.num_he << endl;
                        cout << dckm_res.num_he << endl;

                        avgresFile << "\nKM" << "," << data_list[i] << "," << to_string(clus) 
                        << "," << std::setprecision(2) << to_string(km_res.loop_counter) <<  "," << 
                        std::setprecision(2) << to_string(km_res.runtime) << "," << std::setprecision(6) <<
                        to_string(km_res.runtime/km_res.loop_counter) << "," << std::setprecision(2) << to_string(0)
                            << "," << std::setprecision(2) << to_string(km_res.num_he) <<
                        "," << std::setprecision(2) << to_string(0) << "," << km_timeout;

                            avgresFile << "\nDCKM" << "," << data_list[i] << "," << to_string(clus) 
                        << "," << std::setprecision(2) << to_string(dckm_res.loop_counter) <<  "," << 
                        std::setprecision(2) << to_string(dckm_res.runtime) << "," << std::setprecision(6) <<
                        to_string(dckm_res.runtime/dckm_res.loop_counter) << "," << std::setprecision(2) << to_string(km_res.runtime/dckm_res.runtime)
                            << "," << std::setprecision(2) << to_string(dckm_res.num_he) <<
                        "," << std::setprecision(2) << to_string(km_res.num_he/dckm_res.num_he) << "," << dckm_timeout;

                        // avgresFile << "\nBall-Kmeans" << "," << data_list[i] << "," << to_string(clus) 
                        // << "," << std::setprecision(2) << to_string(ball_iters[rep]) <<  "," << 
                        // std::setprecision(2) << to_string(ball_runTime[rep]) << "," << std::setprecision(6) <<
                        // to_string(ball_runTime_per_iter[rep]) << "," << std::setprecision(2) << to_string(runTime[rep]/ball_runTime[rep])
                        //     << "," << std::setprecision(2) << to_string(ball_dist_calcs[rep]) <<
                        // "," << std::setprecision(2) << to_string(dist_calcs[rep]/ball_dist_calcs[rep]) << "," << ballkm_timeout;    

                        avgresFile.close();
                        
                    }
                    
                }

            }

       cout << "Completed benchmarks" << endl;
       return 0;
 }
