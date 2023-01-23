from sklearn.cluster import kmeans_plusplus
import pandas as pd
import numpy as np

input_path = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/real_data/experiment_data/";
output_path = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/real_data/experiment_data/";


file_list = ["Breastcancer.csv", "CustomerSaleRecords.csv", "CreditRisk.csv",
            "Census.csv", "UserLocation.csv",
            "crop.csv", "Twitter.csv", "birch.csv"]

out_list = ["BreastcancerCentroids", "CustomerSaleRecordsCentroids", "CreditRiskCentroids", 
            "CensusCentroids", "UserLocationCentroids",
            "cropCentroids", "TwitterCentroids", "birchCentroids"]

np.random.seed(25)
seeds = np.random.randint(45, size=(9, 5))

# file_list = ["codrna.csv"]
# out_list = ["codrnaCentroids"]
# seeds = [856]

max_centers = [2, 5, 10, 20, 30]


for i in range(len(file_list)):

    file = file_list[i]    
    path = input_path + file
    data = np.array(pd.read_csv(path, low_memory=False), dtype="float32")
    print(file)

    for clus in max_centers:

        for rep in range(5):

            centers_init, _ = kmeans_plusplus(data, n_clusters=clus, random_state=seeds[i, rep])
            centers_init = pd.DataFrame(centers_init)
            
            centers_init.to_csv(output_path+"comma_seperated_centroids/"+out_list[i]+"_" + str(clus) + "_" + str(rep) + ".txt", sep=",", index=False, header=False)
            
            centers_init.to_csv(output_path+"space_seperated_centroids/"+out_list[i]+"_" + str(clus) + "_" + str(rep) + ".txt", sep=" ", index=False, header=False)