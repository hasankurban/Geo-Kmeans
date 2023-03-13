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

basePath = "/u/parishar/scratch/DATASETS/real_data/"


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


       int num_iters = 500;
       float threshold = 0;
       int num_rep = 5;     
       
       vector<string> file_list = {"Breastcancer.csv"};
       vector<string> out_list = {"BreastcancerCentroids"};
       vector<string> data_list = {"Breastcancer"};
       vector<int> num_clusters = {5, 8, 10, 12, 25};

       map<string, float> km_dist = { {"BreastcancerCentroids5", 33630.0}, {"BreastcancerCentroids8", 54720.0}, 
       {"BreastcancerCentroids10", 66120.0}, {"BreastcancerCentroids12", 103968.0}, {"BreastcancerCentroids25", 193800.0}, 
       {"CustomerSaleRecordsCentroids5", 26901.0}, {"CustomerSaleRecordsCentroids8", 35280.0}, 
       {"CustomerSaleRecordsCentroids10", 52038.0}, {"CustomerSaleRecordsCentroids12", 61387.2}, 
       {"CustomerSaleRecordsCentroids25", 116865.0}, {"CreditRiskCentroids5", 51051.0}, 
       {"CreditRiskCentroids8", 96096.0}, {"CreditRiskCentroids10", 116116.0}, 
       {"CreditRiskCentroids12", 156156.0}, {"CreditRiskCentroids25", 390390.0}, 
       {"magicCentroids5", 2491751.0}, {"magicCentroids8", 3743332.8}, 
       {"magicCentroids10", 15292884.0}, {"magicCentroids12", 12416908.8}, 
       {"magicCentroids25", 47932920.0}, {"spambaseCentroids5", 519913.0}, 
       {"spambaseCentroids8", 1037985.6}, {"spambaseCentroids10", 1021422.0}, 
       {"spambaseCentroids12", 1181536.8}, {"spambaseCentroids25", 2898630.0}, 
       {"cropCentroids5", 53943792.0, {"cropCentroids8", 105318832.0}, 
       {"cropCentroids10", 227334552.0}, {"cropCentroids12", 237352684.8}, 
       {"cropCentroids25", 849293630.0}, {"TwitterCentroids5", 155727750.0}, 
       {"TwitterCentroids8", 291158400.0}, 
       {"TwitterCentroids10", 549421500.0}, {"TwitterCentroids12", 1061048400.0}, 
       {"TwitterCentroids25", 2286340000.0}, 
       {"birchCentroids5", 11700117.0}, {"birchCentroids8", 23840238.4}, {"birchCentroids10", 36000360.0}, 
       {"birchCentroids12", 45360453.6}, {"birchCentroids25", 56500565.0} };
       
       map<string, float> km_time = { {"BreastcancerCentroids5", 0.0022714}, {"BreastcancerCentroids8", 0.0036204000000000006}, 
       {"BreastcancerCentroids10": 0.0041426}, {"BreastcancerCentroids12", 0.0064908}, {"BreastcancerCentroids25": 0.009760000000000001}, 
       {"CustomerSaleRecordsCentroids5", 0.0016836}, "CustomerSaleRecordsCentroids8", 0.0019582}, 
       {"CustomerSaleRecordsCentroids10", 0.0029888000000000002}, {"CustomerSaleRecordsCentroids12", 0.0034437999999999995}, 
       {"CustomerSaleRecordsCentroids25", 0.005547999999999999, {"CreditRiskCentroids5", 0.0034302000000000004}, 
       {"CreditRiskCentroids8", 0.005541}, {"CreditRiskCentroids10": 0.006056199999999999}, 
       {"CreditRiskCentroids12", 0.0083048}, {"CreditRiskCentroids25", 0.016946}, {"magicCentroids5", 0.05024459999999999}, 
       {"magicCentroids8", 0.07038839999999999}, {"magicCentroids10", 0.2907966}, {"magicCentroids12", 0.28427260000000004}, 
       {"magicCentroids25", 0.8554876}, {"spambaseCentroids5", 0.02707}, {"spambaseCentroids8", 0.050457600000000005}, 
       {"spambaseCentroids10", 0.0487794}, {"spambaseCentroids12", 0.0551296}, {"spambaseCentroids25", 0.13022620000000001}, 
       {"cropCentroids5", 1.0954168}, {"cropCentroids8", 1.9830820000000002}, {"cropCentroids10", 4.333607}, 
       {"cropCentroids12", 4.3945300000000005}, {"cropCentroids25", 15.018601}, {"TwitterCentroids5", 10.610926800000001}, 
       {"TwitterCentroids8", 18.0429878}, {"TwitterCentroids10", 33.672369}, {"TwitterCentroids12", 63.217110000000005}, 
       {"TwitterCentroids25", 130.6288358}, {"birchCentroids5", 0.181062, {"birchCentroids8", 0.32542340000000003, 
       {"birchCentroids10", 0.5155641999999999}, {"birchCentroids12", 0.6186592}, {"birchCentroids25": 0.7310532000000001} };

       output_data res;
       string inputfilePath = "", centroidFilePath = "";

       vector<int> labels;

       ofstream avgresFile;
       string outFile = out_path + "benchmark_packages_output.csv" ;
       
    //    avgresFile.open(outFile, ios::out);
    //    avgresFile << "Algorithm,Data,Clusters,Iters,Runtime,Runtime_per_Iter,Runtime_speedup,Distances,Dist_speed_up";
    //    avgresFile.close();

       
       for(int i=0; i<=file_list.size(); i++){
              
              cout << "Processing " << file_list[i] << endl;
              std::vector<vector <float> > dataset;

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


                    avgresFile.open(outFile, ios::app);
                    // Algorithm,Data,Clusters,Iters,Runtime,Runtime_per_Iter,Runtime_speedup,Distances,Dist_speed_up
                    
                    avgresFile << "\nDataCentric-Kmeans" << "," << data_list[i] << "," << to_string(clus) 
                    << "," << std::setprecision(2) << to_string(iters[0]) <<  "," << 
                    std::setprecision(2) << to_string(runTime[0]) << "," << std::setprecision(2) <<
                    to_string(runTime_per_iter[0]) << "," << std::setprecision(2) << to_string(km_time[data_list[i]+to_string(clus)]/runTime[0])
                        << "," << std::setprecision(2) << to_string(dist_calcs[0]) <<
                    "," << std::setprecision(2) << to_string(km_dist[data_list[i]+to_string(clus)]/dist_calcs[0]);

                    avgresFile << "\nBall-Kmeans" << "," << data_list[i] << "," << to_string(clus) 
                    << "," << std::setprecision(2) << to_string(ball_iters[0]) <<  "," << 
                    std::setprecision(2) << to_string(ball_runTime[0]) << "," << std::setprecision(2) <<
                    to_string(ball_runTime_per_iter[0]) << "," << std::setprecision(2) << to_string(km_time[data_list[i]+to_string(clus)]/ball_runTime[0])
                        << "," << std::setprecision(2) << to_string(ball_dist_calcs[0]) <<
                    "," << std::setprecision(2) << to_string(km_dist[data_list[i]+to_string(clus)]/ball_dist_calcs[0]);    

                    avgresFile.close()
            }

       }

       cout << "Finished 3rd suit of benchmarks" << endl;
       return 0;
 }