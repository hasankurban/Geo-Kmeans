#include <iostream>
#include <vector>

using namespace std;

struct neighbor_holder{
    double dist = 0.0;
    vector<int> my_neighbors;
    vector<vector<vector <double> > > mid_points;
    vector<vector<vector <double> > > affine;
};


