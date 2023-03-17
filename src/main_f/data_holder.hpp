#include <vector>
#include <Eigen/Dense>
using namespace std;

#pragma ONCE
struct output_data {
    int loop_counter = 0;
    int num_he = 0;
    vector<vector<float> > centroids;
    Eigen::MatrixXf ballkm_centroids;
    float runtime = 0;
    bool timeout = false;
    float sse = 0;
};
