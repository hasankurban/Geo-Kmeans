#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>

using namespace std;

void benchmark_clus(string basePath){

        string clus_input_path = basePath; 
        string clus_output_path = basePath + "benchmark_clus.csv";
        
        // Declare variables
        // vector<string> clus_file_list = {"20_clusters.csv", "40_clusters.csv", "60_clusters.csv", 
        // "80_clusters.csv", "100_clusters.csv"};

        vector<string> clus_file_list = {"20_clusters.csv", "100_clusters.csv"};

        int num_iterations = 2000, clus = 0;
        float threshold = 0.01;
        //  vector<int> num_clusters = {20, 50, 100, 200, 500};
        vector<int> num_clusters = {20, 100};
        int seed = 12;
        float km_time = 0, kmdc_time = 0, bkm_time = 0; 

        
        // 60 minutes cutoff for running
        int time_limit = 3600000;
        vector<int> labels;
            
        string inputfilePath = "";
        output_data dckm_res;
        output_data ballkm_res;

        ofstream clusoutFile;
        clusoutFile.open(clus_output_path, ios::trunc);
        clusoutFile << "Algorithm,Clusters,Runtime,Runtime_per_Iter,Runtime_speedup,Distances,Dist_speed_up,Timeout";
        clusoutFile.close();

        algorithm_utils alg_utils;

        //////////////////////////
        // Clustering experiments
        //////////////////////////

        for(int i = 0; i < num_clusters.size(); i++){

            clus = num_clusters[i];
            inputfilePath = clus_input_path + clus_file_list[i];
            
            cout << "\n%%%%%%%%%%%%%%%%%%%%%%%" << endl;
            cout << "Processing " << inputfilePath << endl;
            cout << "%%%%%%%%%%%%%%%%%%%%%%%\n" << endl;
            
            vector<vector <float> > dataset;
            
            std::pair<int, int> p = readSimulatedData(inputfilePath, dataset, labels, false, false);
            int numRows = p.first;
            int numCols = p.second;

            // Load data in Eigen format for Ball KMeans
            MatrixOur BallK_dataset = load_data(inputfilePath);

            string km_timeout = "no";
            string dckm_timeout = "no";
            string ballkm_timeout = "no";

            
            //####################
            // KMeans-DataCentric
            //####################

            cout << "Algo: Kmeans-DataCentric" << endl; 

            auto kmdc_start_time = std::chrono::high_resolution_clock::now();

            dckm_res = dckmeans(dataset, clus, threshold, num_iterations, numCols, time_limit, "random", seed);

            auto kmdc_end_time = std::chrono::high_resolution_clock::now();
            kmdc_time = std::chrono::duration_cast<std::chrono::seconds>(kmdc_end_time - kmdc_start_time).count();

            if (dckm_res.timeout == true){
                dckm_timeout = "yes";
                cout << "Timeout: Kmeans-DataCentric time: " << kmdc_time << " seconds" << endl;
            }
            else{
                cout << "Total Kmeans-DataCentric time: " << kmdc_time << " seconds" << endl;
            }

            //####################
            // Ball-KMeans
            //####################

            auto bkm_start_time = std::chrono::high_resolution_clock::now();
            
            ballkm_res = ball_k_means_Ring(BallK_dataset, false, clus, threshold, num_iterations, time_limit, 
                        "random", seed);

            auto bkm_end_time = std::chrono::high_resolution_clock::now();
            bkm_time = std::chrono::duration_cast<std::chrono::seconds>(bkm_end_time - bkm_start_time).count();

            if (ballkm_res.timeout == true){
                ballkm_timeout = "yes";
                cout << "Timeout: BallKmeans time: " << bkm_time << " seconds" << endl;
            }
            else{
                cout << "Total BallKmeans time: " << bkm_time << " seconds" << endl;
            }

            clusoutFile.open(clus_output_path, ios::app);

            clusoutFile << "\nKmeans-DataCentric" << "," << to_string(clus) 
            << "," << std::setprecision(2) << to_string(kmdc_time) << "," << std::setprecision(3) <<
            to_string((float)kmdc_time/dckm_res.loop_counter) << "," << std::setprecision(3) << 
            to_string((float)bkm_time/kmdc_time) << "," << to_string(dckm_res.num_he) <<
            "," << std::setprecision(3) << 
            to_string((float)ballkm_res.num_he/dckm_res.num_he) << "," << dckm_timeout;

            clusoutFile << "\nBall-Kmeans" << "," << to_string(clus) 
            << "," << std::setprecision(2) << to_string(bkm_time) << "," << std::setprecision(3) <<
            to_string((float)bkm_time/ballkm_res.loop_counter) << "," << to_string(1)
            << "," << to_string(ballkm_res.num_he) <<
            "," << to_string(1) << "," << ballkm_timeout;    

            clusoutFile.close();

        }

    cout << "Completed synthetic data clustering benchmarks" << endl;
 }