from sklearn.cluster import kmeans_plusplus
import pandas as pd
import numpy as np


basePath = "/u/parishar/scratch/DATASETS/real_data/experiment_data/comma_seperated_files/"
out_path = basePath

file_list = ["Breastcancer.csv", "CustomerSaleRecords.csv", "CreditRisk.csv",
            "magic.csv", "spambase.csv", "crop.csv", "Twitter.csv", "birch.csv"]

out_list = ["BreastcancerCentroids", "CustomerSaleRecordsCentroids", "CreditRiskCentroids", 
            "magicCentroids", "spambaseCentroids", "cropCentroids", "TwitterCentroids", 
            "birchCentroids"]


np.random.seed(741025369)
seeds = np.random.choice(100, 40, replace=False).reshape(8, 5)
num_rep = 1

max_centers = [5, 8, 10, 12, 25]


for i in range(len(file_list)):

    file = file_list[i]    
    path = basePath + file
    data = np.array(pd.read_csv(path, low_memory=False), dtype="float32")
    print(file)

    for clus in max_centers:

        for rep in range(num_rep):

            centers_init, _ = kmeans_plusplus(data, n_clusters=clus, random_state=seeds[i, rep])
            centers_init = pd.DataFrame(centers_init)

        centers_init.to_csv(out_path+out_list[i]+"_" + str(clus) + "_.txt", sep=",", index=False, header=False)
        
        # centers_init.to_csv(output_path+"space_seperated_centroids/"+out_list[i]+"_" + str(clus) + "_.txt", sep=" ", index=False, header=False)

            # centers_init.to_csv(out_path+out_list[i] + "_" + str(clus) + "_" + str(rep) + ".txt", sep=",", index=False, header=False)
            # centers_init.to_csv(out_path_space+out_list[i] + "_" + str(clus) + "_" + str(rep) + ".txt", sep=" ", index=False, header=False)