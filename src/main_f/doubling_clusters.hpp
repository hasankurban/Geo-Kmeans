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
       vector<string> file_list = {"magic.csv", "spambase.csv", "crop.csv", "Twitter.csv", "birch.csv"};
       vector<string> data_list = {"Magic", "Spambase", "Crop", "Twitter", "Birch"};

    //    vector<string> file_list = {"magic.csv", "spambase.csv"};
    //    vector<string> data_list = {"Magic", "Spambase"};

        int num_iterations = 2000;
        float threshold = 0.001;
        
       string inputfilePath = "";
       bool run_stat = false;
       
       vector<int> labels;
       output_data km_res;
       output_data dckm_res;
       output_data ballkm_res;
       
       // Timeout limit
       int time_limit = 900000;
       
       //initial seed for replication (due to random data selection)
       int seed = 78;
       vector<int> clusters = {3, 6, 12, 24};
       int num_points = 0;

       ofstream avgresFile;
       string outFile = out_path + "doubling_clusters.csv" ;
       
       avgresFile.open(outFile, ios::trunc);
       avgresFile << "Algorithm,Data,Clusters,Distances,Runtime,Iterations,Timeout";
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

            // cout << "Normal Data" << endl;
            // for (int m = 0; m<2; m++){
            //     for (int n = 0; n<dataset[0].size(); n++){
            //         cout << dataset[m][n] << " " ;
            //     }
            //     cout << "\n";
            // }

            // Load data in Eigen format for Ball KMeans
            MatrixOur BallK_dataset = load_data(inputfilePath);

            // cout << "BAll Data" << endl;
            // for (int m = 0; m<2; m++){
            //     for (int n = 0; n<BallK_dataset.cols(); n++){
            //         cout << BallK_dataset(m, n) << " " ;
            //     }
            //     cout << "\n";
            // }

            for (int j = 0; j<clusters.size(); j++){

                int clus = clusters[j];

                cout << "\n" << endl;
                cout << "Clusters: " << clus << endl;
                cout << "\n";
                
                string km_timeout = "no";
                string dckm_timeout = "no";
                string ballkm_timeout = "no";

                vector<vector<float> > centroids(clus, vector<float>(numCols, 0));

                //####################
                // KMeans
                //####################
                cout << "Algo: KMeans" << endl; 
                // alg_utils.init_centroids_sequentially(centroids, dataset, clus);   
                // km_res = kmeans(dataset, centroids, clus, threshold, num_iters, numCols, time_limit);

                km_res = kmeans(dataset, clus, threshold, num_iterations, numCols, time_limit, "sequential", 0);
                
                if (km_res.timeout == true){
                    km_timeout = "yes";
                    cout << "Timeout: Kmeans time: " << km_res.runtime << " milliseconds" << endl;
                }

                //####################
                // KMeans-DataCentric
                //####################
                cout << "Algo: DCKM" << endl; 
                // alg_utils.init_centroids_sequentially(centroids, dataset, clus);
                // dckm_res = dckmeans(dataset, centroids, clus, threshold, num_iters, numCols, time_limit);

                dckm_res = dckmeans(dataset, clus, threshold, num_iterations, numCols, time_limit, "sequential", 0);
                
                if (dckm_res.timeout == true){
                    dckm_timeout = "yes";
                    cout << "Timeout: DCKmeans time: " << dckm_res.runtime << " milliseconds" << endl;
                }

                //####################
                // Ball-KMeans
                //####################
                cout << "Algo: BallKMeans" << endl; 
                // MatrixOur ballKm_centroids = init_ball_centroids(BallK_dataset, clus);
                // ballkm_res = ball_k_means_Ring(BallK_dataset, ballKm_centroids, false, threshold, num_iters, time_limit);

                ballkm_res = ball_k_means_Ring(BallK_dataset, false, clus, threshold, num_iterations, time_limit, "sequential", 0);
                if (ballkm_res.timeout == true){
                    ballkm_timeout = "yes";
                    cout << "Timeout: BallKmeans time: " << ballkm_res.runtime << " milliseconds" << endl;
                }

                cout << "Data: " << file_list[i] << "\t Clusters:" << clus 
                << "\t calc: " << km_res.num_he <<  " " << dckm_res.num_he <<  " " << ballkm_res.num_he << " " << 
                ballkm_res.loop_counter << " " << km_res.runtime << " " << dckm_res.runtime << " " << ballkm_res.runtime << endl;

                avgresFile.open(outFile, ios::app);

                // Algorithm,Data,Clusters,Prop,Distances,Timeout
                avgresFile << "\nKMeans" << "," << data_list[i] << "," << to_string(clus) 
                << "," << to_string(km_res.num_he) << "," << to_string(km_res.runtime) << "," << to_string(km_res.loop_counter) << ","
                << km_timeout;

                avgresFile << "\nDataCentric-KMeans" << "," << data_list[i] << "," << to_string(clus) 
                << "," << to_string(dckm_res.num_he) << ","  << to_string(dckm_res.runtime) << "," << to_string(dckm_res.loop_counter) << ","
                << dckm_timeout;

                avgresFile << "\nBall-Kmeans" << "," << data_list[i] << "," << to_string(clus) 
                << "," << to_string(ballkm_res.num_he) << "," << to_string(ballkm_res.runtime) << "," << to_string(ballkm_res.loop_counter) << "," 
                << ballkm_timeout;    

                avgresFile.close();

            }

        }

       cout << "Completed Doubling Cluster Experiments" << endl;

}
