// #include <iostream>
// #include <string>
// #include <tuple>
// #include "algo_utils.hpp"
// #include "IOutils.hpp"
// #include "data_holder.hpp"
// #include "dckmeans.hpp"
// #include "ball_kmeans++_xf.hpp"
// #include "ball-ann.hpp"
// #include "ball-exp.hpp"
// #include "ball-hamerly.hpp"
// #include <chrono>
// #include <filesystem>

// using namespace std;
// namespace fs = std::filesystem;


// class run_clustering_benchmark{
//     template <typename Tfloat, typename Tint>
//     void run_benchmark(Tfloat threshold, Tfloat num_iterations, 
//     string input_path, string centroid_path, string output_path);

//     template <typename Tfloat, typename Tint>
//     void write_result(ofstream &resFile, Tint i, Tint j, string DataName, string algorithm, 
//     Tint clusters, Tfloat time, Tint iters, Tint distanceCalc);
// };


// template <typename Tfloat, typename Tint>
// void write_result(ofstream &resFile, Tint i, Tint j, string DataName, string algorithm, Tint clusters, 
// Tfloat time, Tint iters, Tint distanceCalc){

//     if(resFile.is_open()){
//         if (i == 0 & j ==0){
//             resFile << "Data, Algorithm, Clusters, Time, Iters, DistanceCalc\n" ;
//         }
//         resFile << DataName << "," << algorithm << "," << std::to_string(clusters) << "," << std::to_string(time) << "," << to_string(iters)
//         << "," << std::to_string(distanceCalc) << "\n";
//     }
// }


// template <typename Tfloat, typename Tint>
// void run_clustering_benchmark(Tfloat threshold, Tint num_iterations, 
// string input_path, string centroid_path, string output_path){

//     // Set the parameters
//     vector<Tint> num_clusters = {50};
//     int num_rep  = 1;
//     vector<string> file_names = {"Breastcancer.csv", "CustomerSaleRecords.csv" , "CreditRisk.csv",
//             "Census.csv", "UserLocation.csv",
//             "crop.csv", "Twitter.csv", "birch.csv"};

//     vector<string> labelNames = {"Breastcancer", "CustomerSaleRecords", "CreditRisk",
//             "Census", "UserLocation",
//             "crop", "Twitter", "birch"};

//     vector<string> centroidFiles = {"BreastcancerCentroids", "CustomerSaleRecordsCentroids", "CreditRiskCentroids",
//             "CensusCentroids", "UserLocationCentroids",
//             "cropCentroids", "TwitterCentroids", "birchCentroids"}


//     // vector<string> file_names = {"Kegg.csv"};
//     // vector<string> labelNames = {"Kegg"};
//     // vector<string> centroidFiles = {"KeggCentroids.txt"};

//     Tint iters = 0, numRows = 0, numCols = 0, clus = 0;
//     double start_time = 0, end_time = 0, runTime=0;

//     std::vector<vector <Tfloat> > dataset;
//     vector<Tint> labels;

//     string read_filePath = "", outFile = "", centroid_filePath = "";
//     string fileName = "";
//     ofstream resFile;


//     cout << "#################################\n" ;
//     cout << "Starting Experiments on Real Data\n" ;
//     cout << "#################################\n\n" ;

//     outFile = output_path + "self_Avg_output.csv" ;
//     fs::path filePath(outFile);
//     std::error_code ec;

//     if (fs::exists(outFile, ec) && !ec){
//         resFile.open(outFile, ios::out | ios::trunc);
//         resFile.close();
//     }

//     output_data res;

//     float km_avg_time = 0, dckm_avg_time = 0, ball_avg_time = 0, ann_avg_time = 0, exp_avg_time = 0; 

//     for (int i = 0; i<file_names.size(); i++){

//         fileName = file_names[i];
//         somefilePath = input_path + fileName;

//         std::pair<int, int> p = readSimulatedData(read_filePath, dataset, labels, false, false);

//         numRows = p.first;
//         numCols = p.second;

//         // cout << "Rows: " << numRows << " C: " << numCols << endl;

//         for(int j = 0; j < num_clusters.size(); j++){
            
