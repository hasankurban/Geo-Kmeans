#include <iostream>
#include <string>
#include <tuple>
#include <iomanip>
#include <chrono>

using namespace std;


void benchmark_dims(string basePath){

       string dims_input_path = basePath;    
       string dims_output_path = basePath + "benchmark_dims.csv";
       
       // Declare variables
        vector<string> dims_file_list = {"200_dims.csv" , "300_dims.csv", "400_dims.csv", "500_dims.csv", "600_dims.csv"};
        vector<int> num_dims = {200, 300, 400, 500, 600};


        //   vector<string> dims_file_list = {"200_dims.csv"};
        //   vector<int> num_dims = {200};
       vector<int> labels;

        int num_iterations = 1000;
        float threshold = 0.01;
        int num_clusters = 10;
        
        // 90 minutes cutoff for running
        int time_limit = 5400000, dims = 0, clus = 20;
            
        string inputfilePath = "";
        
        output_data km_res;
        output_data dckm_res;
        output_data ballkm_res;
       

        ofstream dimsoutFile;
        dimsoutFile.open(dims_output_path, ios::trunc);
        dimsoutFile << "Algorithm,Num_Points,Iters,Runtime,Runtime_per_Iter,Runtime_speedup,Distances,Dist_speed_up,Timeout";
        dimsoutFile.close();

        algorithm_utils alg_utils;

        ///////
        // Dimensionality experiments
        ///////

        for(int i = 0; i < num_dims.size(); i++){

                dims = num_dims[i];
                inputfilePath = dims_input_path + dims_file_list[i];
              
                cout << "\n%%%%%%%%%%%%%%%%%%%%%%%" << endl;
                cout << "Processing " << dims_file_list[i] << endl;
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

                // alg_utils.init_centroids(centroids, dataset, clus);               
                // dckm_res = dckmeans(dataset, centroids, clus, threshold, num_iters, numCols, time_limit);

                dckm_res = dckmeans(dataset, num_clusters, threshold, num_iterations, numCols, time_limit, "sequential", 0);
                
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

                cout << "Algo: Ball-KMeans" << endl; 
                
                // Load data in Eigen format for Ball KMeans
                MatrixOur BallK_dataset = load_data(inputfilePath);
                // MatrixOur ballKm_centroids = init_ball_centroids(BallK_dataset, clus);

                ballkm_res = ball_k_means_Ring(BallK_dataset, false, num_clusters, threshold, num_iterations, time_limit, 
                        "sequential", 0);

                if (ballkm_res.timeout == true){
                    ballkm_timeout = "yes";
                    cout << "Timeout: BallKmeans time: " << ballkm_res.runtime << " milliseconds" << endl;
                }
                else{
                    cout << "Total BallKmeans time: " << ballkm_res.runtime << " milliseconds" << endl;
                }


                dimsoutFile.open(dims_output_path, ios::app);

                dimsoutFile << "\nDataCentric-KMeans" << "," << to_string(dims) 
                << "," << std::setprecision(2) << to_string(dckm_res.loop_counter) <<  "," << 
                std::setprecision(2) << to_string(dckm_res.runtime) << "," << std::setprecision(6) <<
                to_string(float(dckm_res.runtime/dckm_res.loop_counter)) << "," << std::setprecision(2) << to_string(float(km_res.runtime/dckm_res.runtime))
                    << "," << std::setprecision(2) << to_string(dckm_res.num_he) <<
                "," << std::setprecision(2) << to_string(float(km_res.num_he/dckm_res.num_he)) << "," << dckm_timeout;

                dimsoutFile << "\nBall-Kmeans" << "," << to_string(dims) 
                << "," << std::setprecision(2) << to_string(ballkm_res.loop_counter) <<  "," << 
                std::setprecision(2) << to_string(ballkm_res.runtime) << "," << std::setprecision(6) <<
                to_string(float(ballkm_res.runtime/ballkm_res.loop_counter)) << "," << std::setprecision(2) << to_string(float(km_res.runtime/ballkm_res.runtime))
                    << "," << std::setprecision(2) << to_string(ballkm_res.num_he) <<
                "," << std::setprecision(2) << to_string(float(km_res.num_he/ballkm_res.num_he)) << "," << ballkm_timeout;    

                dimsoutFile.close();

            }

       cout << "Completed Dimensionality Benchmarks" << endl;
}