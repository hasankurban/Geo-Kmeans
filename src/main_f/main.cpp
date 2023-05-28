// #include <iostream>
// #include <string>
// #include <tuple>
// #include "data_holder.hpp"
// #include "IOutils.hpp"
// #include "algo_utils.hpp"
// #include "kmeans.hpp"
// #include "dckmeans.hpp"
// #include <chrono>
// // #include "mlpack.hpp"

// #include <filesystem>

// using namespace std;
// // using namespace mlpack;
// // namespace fs = std::filesystem;

// //  string basePath = "/nobackup/parishar/DATASETS/";
// string basePath = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/PhD/DATASETS/";
// // string basePath = "/Users/schmuck/Documents/Box Sync/PhD/Data-Centric-KMeans/";

// int main(){

//     // Experiment specific path
//     // string filePath = basePath + "clustering_data/";
//     // string filePath = basePath + "dims_data/";
//     // string filePath = basePath + "scal_data/";
// //    string filePath = basePath + "sample_data/";
//      basePath = basePath + "real_data/";
    
// //    string fileName = "50_2_10.csv";
//     // string fileName = "crop.csv";
//     // string somefilePath = filePath + fileName;
    
//     // string centroidFilePath = filePath + "cropCentroids_30.txt";
//     string somefilePath = "";


//     std::vector<vector <float> > dataset;
//     vector<int> labels;

//     // cout << dataset.max_size() << endl;
    
//     // Declare variables
//     int num_iterations = 500;
//     float threshold = 0.001;

//     // vector<string> file_list = {"fourclass.csv", "codrna.csv", "Breastcancer.csv",
//     // "crop.csv", "Census.csv"};

//     // vector<string> out_list = {"fourclassCentroids", "codrnaCentroids", "BreastcancerCentroids",
//     // "cropCentroids", "CensusCentroids"};


//     vector<string> file_list = {"deng.csv"};
//     vector<string> out_list = {"deng"};
//     int num_clusters = 15;
//     int seed = 21;

//     somefilePath =  basePath + "deng.csv";

//     cout << somefilePath << endl;

//     // Read in the data
//     cout << somefilePath << endl;
//     auto t1 = std::chrono::high_resolution_clock::now();
//     std::pair<int, int> p = readSimulatedData(somefilePath, dataset, labels, false, false);
//     auto t2 = std::chrono::high_resolution_clock::now();
//     auto file_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    
//     cout << "File reading: " << file_int.count() << " MS\n";
//     cout << "Data size: " << dataset.size() << " X " << dataset[0].size() << endl;
    
//     int numRows = p.first;
//     int numCols = p.second;
    

//     output_data res;
     

//     // Debug - Testing
//     cout << "\nAlgo: KMeans," << " Clusters: " << num_clusters << ", Threshold: " << threshold << endl;
//     auto t3 = std::chrono::high_resolution_clock::now();
//     res = kmeans(dataset, num_clusters, threshold, num_iterations, numCols, 60000, "random", seed);
//     auto t4 = std::chrono::high_resolution_clock::now();
//     auto km_int = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3);
//     std::cout << "Total KMeans time: " << km_int.count() << " milliseconds\n";


//     cout << "\nAlgo: DCKM," << " Clusters: " << num_clusters << ", Threshold: " << threshold << endl;
//     auto t5 = std::chrono::high_resolution_clock::now();
//     res = dckmeans(dataset, num_clusters, threshold, num_iterations, numCols, 60000, "random", seed);
//     auto t6 = std::chrono::high_resolution_clock::now();
//     auto ms_int2 = std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5);
//     std::cout << "Total DCKmeans time: " << ms_int2.count() << " milliseconds\n";

    

//     // for(int i=2; i<=20; i++){

//     //     num_clusters = i;
//     //     vector<vector<float> > km_centers(num_clusters, vector<float>(numCols, 0.0));
//     //     vector<int> km_assign(dataset.size());
//     //     output_data res;

//     //     cout << "\nAlgo: DCKM," << " Clusters: " << num_clusters << ", Threshold: " << threshold << endl;
//     //     auto t5 = std::chrono::high_resolution_clock::now();
//     //     res = dckmeans(dataset, num_clusters, threshold, num_iterations, numCols);
//     //     auto t6 = std::chrono::high_resolution_clock::now();
//     //     auto ms_int2 = std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5);
//     //     std::cout << "Total DCKmeans time: " << ms_int2.count() << " milliseconds\n";

//     //     cout << "\nAlgo: KMeans," << " Clusters: " << num_clusters << ", Threshold: " << threshold << endl;
//     //     auto t3 = std::chrono::high_resolution_clock::now();
//     //     km_iter = kmeans(dataset, num_clusters, threshold, num_iterations, 
//     //     numRows, numCols, km_centers, km_assign);
//     //     auto t4 = std::chrono::high_resolution_clock::now();
//     //     auto km_int = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3);
//     //     std::cout << "Total KMeans time: " << km_int.count() << " milliseconds\n";

//     // }

// return 0;
// }
