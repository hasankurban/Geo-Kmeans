from sklearn.cluster import kmeans_plusplus
import pandas as pd
import numpy as np


basePath = "/Users/schmuck/Documents/Box Sync/PhD/DATASETS/real_data/experiment_data/comma_seperated_files/"
out_path = basePath

file_list = ["Breastcancer.csv", "CreditRisk.csv",
            "census.csv", "crop.csv", "Twitter.csv", "birch.csv"]

out_list = ["BreastcancerCentroids", "CreditRiskCentroids", 
            "CensusCentroids", "cropCentroids", "TwitterCentroids", 
            "birchCentroids"]


np.random.seed(50473)
seeds = np.random.choice(100, 50, replace=False).reshape(5, 10)
num_rep = 10

max_centers = [5, 8, 12, 15, 20]


for i in range(len(file_list)):

    file = file_list[i]    
    path = basePath + file
    data = np.array(pd.read_csv(path, low_memory=False), dtype="float32")
    print(file)

    for clus_index in range(len(max_centers)):

        clus = max_centers[clus_index]

        for rep in range(num_rep):

            centers_init, _ = kmeans_plusplus(data, n_clusters=clus, random_state=seeds[clus_index, rep])
            centers_init = pd.DataFrame(centers_init)

            centers_init.to_csv(out_path+out_list[i]+"_" + str(clus) + "_" + str(rep) + "_.txt", sep=",", index=False, header=False)
        
        # centers_init.to_csv(output_path+"space_seperated_centroids/"+out_list[i]+"_" + str(clus) + "_.txt", sep=" ", index=False, header=False)

            # centers_init.to_csv(out_path+out_list[i] + "_" + str(clus) + "_" + str(rep) + ".txt", sep=",", index=False, header=False)
            # centers_init.to_csv(out_path_space+out_list[i] + "_" + str(clus) + "_" + str(rep) + ".txt", sep=" ", index=False, header=False)