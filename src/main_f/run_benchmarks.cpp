#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
#include <vector>
#include "IOutils.hpp"
#include "algo_utils.hpp"
#include "misc_utils.hpp"
#include "data_holder.hpp"
#include "kmeans.hpp"
#include "dckmeans.hpp"
#include "ball_kmeans++_xf.hpp"
#include "doubling_proportion.hpp"
#include "doubling_clusters.hpp"
#include "benchmark_scal.hpp"
#include "benchmark_dims.hpp"
#include "benchmark_real_data.hpp"


using namespace std;

int main(int argc, char* argv[]){

    // string basePath = "/u/parishar/scratch/DATASETS/real_data/";

    string basePath = argv[1];
    string benchmark_type = argv[2];

    if (benchmark_type == "doubling_clusters"){
        double_clusters(basePath);
    }
    else if (benchmark_type == "doubling_proportion"){
        double_prop(basePath);
    }
    // else if (benchmark_type == "benchmark_real_data"){
    //     benchmark_on_real_data(basePath);
    // }
    // else if (benchmark_type == "benchmark_scal"){
    //     benchmark_scal(basePath);
    // }
    // else if (benchmark_type == "benchmark_dim"){
    //     benchmark_dims(basePath);
    // }

    return 0;
}