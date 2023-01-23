import subprocess
import numpy as np
import time


data_basePath = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/real_data/experiment_data/comma_seperated_files/";
centroid_basePath = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/real_data/experiment_data/comma_seperated_centroids/";
out_path = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/real_data/experiment_data/"
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
algorithms = ["naive", "hamerly", "pelleg-moore", "dualtree"]
num_rep = 5
num_iters = 500


# num_clusters = [2, 5, 10, 20, 30]
# algorithms = ["naive", "hamerly"]
# num_rep = 5
# num_iters = 500

# file_list = ["Breastcancer.csv"]
# data_list = ["Breastcancer"]
# out_list = ["BreastcancerCentroids"]


def run_algos():

    with open(out_path+"Avg_output.csv", "w") as output_file:
        output_file.write("Algorithm,Data,Clusters,Iters,Runtime,Runtime_per_Iter,Runtime_speedup,Distances,Dist_speed_up\n")


        km_runtime = {}
        km_dist = {}

    for alg in algorithms:

        print("Algorithm: ", alg)
        if alg == "naive":
            alg_out = "Lloyd"
        else:
            alg_out = alg

        for i in range(len(file_list)):

            read_file_path = data_basePath + file_list[i]
            
            print("Processing: ", data_list[i])

            for clus in num_clusters:

                dist_counter = []
                iteration_counter = []
                runtime_Counter = []
                runtime_per_iteration = []
                iters = 0

                for rep in range(num_rep):
            
                    read_centroid_path = centroid_basePath + out_list[i] +"_" + str(clus) + "_" + str(rep) +".txt"

                    # print("Data: ", read_file_path)
                    # print("Centroid: ", read_centroid_path)
                    
                    args = [program_basePath, "--input_file", read_file_path, "--initial_centroids_file", read_centroid_path, 
                    "-a", alg, "-m", str(num_iters), "--clusters", str(clus), "--verbose"]
                    
                    popen = subprocess.Popen(args, stdout=subprocess.PIPE, encoding='ASCII')
                    collected_output = popen.communicate()[0]

                    # print(collected_output)

                    collected_output = collected_output.split("\n")
                
                    
                    for line in collected_output:
                        
                        line = line.strip()

                        if "converged" in line:
                            iters = float(line.split(" ")[5].strip())
                            iteration_counter.append(iters)
                        
                        elif "distance calculations" in line:
                            d = line.split(" ")[2].strip()
                            dist_counter.append(float(d[4:]))

                        elif "clustering" in line:
                            d = line.split(":")[1].strip()
                            d = float(d[0:len(d)-1])
                            runtime_Counter.append(d)
                            runtime_per_iteration.append(d/iters)

                with open(out_path+"Avg_output.csv", "a") as output_file:

                    # Algorithm,Data,Clusters,Iters,Runtime,Runtime_per_iter,Distances,Dist_speed_up
                    
                    if alg_out == "Lloyd":
                        km_dist[clus] = np.mean(dist_counter)
                        km_runtime[clus] = np.mean(runtime_Counter)

                        output_file.write(alg_out + "," + data_list[i] + "," + str(clus) + "," + 
                        str("{:.6f}".format(np.mean(iteration_counter))) + "," + str("{:.6f}".format(np.mean(runtime_Counter))) + "," +
                        str("{:.6f}".format(np.mean(runtime_per_iteration))) + ",0," + str("{:.6f}".format(np.mean(dist_counter))) + ",0" + "\n")
                    
                    else:

                        # print(alg_out + "," + data_list[i] + ","+ str(clus) + "," + 
                        # str(np.mean(iteration_counter)) + "," + str(np.mean(runtime_Counter)) + "," +
                        # str(np.mean(runtime_per_iteration)) + "," + str(np.mean(dist_counter)) + "," 
                        # + str((km_dist[clus]/np.mean(dist_counter))))

                        temp1 = np.format_float_positional(np.mean(runtime_Counter), trim='-')
                        temp2 = np.format_float_positional(np.mean(runtime_per_iteration), trim='-')

                        output_file.write(alg_out + "," + data_list[i] + ","+ str(clus) + "," + 
                        str("{:.2f}".format(np.mean(iteration_counter))) + "," + str("{:.2f}".format(np.mean(runtime_Counter))) + "," +
                        str("{:.6f}".format(np.mean(runtime_per_iteration))) + "," + str("{:.4f}".format((km_runtime[clus]/np.mean(runtime_Counter)))) + 
                        "," + str("{:.2f}".format(np.mean(dist_counter))) + "," 
                        + str("{:.2f}".format((km_dist[clus]/np.mean(dist_counter)))) + "\n")

                time.sleep(3)

    return km_dist, km_runtime