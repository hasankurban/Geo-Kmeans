#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#pragma once

using namespace std;

class dataIO{
    public:
        template <typename Tfloat, typename Tint>
        pair<Tint, Tint> readSimulatedData(string, vector<vector<Tfloat> > &, 
        vector<Tint> &, bool label_present, bool head_present);
        
        float ** alloc_data_memory(int numRows, int numCols);
        int * alloc_label_memory(int);

        template <typename T1>
        void readCSV(string filepath, vector<vector<T1> > & );

        template <typename T1>
        void readLabels(string, vector<T1> &);
};

float ** dataIO::alloc_data_memory(int numRows, int numCols){

    float **array_pointer = 0;
    array_pointer = new float *[numRows];

    for (int i = 0; i<numRows;i++){
        array_pointer[i] = new float [numCols];
    }

    for(int i =0; i < numRows; i++){
        for(int j =0; j<numCols; j++)
            array_pointer[i][j] = 0;
    }
    return array_pointer;
}

int * dataIO::alloc_label_memory(int numRows){

    int *temp = 0;
    temp = new int[numRows];

    for (int i =0; i < numRows; i++){
        temp[i] = 0;
    }
    return temp;
}

std::string& ltrim(std::string &s)
{
    auto it = std::find_if(s.begin(), s.end(),
                    [](char c) {
                        return !std::isspace<char>(c, std::locale::classic());
                    });
    s.erase(s.begin(), it);
    return s;
}
 
std::string& rtrim(std::string &s)
{
    auto it = std::find_if(s.rbegin(), s.rend(),
                    [](char c) {
                        return !std::isspace<char>(c, std::locale::classic());
                    });
    s.erase(it.base(), s.end());
    return s;
}
 
std::string& trim(std::string &s) {
    return ltrim(rtrim(s));
}


template <typename Tfloat>
void read_centroids(string filePath, 
vector<vector <Tfloat> > &centroids){

    string value, line;

    // file stream handle
    fstream f;
    
    // open file for reading
    f.open(filePath, ios::in);

    if(f.is_open()){
        while(getline(f, line)){
                // cout << line << "\n";
                stringstream s (line);
                vector<float> row;
                
                while(getline(s, value, ',')){ 
                    
                    try{
                        value.erase(remove( value.begin(), value.end(), '\"' ),value.end()); 
                        row.push_back(std::stod(value));
                        // cout << value << " ";
                    }
                    catch(...){
                        cerr << "Exception in file reading" << "\n";
                        }
                }
                // cout << "\n";
                centroids.push_back(row);
            }
    }
    f.close();

}


template <typename Tfloat, typename Tint> pair<Tint, Tint> readSimulatedData(string filePath, 
vector<vector <Tfloat> > &dataset, vector<Tint> &labels, bool label_present, bool head_present){

    /* 
    1) Find the number of rows and columns.
    2) Declare an array with dimensions = rows * columns
    3) Return the pointer to that array
    */  

    int numRows = 0, numCols= 0, i =0, j = 0, index = -1;

    // temporary variables
    string value, line;
    
    // file stream handle
    fstream f;
    
    // open file for reading
    f.open(filePath, ios::in);

    if(f.is_open()){
        while(getline(f, line)){
            if (numRows==0){
                stringstream s (line);
                while(getline(s, value, ',')){
                        numCols++;  
                    }
                }
                else{
                    break;
                }
            numRows++;
        }
    }
    f.close();

    cout << "Rows: " << numRows << "\t Cols: " << numCols << "\n";

    if (head_present)
        index = 0;
    if (label_present)
        numCols -= 1;

    cout << "Rows: " << numRows << "\t Cols: " << numCols << "\n";
 
    f.open(filePath, ios::in);
    if(f.is_open()){
        while(getline(f, line)){
            if (i > index){
                // cout << line << "\n";
                stringstream s (line);
                vector<float> row;
                j = 0;
                while(getline(s, value, ',')){ 
                    
                    try{
                        value.erase(remove( value.begin(), value.end(), '\"' ),value.end()); 
                        
                        if (j == numCols){
                            // cout << value << "\t";
                            labels.push_back(stoi(value));
                        }
                        else{
                            // cout << value << "\t";
                            row.push_back(std::stod(value));
                        }
                        j++;
                    }
                    catch(...){
                        cerr << "Exception in file reading" << "\n";
                        }
                }
                dataset.push_back(row);
                // cout << "\n";
            } 
            i++;
            }
    }
    f.close();

    return std::make_pair(numRows, numCols);
}

// template <typename T1>
// void readCSV(string filepath, vector<vector<T1> > & output) {
//             boost::filesystem::ifstream fileHandler(filepath);
//             std::string line;
//             while (getline(fileHandler, line)) {
//                 boost::tokenizer<boost::escaped_list_separator<char>> tokenizer { line };
//                 kmeans::common::DataVector<float> row;
//                 for (const auto & token : tokenizer) {
//                     row.push_back(std::stod(token));
//                 }
//                 output.push_back(row);
//             }
//         }

// template <typename T1>
// void readLabels(std::string filepath, vector<T1> & labels) {
//             boost::filesystem::ifstream fileHandler(filepath);
//             std::string line;
//             while (getline(fileHandler, line)) {
//                 boost::tokenizer<boost::escaped_list_separator<char>> tokenizer { line };
//                 kmeans::common::DataVector<std::size_t> row;
//                 for (const auto & token : tokenizer) {
//                     row.push_back(std::stod(token));
//                 }
//                 labels.push_back(row[0]);
//     }
// }