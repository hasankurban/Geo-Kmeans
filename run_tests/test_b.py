import subprocess
import numpy as np
from run_firsuit_algos import run_algos
from run_secsuit_algos import run_sec_algos
import json

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

# Save the stats
with open("run_stat.txt", "w") as file:
    file.write(json.dumps(km_runtime))

with open("dist_stat.txt", "w") as file:
    file.write(json.dumps(km_dist))

print("Starting 2nd suit benchmark\n")
run_sec_algos(km_dist, km_runtime)
print("\nCompleted the 2nd suit benchmark")


# algorithms = ["ball-Kmeans", "KMDataCentric"]

                



