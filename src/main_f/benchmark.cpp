#include <iostream>
#include <string>
#include "benchmark_real_data.hpp"


using namespace std;

int main(){

    string input_path = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/real_data/experiment_data/";
    string output_path = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/real_data/experiment_data/output_data/";

    // Declare variables
    int num_iterations = 100;
    float threshold = 0.001;

    run_clustering_benchmark(threshold,  num_iterations, input_path, output_path);

    return 0;

}
