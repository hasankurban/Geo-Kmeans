#include <iostream>
#include <string>
#include <tuple>
#include <iomanip>
#include <chrono>

using namespace std;


void benchmark_scal(string basePath){

        string scal_input_path = basePath;    
        string scal_output_path = basePath + "benchmark_scal.csv";;
        
        // Declare variables
        // vector<string> scal_file_list = {"1000000_points.csv", "3000000_points.csv" , "5000000_points.csv",
        //         "8000000_points.csv", "10000000_points.csv"};
        // vector<int> num_points = {1000000, 3000000, 5000000, 8000000, 10000000};

        vector<string> scal_file_list = {"3000000_points.csv"};
        vector<int> num_points = {3000000};

        vector<int> labels;
        int num_iterations = 2000;
        float threshold = 0.01;
        int num_clusters = 10;

        // 90 minutes cutoff for runtime
        int time_limit = 5400000, clus = 10, points = 0;
        
        string inputfilePath = "";
        bool run_stat = false;
        
        output_data dckm_res;
        output_data ballkm_res;
        
        ofstream scaloutFile;
        scaloutFile.open(scal_output_path, ios::trunc);
        scaloutFile << "Algorithm,Num_Points,Runtime,Runtime_per_Iter,Runtime_speedup,Distances,Dist_speed_up,Timeout";
        scaloutFile.close();

        algorithm_utils alg_utils;

        ///////
        // Scalability experiments
        ///////
        
        for(int i = 0; i < num_points.size(); i++){

            points = num_points[i];
            inputfilePath = scal_input_path + scal_file_list[i];
            
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

            cout << dataset.size() << endl;
            
            //####################
            // KMeans-DataCentric
            //####################

            cout << "Algo: Kmeans-DataCentric" << endl;

            // alg_utils.init_centroids(centroids, dataset, clus);               
            // dckm_res = dckmeans(dataset, centroids, clus, threshold, num_iters, numCols, time_limit);

            dckm_res = dckmeans(dataset, num_clusters, threshold, num_iterations, numCols, time_limit, "sequential", 0);
            
            if (dckm_res.timeout == true){
                dckm_timeout = "yes";
                cout << "Timeout: Kmeans-DataCentric time: " << dckm_res.runtime << " milliseconds" << endl;
            }
            else{
                cout << "Total Kmeans-DataCentric time: " << dckm_res.runtime << " milliseconds" << endl;
            }

            //####################
            // Ball-KMeans
            //####################

            cout << "Algo: Ball-KMeans" << endl;
        
            // MatrixOur ballKm_centroids = init_ball_centroids(BallK_dataset, clus);
            // ballkm_res = ball_k_means_Ring(BallK_dataset, ballKm_centroids, false, threshold, num_iters, time_limit);

            ballkm_res = ball_k_means_Ring(BallK_dataset, false, num_clusters, threshold, num_iterations, time_limit, "sequential", 0);

            if (ballkm_res.timeout == true){
                ballkm_timeout = "yes";
                cout << "Timeout: BallKmeans time: " << ballkm_res.runtime << " milliseconds" << endl;
            }
            else{
                cout << "Total BallKmeans time: " << ballkm_res.runtime << " milliseconds" << endl;
            }

            scaloutFile.open(scal_output_path, ios::app);

            scaloutFile << "\nKmeans-DataCentric" << "," << to_string(points) 
                <<  "," << 
            std::setprecision(2) << to_string(dckm_res.runtime) << "," << std::setprecision(6) <<
            to_string(float(dckm_res.runtime/dckm_res.loop_counter)) << "," << std::setprecision(2) 
            << to_string(float(ballkm_res.runtime/dckm_res.runtime)) << "," << std::setprecision(2) << to_string(dckm_res.num_he) <<
            "," << std::setprecision(2) << to_string(float(ballkm_res.num_he/dckm_res.num_he)) << "," << dckm_timeout;

            scaloutFile << "\nBall-Kmeans" << "," << to_string(points) 
            << "," <<  std::setprecision(2) << to_string(ballkm_res.runtime) << "," << std::setprecision(6) <<
            to_string(float(ballkm_res.runtime/ballkm_res.loop_counter)) << "," << std::setprecision(2) << to_string(1)
                << "," << std::setprecision(2) << to_string(ballkm_res.num_he) <<
            "," << std::setprecision(2) << to_string(1) << "," << ballkm_timeout;    

            scaloutFile.close();

        }
        
    cout << "Completed Scalability benchmarks" << endl;
}
