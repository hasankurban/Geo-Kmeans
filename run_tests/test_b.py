import subprocess
import numpy as np
from run_firsuit_algos import run_algos
from run_secsuit_algos import run_sec_algos

data_basePath = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/real_data/experiment_data/";
centroid_basePath = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/real_data/experiment_data/centroids/";
program_basePath = "/Users/schmuck/Downloads/mlpack_inst/bin/mlpack_kmeans"

file_list = ["Breastcancer.csv", "CustomerSaleRecords.csv" , "CreditRisk.csv",
            "Census.csv", "UserLocation.csv",
            "crop.csv", "Twitter.csv", "birch.csv"]

data_list = ["Breastcancer", "CustomerSaleRecords", "CreditRisk",
            "Census", "UserLocation",
            "crop", "Twitter", "birch"]

out_list = ["BreastcancerCentroids", "CustomerSaleRecordsCentroids", "CreditRiskCentroids",
            "CensusCentroids", "UserLocationCentroids",
            "cropCentroids", "TwitterCentroids", "birchCentroids"]

num_clusters = [2, 5, 10, 20, 30]
num_rep = 5
algorithms = ["naive", "hamerly", "pelleg-moore", "dualtree"]

# file_list = ["Breastcancer.csv"]
# out_list = ["BreastcancerCentroids"]

# num_clusters = [2, 5, 10]
# num_rep = 2
# algorithms = ["naive"]

# km_dist = []
# km_runtime = []

print("Starting 1st suit benchmark\n")
km_dist, km_runtime = run_algos()
print("\nCompleted the 1st suit benchmark")

# print(km_dist, km_runtime)

print("Starting 2nd suit benchmark\n")
run_sec_algos(km_dist, km_runtime)
print("\nCompleted the 2nd suit benchmark")


algorithms = ["ball-Kmeans", "KMDataCentric"]

                



