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

using namespace std;

int main(){

    string basePath = "/u/parishar/scratch/DATASETS/real_data/";

    double_prop(basePath);
    double_clusters(basePath);

    return 0;
}