//             clus = num_clusters[j];
//             vector<vector<float> > centroids(clus, vector<float>(numCols, 0.0));

//             // vector<float> km_iter_counter(5, 0);
//             // vector<float> km_dist_counter(5, 0);
//             // vector<float> km_time_counter(5, 0);

//             vector<float> dckm_iter_counter(5, 0);
//             vector<float> dckm_dist_counter(5, 0);
//             vector<float> dckm_time_counter(5, 0);
//             vector<float> dckm_speed_up(5, 0);

//             vector<float> ball_iter_counter(5, 0);
//             vector<float> ball_dist_counter(5, 0);
//             vector<float> ball_time_counter(5, 0);
//             vector<float> ball_speed_up(5, 0);

//             // vector<float> ann_iter_counter(5, 0);
//             // vector<float> ann_dist_counter(5, 0);
//             // vector<float> ann_time_counter(5, 0);
//             // vector<float> ann_speed_up(5, 0);

//             // vector<float> ham_iter_counter(5, 0);
//             // vector<float> ham_dist_counter(5, 0);
//             // vector<float> ham_time_counter(5, 0);
//             // vector<float> ham_speed_up(5, 0);

//             // vector<float> exp_iter_counter(5, 0);
//             // vector<float> exp_dist_counter(5, 0);
//             // vector<float> exp_time_counter(5, 0);
//             // vector<float> exp_speed_up(5, 0);


//             for(int k=0; k < num_rep ; k++){

//                 centroidFilePath = centroid_path + centroidFiles[i] + "_" + to_string(clus) + "_" + to_string(k) + ".txt";

//                 // read_kplus_plus_centroids(centroidFilePath, centroids, clus);

//                 //####################
//                 // KMeans
//                 //####################
//                 // cout << "\nAlgo: KMeans," << " Data: " << fileName << " Clusters: " << clus << ", Threshold: " << threshold << endl;
                
//                 // start_time = clock();
//                 // res = kmeans(dataset, centroids, clus, threshold, num_iterations, numCols);
//                 // runTime = (clock() - start_time)/CLOCKS_PER_SEC;

//                 // if (runTime < 0){
//                 //     runTime = runTime*1000;
//                 // }

//                 // km_iter_counter.push_back(res.loop_counter);
//                 // km_dist_counter.push_back(res.num_he);
//                 // km_time_counter.push_back(runTime);
                
                
//                 // Write to output file
//                 // write_result(resFile, i, j, labelNames[i], "Lloyd", clus, 
//                 // res.loop_counter, res.num_he, runTime);

//                 // ####################
//                 // KMeans Data Centric
//                 // ####################
//                 cout << "\nAlgo: KMeans-DC," << " Data: " << fileName << " Clusters: " << clus << endl;
                
//                 read_kplus_plus_centroids(centroidFilePath, centroids, clus);
//                 runTime = 0;
                
//                 start_time = clock();
//                 res = dckmeans(dataset, centroids, clus, threshold, num_iterations, numCols);
//                 runTime = (clock() - start_time)/CLOCKS_PER_SEC;

//                 if (runTime < 0){
//                     runTime = runTime*1000;
//                 }

//                 // Write to output file
//                 // write_result(resFile, 1, 1, labelNames[i], "KMeans-DC", clus, 
//                 // runTime, res.loop_counter, res.num_he);

//                 dckm_iter_counter.push_back(res.loop_counter);
//                 dckm_dist_counter.push_back(res.num_he);
//                 dckm_time_counter.push_back(runTime);

           
//                 //####################
//                 // Ball Kmeans
//                 //####################

//                 cout << "\nAlgo: Ball-KMeans," << " Data: " << fileName << " Clusters: " << clus << ", Threshold: " << threshold << endl;

//                 // Load data in Eigen format for Ball KMeans
//                 MatrixOur BallK_dataset = load_data(somefilePath);
//                 MatrixOur ballKm_centroids = load_centroids(centroidFilePath, clus, numCols);

//                 // // cout << BallK_dataset.rows() << " X " << BallK_dataset.cols() << endl;
//                 // // cout << ballKm_centroids.rows() << " X " << ballKm_centroids.cols() << endl;
//                 // // cout << ballKm_centroids(4, 2) << endl;

