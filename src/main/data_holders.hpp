#include <iostream>
#include <vector>

using namespace std;

struct kmeans_output{
    vector<vector<double> > * new_centroids;
    vector<int> * assigned_clusters;
    int loop_counter;
};