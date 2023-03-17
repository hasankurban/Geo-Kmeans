#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
#include <vector>

using namespace std;


void double_prop(string basePath){


       string input_path = basePath + "experiment_data/comma_seperated_files/";     
       string output_path = basePath + "experiment_data/";
       string centroid_path = basePath + "experiment_data/comma_seperated_centroids/";
       string out_path = basePath + "experiment_data/";
       
       // Declare variable
       vector<string> file_list = {"magic.csv", "spambase.csv", "crop.csv", "Twitter.csv", "birch.csv"};
       vector<string> data_list = {"Magic", "Spambase", "Crop", "Twitter", "Birch"};

<<<<<<< HEAD:src/main_f/doubling_proportion.hpp
    //    vector<string> file_list = {"spambase.csv", "Twitter.csv"};
    //    vector<string> data_list = {"Spambase", "Twitter"};
=======
    //    vector<string> file_list = {"magic.csv", "spambase.csv"};
    //    vector<string> data_list = {"Magic", "Spambase"};
>>>>>>> b78b3722d4cb4dc8acbb57a50cb1014ad0f4e3c3:src/main_f/doubling_experiments.cpp

        int num_iters = 2000;
        float threshold = 0.001;
        // vector<float> data_prop = {0.2, 0.4};  
        vector<float> data_prop = {0.2, 0.4, 0.8, 1.0};
        
       string inputfilePath = "", centroidFilePath = "";
       bool run_stat = false;
       
       vector<int> labels;
       output_data km_res;
       output_data dckm_res;
       output_data ballkm_res;
       
       // Timeout limit
       int time_limit = 600000;
       
       //initial seed for replication (due to random data selection)
       int seed = 78;
       int clus = 5;
       int num_points = 0;

       ofstream avgresFile;
       string outFile = out_path + "doubling_proportion.csv" ;
       
       avgresFile.open(outFile, ios::trunc);
       avgresFile << "Algorithm,Data,Clusters,Prop,Distances,Runtime,Iterations,Timeout";
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

            for (int j = 0; j<data_prop.size(); j++){

                float prop = data_prop[j];

                cout << "\n" << endl;
                cout << "Data prop: " << prop << endl;
                cout << "\n";
                
                string km_timeout = "no";
                string dckm_timeout = "no";
                string ballkm_timeout = "no";

                // Extract the proportion of data from original dataset to run the experiments
                num_points = ceil(dataset.size() * prop);
                vector<vector<float> > extracted_data(num_points, vector<float>(numCols, 0.0));
                vector<vector<float> > centroids(clus, vector<float>(numCols, 0));

                alg_utils.extract_data(dataset, extracted_data, num_points, seed+j);

                // Extract data for Ball KMeans
                num_points = ceil(BallK_dataset.rows() * prop);
                MatrixOur extracted_ball_data(num_points, BallK_dataset.cols());
                extract_ball_data(BallK_dataset, extracted_ball_data, prop, num_points, seed+j);

                //####################
                // KMeans
                //####################
                cout << "Algo: KMeans" << endl; 
                alg_utils.init_centroids_sequentially(centroids, extracted_data, clus);   

                km_res = kmeans(extracted_data, centroids, clus, threshold, num_iters, numCols, time_limit);
                
                if (km_res.timeout == true){
                    km_timeout = "yes";
                    cout << "Timeout: Kmeans time: " << km_res.runtime << " milliseconds" << endl;
                }

                //####################
                // KMeans-DataCentric
                //####################

                cout << "Algo: DCKM" << endl; 
                alg_utils.init_centroids_sequentially(centroids, extracted_data, clus);
                
                dckm_res = dckmeans(extracted_data, centroids, clus, threshold, num_iters, numCols, time_limit);
                
                if (dckm_res.timeout == true){
                    dckm_timeout = "yes";
                    cout << "Timeout: DCKmeans time: " << dckm_res.runtime << " milliseconds" << endl;
                }

                //####################
                // Ball-KMeans
                //####################
                cout << "Algo: BallKMeans" << endl; 
                MatrixOur ballKm_centroids = init_ball_centroids(extracted_ball_data, clus);

                ballkm_res = ball_k_means_Ring(extracted_ball_data, ballKm_centroids, false, threshold, num_iters, time_limit);
                // ballkm_res = run_ham(extracted_ball_data, ballKm_centroids, false, threshold, num_iters, time_limit);

                if (ballkm_res.timeout == true){
                    ballkm_timeout = "yes";
                    cout << "Timeout: BallKmeans time: " << ballkm_res.runtime << " milliseconds" << endl;
                }

                cout << "Data: " << file_list[i] << " Prop: " << prop << "\t" << " Clusters:" << clus 
                << "\t calc: " << km_res.num_he <<  " " << dckm_res.num_he <<  " " << ballkm_res.num_he << " " << 
                ballkm_res.loop_counter << " " << km_res.runtime << " " << dckm_res.runtime << " " << ballkm_res.runtime << endl;

                avgresFile.open(outFile, ios::app);

                // Algorithm,Data,Clusters,Prop,Distances,Timeout

                avgresFile << "\nKMeans" << "," << data_list[i] << "," << to_string(clus) 
                << "," << to_string(prop) << "," << to_string(km_res.num_he) << "," << to_string(km_res.runtime) << "," 
                << to_string(km_res.loop_counter) << "," << km_timeout;

                avgresFile << "\nDataCentric-KMeans" << "," << data_list[i] << "," << to_string(clus) 
                << "," << to_string(prop) << "," << to_string(dckm_res.num_he) << ","  << to_string(dckm_res.runtime) << "," 
                << to_string(dckm_res.loop_counter) << "," << dckm_timeout;

                avgresFile << "\nBall-Kmeans" << "," << data_list[i] << "," << to_string(clus) 
                << "," << to_string(prop) << "," << to_string(ballkm_res.num_he) << "," << to_string(ballkm_res.runtime) << "," 
                << to_string(ballkm_res.loop_counter) << "," << ballkm_timeout;    

                avgresFile.close();

            }

        }

       cout << "Completed Doubling Proportion Experiments" << endl;
    //    return 0;
}