//                 start_time = clock();
//                 res = ball_k_means_Ring(BallK_dataset, ballKm_centroids, true, threshold, num_iterations);
//                 runTime = (clock() - start_time)/CLOCKS_PER_SEC;

//                 if (runTime < 0){
//                     runTime = runTime*1000;
//                 }

//                 // // // Write to output file
//                 // write_result(resFile, 1, 1, labelNames[i], "Ball-Kmeans", clus, 
//                 // runTime, res.loop_counter, res.num_he);

//                 ball_iter_counter.push_back(res.loop_counter);
//                 ball_dist_counter.push_back(res.num_he);
//                 ball_time_counter.push_back(runTime);

            
//                 // //####################
//                 // // Hamerly Kmeans
//                 // //####################

//                 // cout << "\nAlgo: Hamerly Kmeans," << " Data: " << fileName << " Clusters: " << clus << ", Threshold: " << threshold << endl;

//                 // start_time = clock();
//                 // res = run_ham(BallK_dataset, ballKm_centroids, threshold, num_iterations);
//                 // runTime = (clock() - start_time)/CLOCKS_PER_SEC;

//                 // if (runTime < 0){
//                 //     runTime = runTime*1000;
//                 // }

//                 // ham_iter_counter.push_back(res.loop_counter);
//                 // ham_dist_counter.push_back(res.num_he);
//                 // ham_time_counter.push_back(runTime);

//                 // // Write to output file
//                 // write_result(resFile, 1, 1, labelNames[i], "Hamerly", clus, 
//                 // runTime, res.loop_counter, res.num_he);


//                 // //####################
//                 // // Annulus Kmeans
//                 // //####################

//                 cout << "\nAlgo: Annulus Kmeans," << " Data: " << fileName << " Clusters: " << clus << ", Threshold: " << threshold << endl;

//                 start_time = clock();
//                 res = run_ann(BallK_dataset, ballKm_centroids, threshold, num_iterations);
//                 runTime = (clock() - start_time)/CLOCKS_PER_SEC;

//                 if (runTime < 0){
//                     runTime = runTime*1000;
//                 }

//                 // Write to output file
//                 // write_result(resFile, 1, 1, labelNames[i], "Annulus", clus, 
//                 // runTime, res.loop_counter, res.num_he);

//                 ann_iter_counter.push_back(res.loop_counter);
//                 ann_dist_counter.push_back(res.num_he);
//                 ann_time_counter.push_back(runTime);

//                 // //####################
//                 // // Annulus Kmeans
//                 // //####################

//                 cout << "\nAlgo: Exponion Kmeans," << " Data: " << fileName << " Clusters: " << clus << ", Threshold: " << threshold << endl;

//                 // Load data in Eigen format for Ball KMeans
//                 // MatrixOur exp_centroids = load_centroids(centroidFilePath, clus, numCols);

//                 // cout << exp_centroids.rows() << " X " << exp_centroids.cols() << endl;
//                 // cout << ballKm_centroids(5, 2) << endl;
//                 // cout << ham_centroids(5, 2) << endl;
//                 // cout << exp_centroids(5, 2) << endl;

//                 start_time = clock();
//                 res = run_exp(BallK_dataset, ballKm_centroids, threshold, num_iterations);
//                 runTime = (clock() - start_time)/CLOCKS_PER_SEC;

//                 if (runTime < 0){
//                     runTime = runTime*1000;
//                 }

//                 ham_iter_counter.push_back(res.loop_counter);
//                 exp_dist_counter.push_back(res.num_he);
//                 exp_time_counter.push_back(runTime);
            
//         }

//         // Calculate average statistics

//         for(int m=0; m<km_dist_counter.size(); m++){
//             km_avg

//         }

//         // Write to output file
//         resFile.open(ios_base::app);
//         write_result(resFile, i, j, labelNames[i], "Lloyd", clus, 
//         res.loop_counter, res.num_he, runTime);
//         resFile.close();



//     }

//         // Clear the contents of the dataset
//         dataset.clear();
//     }
//     resFile.close(); 
// }