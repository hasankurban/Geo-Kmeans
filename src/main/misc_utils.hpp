#include <iostream>
#include <vector>
#include <map>
#pragma once

using namespace std;

class print_utils{
        template <typename T> 
        void print_3d_vector(vector<vector<vector<T> > > &data, int num_records,
        string dataname);

        template <typename T>
        void print_2d_vector(vector<vector<T> > &, int, string);
        
        template <typename T> 
        void print_vector(vector<T> &, int, string);

        void print_map(map<string, vector<double> > &data, int num_records, string dataname); ;
};


template <typename T> void print_2d_vector(vector<vector<T> > &data, int num_records,
string dataname){

cout << "Printing: " << dataname << " \n" ;

int limit = 0;

for (int i =0; i< data.size(); i++){
    if (limit < num_records){
        for (int j=0; j<data[i].size(); j++)
            cout << data[i][j] << "\t";
        }
    else{
        break;
    }
        cout << "\n";
        limit++;
    } 
}


template <typename T> void print_vector(vector<T> &data, int num_records, string dataname){

cout << "Printing: " << dataname << " \n" ;
int limit = 0;

for (int i =0; i< data.size(); i++){
    if (limit < num_records){
        cout << data[i] << "\n";
        limit++;
        }
    else{
        break;
    }
    }

}


void print_map(map<string, vector<double> > &data, int num_records, string dataname){

cout << "Printing: " << dataname << " \n" ;
int limit = 0;

for(map<string, vector<double> >::iterator ii=data.begin(); ii!=data.end(); ++ii){
       cout << (*ii).first << ": ";
       vector <double> inVect = (*ii).second;
       for (unsigned j=0; j<inVect.size(); j++){
           cout << inVect[j] << " ";
       }
       cout << endl;
   }
}

  //cout << "Printing first five records" << "\n";
    // int limit = 5;
    // int i=0;
    // for(auto row: dataset){
    //     if (i < limit)
    //         for (auto col: row)
    //             cout << col << "\t" ;
    //     else
    //         break;
    //     cout << labels[i] << "\n";
    //     i++;
    // }