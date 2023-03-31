#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
#include <vector>

using namespace std;


void double_clusters(string basePath){

       string input_path = basePath;     
       string out_path = basePath ;
       
       // Declare variable
       vector<string> file_list = {"census.csv", "crop.csv", "Twitter.csv", "birch.csv"};
       vector<string> data_list = {"Census", "Crop", "Twitter", "Birch"};

//    vector<string> file_list = {"magic.csv", "spambase.csv"};
//    vector<string> data_list = {"Magic", "Spambase"};

        int num_iterations = 2000;
        float threshold = 0.001;
        
       string inputfilePath = "";
       bool run_stat = false;
       
       vector<int> labels;
       output_data km_res;
       output_data kmdc_res;
       output_data ballkm_res;
       
       // Timeout limit
       int time_limit = 900000;
       
       //initial seed for replication (due to random data selection)
       int seed = 78;
       vector<int> clusters = {3, 6, 12, 24};
       int num_points = 0;
       vector<int> rep = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};

       float km_time = 0, kmdc_time = 0, bkm_time = 0 ;
       string km_timeout = "no", kmdc_timeout = "no", ballkm_timeout = "no";

       ofstream avgresFile;
       string outFile = out_path + "doubling_clusters.csv" ;
       
       avgresFile.open(outFile, ios::trunc);
       avgresFile << "Algorithm,Data,Clusters,Distances,Iterations,Timeout,Sample";
       avgresFile.close();
       string alg = "";

       ofstream tempFile;
       algorithm_utils alg_utils;

       for(int i=0; i < file_list.size(); i++){

        cout << "\n%%%%%%%%%%%%%%%%%%%%%%%" << endl;
        cout << "Processing " << file_list[i] << endl;
        cout << "%%%%%%%%%%%%%%%%%%%%%%%\n" << endl;

        inputfilePath = input_path + file_list[i];
        cout << inputfilePath << endl;

        vector<vector <float> > dataset;
        
        std::pair<int, int> p = readSimulatedData(inputfilePath, dataset, labels, false, false);
        int numRows = p.first;
        int numCols = p.second;

        // Load data in Eigen format for Ball KMeans
        MatrixOur BallK_dataset = load_data(inputfilePath);

        for (int j = 0; j<clusters.size(); j++){

            int clus = clusters[j];
            // vector<vector<float> > centroids(clus, vector<float>(numCols, 0));

            cout << "\n" << endl;
            cout << "Clusters: " << clus << endl;
            cout << "\n";

            for (int n = 0; n<rep.size(); n++){

                //####################
                // KMeans
                //####################
                cout << "Algo: KMeans" << endl; 
                
                auto km_start_time = std::chrono::high_resolution_clock::now();

                km_res = kmeans(dataset, clus, threshold, num_iterations, numCols, time_limit, 
                "random", seed+rep[n]);

                auto km_end_time = std::chrono::high_resolution_clock::now();
                km_time = std::chrono::duration_cast<std::chrono::milliseconds>(km_end_time - km_start_time).count();
                
                if (km_res.timeout == true){
                    km_timeout = "yes";
                    cout << "Timeout: Kmeans time: " << km_time << " milliseconds" << endl;
                }

                //####################
                // KMeans-DataCentric
                //####################
                cout << "Algo: Kmeans-DataCentric" << endl; 

                auto kmdc_start_time = std::chrono::high_resolution_clock::now();

                kmdc_res = dckmeans(dataset, clus, threshold, num_iterations, numCols, time_limit, 
                "random", seed+rep[n]);

                auto kmdc_end_time = std::chrono::high_resolution_clock::now();
                kmdc_time = std::chrono::duration_cast<std::chrono::milliseconds>(kmdc_end_time - kmdc_start_time).count();
                
                if (kmdc_res.timeout == true){
                    kmdc_timeout = "yes";
                    cout << "Timeout: Kmeans-DataCentric time: " << kmdc_time << " milliseconds" << endl;
                }

                //####################
                // Ball-KMeans
                //####################
                cout << "Algo: BallKMeans" << endl; 

                auto bkm_start_time = std::chrono::high_resolution_clock::now();

                ballkm_res = ball_k_means_Ring(BallK_dataset, false, clus, threshold, num_iterations, time_limit, 
                "random", seed+rep[n]);

                auto bkm_end_time = std::chrono::high_resolution_clock::now();
                    bkm_time = std::chrono::duration_cast<std::chrono::milliseconds>(bkm_end_time - bkm_start_time).count();
                
                if (ballkm_res.timeout == true){
                    ballkm_timeout = "yes";
                    cout << "Timeout: BallKmeans time: " << bkm_time << " milliseconds" << endl;
                }

                // cout << "Data: " << file_list[i] << "\t Clusters:" << clus 
                // << "\t calc: " << km_res.num_he <<  " " << kmdc_res.num_he <<  " " << ballkm_res.num_he << " " << 
                // ballkm_res.loop_counter << " " << km_res.runtime << " " << kmdc_res.runtime << " " << ballkm_res.runtime << endl;

                avgresFile.open(outFile, ios::app);

                avgresFile << "\nKmeans" << "," << data_list[i] << "," << to_string(clus) 
                << "," << to_string(km_res.num_he) << "," << to_string(km_res.loop_counter) << ","
                << km_timeout << "," << to_string(n+1);    

                avgresFile << "\nKmeans-DataCentric" << "," << data_list[i] << "," << to_string(clus) 
                << "," << to_string(kmdc_res.num_he) << "," << to_string(kmdc_res.loop_counter) << ","
                << kmdc_timeout << "," << to_string(n+1);    

                avgresFile << "\nBall-Kmeans" << "," << data_list[i] << "," << to_string(clus) 
                << "," << to_string(ballkm_res.num_he) << "," << to_string(ballkm_res.loop_counter) << "," 
                << ballkm_timeout << "," << to_string(n+1);        

                avgresFile.close();

            }

        }

    }

    cout << "Completed Doubling Cluster Experiments" << endl;

}
