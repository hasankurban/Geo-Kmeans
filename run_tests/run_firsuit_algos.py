import subprocess
import numpy as np
import time


basePath = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/real_data/"
program_basePath = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/Box Sync/PhD/DATASETS/real_data/experiment_data/programs/mlpack_kmeans"

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
algorithms = ["naive", "hamerly", "pelleg-moore", "dualtree"]
num_rep = 5
num_iters = 500


# num_clusters = [20]
algorithms = ["naive"]
# num_rep = 5
# num_iters = 500

# file_list = ["birch.csv"]
# data_list = ["birch"]
# out_list = ["birchCentroids"]


def run_algos():

    with open(out_path+"Avg_output.csv", "w") as output_file:
        output_file.write("Algorithm,Data,Clusters,Iters,Runtime,Runtime_per_Iter,Runtime_speedup,Distances,Dist_speed_up\n")

        km_runtime = {}
        km_dist = {}
        full_output = []

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
                    temp456 = [alg_out, data_list[i], str(clus)]
                    dist_calc = 0
                    runTime = 0
                    runTime_iter = 0
                
                    
                    for line in collected_output:
                        
                        line = line.strip()

                        if "converged after" in line:
                            # print(alg_out, line)
                            iters = float(line.split(" ")[5].strip())
                            iteration_counter.append(iters)

                        else:
                            iters = num_iters
                            iteration_counter.append(iters)
                        
                        if "distance calculations" in line:
                            dist_calc = line.split(" ")[2].strip()
                            dist_calc = float(dist_calc[4:])
                            dist_counter.append(dist_calc)

                        if "clustering" in line:
                            runTime = line.split(":")[1].strip()
                            if "mins" in runTime:
                                runTime = runTime.split("(")[0].strip()
                            runTime = float(runTime[0:len(runTime)-1])
                            runtime_Counter.append(runTime)

                            if iters == 0:
                                iters = num_iters
                            runTime_iter = runTime/iters
                            runtime_per_iteration.append(runTime_iter)
                            
                            
                    temp456.append(str(iters))
                    temp456.append(str(runTime))
                    temp456.append(str(runTime_iter))
                    temp456.append(str(dist_calc))
                    full_output.append(temp456)

                # print(alg_out, dist_counter, runtime_Counter, iteration_counter, runtime_per_iteration)
                with open(out_path+"Avg_output.csv", "a") as output_file:

                    # Algorithm,Data,Clusters,Iters,Runtime,Runtime_per_iter,Distances,Dist_speed_up
                    
                    if alg_out == "Lloyd":
                        km_dist[data_list[i]+str(clus)] = np.mean(dist_counter)
                        km_runtime[data_list[i]+str(clus)] = np.mean(runtime_Counter)

                        output_file.write(alg_out + "," + data_list[i] + "," + str(clus) + "," + 
                        str("{:.2f}".format(np.mean(iteration_counter))) + "," + str("{:.2f}".format(np.mean(runtime_Counter))) + "," +
                        str("{:.6f}".format(np.mean(runtime_per_iteration))) + ",0," + str("{:.2f}".format(np.mean(dist_counter))) + ",0" + "\n")
                    
                    else:

                        output_file.write(alg_out + "," + data_list[i] + ","+ str(clus) + "," + 
                        str("{:.2f}".format(np.mean(iteration_counter))) + "," + str("{:.2f}".format(np.mean(runtime_Counter))) + "," +
                        str("{:.6f}".format(np.mean(runtime_per_iteration))) + "," + str("{:.4f}".format((km_runtime[data_list[i]+str(clus)]/np.mean(runtime_Counter)))) + 
                        "," + str("{:.2f}".format(np.mean(dist_counter))) + "," 
                        + str("{:.2f}".format((km_dist[data_list[i]+str(clus)]/np.mean(dist_counter)))) + "\n")

                time.sleep(1)

    
    with open("Full_output.csv", "w") as full_file:
        full_file.write("Algorithm,Data,Clusters,Iters,Runtime,Runtime_per_Iter,Distances\n")
        for entry in full_output:
            out = ",".join(entry)
            out += "\n"
            full_file.write(out)
    
    return km_dist, km_runtime