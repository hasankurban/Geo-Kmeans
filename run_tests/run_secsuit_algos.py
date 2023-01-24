import subprocess
import numpy as np
import time


basePath = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/real_data/"
program_basePath = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/real_data/experiment_data/programs/blaslesskmeans"

data_basePath = basePath + "experiment_data/comma_seperated_files/";
centroid_basePath = basePath + "experiment_data/comma_seperated_centroids/";
out_path = basePath + "experiment_data/"

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
algorithms = ["exp-ns"]
num_rep = 5
num_iters = 500


# num_clusters = [2, 5, 10, 20, 30]
# # algorithms = ["ann"]
# num_rep = 5
# num_iters = 500

# file_list = ["Breastcancer.csv"]
# data_list = ["Breastcancer"]
# out_list = ["BreastcancerCentroids"]


def run_sec_algos(km_dist, km_runtime):

    # with open(out_path+"Avg_output.csv", "w") as output_file:
    #     output_file.write("Algorithm,Data,Clusters,Iters,Runtime,Runtime_per_Iter,Runtime_speedup,Distances,Dist_speed_up\n")

    full_output = []

    for alg in algorithms:

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
                    # print(alg)
                    
                    args = [program_basePath, "-din", read_file_path, "-cin", read_centroid_path, 
                    "-alg", alg, "-mi", str(num_iters), "-nc", str(clus), "-cver", str(1) , "-nth", str(1), "-see", str(29)]


                    popen = subprocess.Popen(args, stdout=subprocess.PIPE, encoding='ASCII')
                    collected_output = popen.communicate()[0]

                    # print(collected_output)

                    collected_output = collected_output.split("\n")
                    temp456 = [alg_out, data_list[i], str(clus)]
                    dist_calc = 0
                    runTime = 0
                    runTime_iter = 0
                
                    
                    for line in collected_output:
                        
                        line = line.strip()

                        if "rounds" in line:
                            # print(line)
                            line = line.split("\t")

                            iters = float(line[0].strip().split(':')[1].strip())
                            dist_calc = float(line[3].strip().split(":")[1].strip())
                            runTime = float(line[4].strip().split(":")[1].strip().split(" ")[0].strip())
                            runTime_iter = runTime/iters

                            iteration_counter.append(iters)
                            dist_counter.append(dist_calc)
                            runtime_Counter.append(runTime)
                            runtime_per_iteration.append(runTime_iter)

                    
                    temp456.append(str(iters))
                    temp456.append(str(runTime))
                    temp456.append(str(runTime_iter))
                    temp456.append(str(dist_calc))
                    full_output.append(temp456)


                with open(out_path+"Avg_output.csv", "a") as output_file:
                    
                    if np.mean(runtime_Counter) == 0:
                        for q in range(len(runtime_Counter)):
                            runtime_Counter[q] = 1

                    time_speed_up = "{:.4f}".format(km_runtime[data_list[i]+str(clus)]/np.mean(runtime_Counter))

                    output_file.write(alg_out + "," + data_list[i] + ","+ str(clus) + "," + 
                    str("{:.2f}".format(np.mean(iteration_counter))) + "," + str("{:.2f}".format(np.mean(runtime_Counter))) + "," +
                    str("{:.6f}".format(np.mean(runtime_per_iteration))) + "," + str(time_speed_up) + 
                    "," + str("{:.2f}".format(np.mean(dist_counter))) + "," 
                    + str("{:.2f}".format((km_dist[data_list[i]+str(clus)]/np.mean(dist_counter)))) + "\n")

                
                time.sleep(5)

    
    with open("Full_output.csv", "a") as full_file:
        for entry in full_output:
            out = ",".join(entry)
            out += "\n"
            full_file.write(out)



