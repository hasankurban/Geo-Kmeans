#include <iostream>
#include <string>
#include <tuple>
#include <iomanip>
#include <chrono>

using namespace std;


void benchmark_on_real_data(string basePath){

       string input_path = basePath;     
       string centroid_path = basePath + "/centroids/";
       string out_path = basePath;
       
       // Declare variables
       vector<string> file_list = {"Breastcancer.csv", "CustomerSaleRecords.csv" , "CreditRisk.csv",
            "magic.csv", "spambase.csv", "crop.csv", "Twitter.csv", "birch.csv"};

       vector<string> out_list = {"BreastcancerCentroids", "CustomerSaleRecordsCentroids", "CreditRiskCentroids",
            "magicCentroids", "spambaseCentroids", "cropCentroids", "TwitterCentroids", "birchCentroids"};

       vector<string> data_list = {"Breastcancer", "CustomerSaleRecords", "CreditRisk",
            "magic", "spambase", "crop", "Twitter", "birch"};

    //    vector<string> file_list = {"crop.csv", "Twitter.csv", "birch.csv"};
    //    vector<string> out_list = {"cropCentroids", "TwitterCentroids", "birchCentroids"};
    //    vector<string> data_list = {"crop", "Twitter", "birch"};

        int num_iterations = 2000;
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
       vector<string> algorithm = {"Kmeans", "Kmeans-DataCentric", "Ball-Kmeans"};


        for(int i=0; i < file_list.size(); i++){

            inputfilePath = input_path + file_list[i];
              
            cout << "\n%%%%%%%%%%%%%%%%%%%%%%%" << endl;
            cout << "Processing " << inputfilePath << endl;
            cout << "%%%%%%%%%%%%%%%%%%%%%%%\n" << endl;

            vector<vector <float> > dataset;
            
            std::pair<int, int> p = readSimulatedData(inputfilePath, dataset, labels, false, false);
            int numRows = p.first;
            int numCols = p.second;

            // Load data in Eigen format for Ball KMeans
            MatrixOur BallK_dataset = load_data(inputfilePath);

            // cout << file_list[i] << " " << dataset.size() << endl;
            
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
                cout << "Algo: KMeans" << endl; 

                // read_kplus_plus_centroids(centroidFilePath, centroids, clus);
                // km_res = kmeans(dataset, centroids, clus, threshold, num_iters, numCols, time_limit);

                km_res = kmeans(dataset, clus, threshold, num_iterations, numCols, 
                        time_limit, centroidFilePath, 0);
                
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
                cout << "Algo: Kmeans-DataCentric" << endl; 

                // read_kplus_plus_centroids(centroidFilePath, centroids, clus);
                // dckm_res = dckmeans(dataset, centroids, clus, threshold, num_iters, numCols, time_limit);

                dckm_res = dckmeans(dataset, clus, threshold, num_iterations, numCols, time_limit, centroidFilePath, 0);
                
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

                // MatrixOur ballKm_centroids = load_centroids(centroidFilePath, clus, numCols);
                // ballkm_res = ball_k_means_Ring(BallK_dataset, ballKm_centroids, false, threshold, num_iters, time_limit);

                ballkm_res = ball_k_means_Ring(BallK_dataset, false, clus, threshold, num_iterations, time_limit, 
                centroidFilePath, 0);

                if (ballkm_res.timeout == true){
                    ballkm_timeout = "yes";
                    cout << "Timeout: BallKmeans time: " << ballkm_res.runtime << " milliseconds" << endl;
                }
                else{
                    cout << "Total BallKmeans time: " << ballkm_res.runtime << " milliseconds" << endl;
                }

                avgresFile.open(outFile, ios::app);

                avgresFile << "\nKmeans" << "," << data_list[i] << "," << to_string(clus) 
                << "," << std::setprecision(2) << to_string(km_res.loop_counter) <<  "," << 
                std::setprecision(2) << to_string(km_res.runtime) << "," << std::setprecision(6) <<
                to_string(float(km_res.runtime/km_res.loop_counter)) << "," << std::setprecision(2) << to_string(0)
                    << "," << std::setprecision(2) << to_string(km_res.num_he) <<
                "," << std::setprecision(2) << to_string(0) << "," << km_timeout;

                    avgresFile << "\nKmeans-DataCentric" << "," << data_list[i] << "," << to_string(clus) 
                << "," << std::setprecision(2) << to_string(dckm_res.loop_counter) <<  "," << 
                std::setprecision(2) << to_string(dckm_res.runtime) << "," << std::setprecision(6) <<
                to_string(float(dckm_res.runtime/dckm_res.loop_counter)) << "," << std::setprecision(2) << to_string(float(ballkm_res.runtime/dckm_res.runtime))
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
}
