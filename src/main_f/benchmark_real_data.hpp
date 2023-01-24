#include <iostream>
#include <string>
#include <tuple>
#include "data_holder.hpp"
#include "IOutils.hpp"
#include "algo_utils.hpp"
#include "dckmeans.hpp"
#include "ball_kmeans++_xf.hpp"
#include <map>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

string basePath = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/real_data/";

int main(){

       string input_path = basePath + "experiment_data/comma_seperated_files/";     
       string output_path = basePath + "experiment_data/";
       string centroid_path = basePath + "experiment_data/comma_seperated_centroids/";
       string out_path = basePath + "experiment_data/";
       
       // Declare variables
    //    vector<string> file_list = {"Breastcancer.csv", "CustomerSaleRecords.csv" , "CreditRisk.csv",
    //         "Census.csv", "UserLocation.csv",
    //         "crop.csv", "Twitter.csv", "birch.csv"};

    //    vector<string> out_list = {"BreastcancerCentroids", "CustomerSaleRecordsCentroids", "CreditRiskCentroids",
    //         "CensusCentroids", "UserLocationCentroids",
    //         "cropCentroids", "TwitterCentroids", "birchCentroids"};

    //    vector<string> data_list = {"Breastcancer", "CustomerSaleRecords", "CreditRisk",
    //         "Census", "UserLocation", "crop", "Twitter", "birch"};


       int num_iters = 500;
       float threshold = 0;
       int num_rep = 5;     
       
       vector<string> file_list = {"Breastcancer.csv"};
       vector<string> out_list = {"BreastcancerCentroids"};
       vector<string> data_list = {"Breastcancer"};
       vector<int> num_clusters = {2, 5, 10, 20, 30};

       map<string, float> km_dist = { {"Breastcancer2", 7752.0}, {"Breastcancer5", 27360.0}, {"Breastcancer10", 53580.0}, 
                                          {"Breastcancer20", 143640.0}, {"Breastcancer30", 246240.0}, {"CustomerSaleRecords2", 7408.8}, 
                                          {"CustomerSaleRecords5", 38808.0}, {"CustomerSaleRecords10", 65268.0}, 
                                          {"CustomerSaleRecords20", 118188.0}, {"CustomerSaleRecords30", 224910.0}, 
                                          {"CreditRisk2", 15215.2}, {"CreditRisk5", 59059.0}, {"CreditRisk10", 116116.0}, 
                                          {"CreditRisk20", 456456.0}, 
                                          {"CreditRisk30", 408408.0}, {"Census2", 3002740.8}, {"Census5", 7325964.0}, 
                                          {"Census10", 21616116.0}, {"Census20", 97860408.0}, {"Census30", 130239360.0}, 
                                          {"UserLocation2", 454586.4}, {"UserLocation5", 1953924.0}, {"UserLocation10", 5024376.0}, 
                                          {"UserLocation20", 14594616.0}, {"UserLocation30", 25241508.0}, {"crop2", 9375944.8}, 
                                          {"crop5", 63255518.0}, {"crop10", 240820500.0}, {"crop20", 842550656.0}, 
                                          {"crop30", 1795557648.0}, {"Twitter2", 44793600.0}, 
                                          {"Twitter5", 226301000.0}, {"Twitter10", 484097500.0}, {"Twitter20", 1700757000.0}, 
                                          {"Twitter30", 2869590000.0}, 
                                          {"birch2", 2680026.8}, {"birch5", 15000150.0}, {"birch10", 40400404.0}, 
                                          {"birch20", 61200612.0}, {"birch30", 67200672.0} };
       
       map<string, float> km_time = { {"Breastcancer2", 0.0012824}, {"Breastcancer5", 0.000995}, {"Breastcancer10", 0.0015736}, 
                                          {"Breastcancer20", 0.0080018}, {"Breastcancer30", 0.013569999999999999}, 
                                          {"CustomerSaleRecords2", 0.00044140000000000005}, {"CustomerSaleRecords5", 0.0010918}, 
                                          {"CustomerSaleRecords10", 0.0013344000000000001}, {"CustomerSaleRecords20", 0.0024006}, 
                                          {"CustomerSaleRecords30", 0.004408400000000001}, {"CreditRisk2", 0.0007026000000000001}, 
                                          {"CreditRisk5", 0.0020488}, {"CreditRisk10", 0.0059566}, {"CreditRisk20", 0.018805}, 
                                          {"CreditRisk30", 0.012767200000000001}, {"Census2", 0.11894680000000002}, {"Census5", 0.1415328}, 
                                          {"Census10", 0.33019099999999996}, {"Census20", 1.3884394000000002}, {"Census30", 1.7986269999999998}, 
                                          {"UserLocation2", 0.1878116}, {"UserLocation5", 0.7567109999999999}, {"UserLocation10", 1.6131564}, 
                                          {"UserLocation20", 4.475524000000001}, {"UserLocation30", 8.0982898}, {"crop2", 0.23463700000000004}, 
                                          {"crop5", 1.404347}, {"crop10", 4.286879999999999}, {"crop20", 13.285787}, {"crop30", 27.6544752}, 
                                          {"Twitter2", 3.8534382000000003}, {"Twitter5", 13.4850028}, {"Twitter10", 25.026217600000003}, 
                                          {"Twitter20", 81.72732719999999}, {"Twitter30", 138.5618784}, {"birch2", 0.0558238}, 
                                          {"birch5", 0.22413600000000003}, {"birch10", 0.5266590000000001}, 
                                          {"birch20", 0.7778386}, {"birch30", 0.8218354} };

       output_data res;
       string inputfilePath = "", centroidFilePath = "";

       std::vector<vector <float> > dataset;
       vector<int> labels;

       ofstream avgresFile;
       string outFile = out_path + "benchmark_out.csv" ;
       
       avgresFile.open(outFile, ios::out);
       avgresFile << "Algorithm,Data,Clusters,Iters,Runtime,Runtime_per_Iter,Runtime_speedup,Distances,Dist_speed_up";
       avgresFile.close();

       
       for(int i=0; i<=file_list.size(); i++){
              
              cout << "Processing " << file_list[i] << endl;

              inputfilePath = input_path + file_list[i];

              std::pair<int, int> p = readSimulatedData(inputfilePath, dataset, labels, false, false);

              int numRows = p.first;
              int numCols = p.second;

              for (int j = 0; j< num_clusters.size(); j++){
            
                    int clus = num_clusters[j];
                    vector<vector<float> > centroids(clus, vector<float>(numCols, 0.0));

                    vector<float> iters(num_rep, 0);
                    vector<float> runTime(num_rep, 0);
                    vector<float> runTime_per_iter(num_rep, 0);
                    vector<float> dist_calcs(num_rep, 0);

                    vector<float> ball_iters(num_rep, 0);
                    vector<float> ball_runTime(num_rep, 0);
                    vector<float> ball_runTime_per_iter(num_rep, 0);
                    vector<float> ball_dist_calcs(num_rep, 0);

                     for (int rep = 0 ; rep < num_rep ; rep++){
                            
                            // Read centroids  
                            centroidFilePath = centroid_path + out_list[i] + "_" + to_string(clus) + "_" + to_string(rep) +".txt";
                            read_kplus_plus_centroids(centroidFilePath, centroids, clus);
                            
                            cout << "Algo: DCKM" << endl; 
                            auto t5 = std::chrono::high_resolution_clock::now();
                            res = dckmeans(dataset, centroids, clus, threshold, num_iters, numCols);
                            auto t6 = std::chrono::high_resolution_clock::now();
                            auto kmdc_time = std::chrono::duration_cast<std::chrono::seconds>(t6 - t5);
                            
                            std::cout << "Total DCKmeans time: " << ms_int2.count() << " seconds\n";
                            
                            iters[rep] = res.loop_counter;
                            runTime[rep] = kmdc_time.count();
                            runTime_per_iter[rep] = float(kmdc_time.count()/res.loop_counter);
                            dist_calcs[rep] = res.num_he;

                            
                            cout << "Algo: Ball-Kmeans" << endl;
                            
                            // Load data in Eigen format for Ball KMeans
                            MatrixOur BallK_dataset = load_data(inputfilePath);
                            MatrixOur ballKm_centroids = load_centroids(centroidFilePath, clus, numCols);

                            auto t7 = std::chrono::high_resolution_clock::now();
                            res = ball_k_means_Ring(BallK_dataset, ballKm_centroids, true, threshold, num_iterations);
                            auto t8 = std::chrono::high_resolution_clock::now();
                            auto ball_time = std::chrono::duration_cast<std::chrono::seconds>(t8 - t7);
                            
                            std::cout << "Total DCKmeans time: " << ms_int2.count() << " seconds\n";
                            
                            ball_iters[rep] = res.loop_counter;
                            ball_runTime[rep] = ball_time.count();
                            ball_runTime_per_iter[rep] = float(ball_time.count()/res.loop_counter);
                            ball_dist_calcs[rep] = res.num_he;

                    }

                     // Find average stats
                     for (int u = 1; u < runTime.size(); u++){
                          iters[0] += iters[u] ;
                          runTime[0] += runTime[u] ;
                          runTime_per_iter[0] += runTime_per_iter[u];
                          dist_calcs[0] += dist_calcs[u];

                          ball_iters[0] += ball_iters[u] ;
                          ball_runTime[0] += ball_runTime[u] ;
                          ball_runTime_per_iter[0] += ball_runTime_per_iter[u];
                          ball_dist_calcs[0] += ball_dist_calcs[u];
                    }

                    iters[0] = iters[0]/num_rep;
                    runTime[0] = runTime[0]/num_rep;
                    runTime_per_iter[0] = runTime_per_iter[0]/num_rep;
                    dist_calcs[0] = dist_calcs[0]/num_rep;

                    ball_iters[0] = ball_iters[0]/num_rep;
                    ball_runTime[0] = ball_runTime[0]/num_rep;
                    ball_runTime_per_iter[0] = ball_runTime_per_iter[0]/num_rep;
                    ball_dist_calcs[0] = ball_dist_calcs[0]/num_rep;

                    // Write to the full output
                    // Algorithm,Data,Clusters,Iters,Runtime,Runtime_per_iter,Distances,Dist_speed_up

                    avgresFile.open(outFile, ios::app);

                    avgresFile << "\nDataCentric-Kmeans" << "," << data_list[i] << "," << to_string(clus) 
                    << "," << std::setprecision(2) << to_string(iters[0]) <<  "," << 
                    std::setprecision(2) << to_string(runTime[0]) << "," << std::setprecision(6) <<
                    to_string(runTime_per_iter[0]) << "," << std::setprecision(2) << to_string(km_time[data_list[i]+to_string(clus)]/runTime[0])
                        << "," << std::setprecision(2) << to_string(dist_calcs[0]) <<
                    "," << std::setprecision(2) << to_string(km_dist[data_list[i]+to_string(clus)]/dist_calcs[0]);

                    avgresFile << "\nBall-Kmeans" << "," << data_list[i] << "," << to_string(clus) 
                    << "," << std::setprecision(2) << to_string(ball_iters[0]) <<  "," << 
                    std::setprecision(2) << to_string(ball_runTime[0]) << "," << std::setprecision(6) <<
                    to_string(ball_runTime_per_iter[0]) << "," << std::setprecision(2) << to_string(km_time[data_list[i]+to_string(clus)]/ball_runTime[0])
                        << "," << std::setprecision(2) << to_string(ball_dist_calcs[0]) <<
                    "," << std::setprecision(2) << to_string(km_dist[data_list[i]+to_string(clus)]/ball_dist_calcs[0]);    

                    avgresFile.close()
            }

       }

       cout << "Finished 3rd suit of benchmarks" << endl;
       return 0;
 }
