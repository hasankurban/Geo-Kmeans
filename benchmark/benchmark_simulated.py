import time
from glob import glob
import sys
from datetime import datetime

# Append the previous folder to import files
sys.path.append("..")
from base.DCKmeans import *
from utils.dataIO import *

# Setting up parameters
epsilon = 0.001
num_iters = 100
num_rep = 10
data_prop = 3

# Set the current directory
parent_dir = os.path.dirname(os.getcwd())

# generate the array of seeds
np.random.seed(7415)
seeds = np.array(np.random.randint(1, 10000, 25)).reshape(5, 5)


def check_directory(analysis_type):
    output_loc = os.path.join(parent_dir, analysis_type)

    if os.path.exists(output_loc) is False:
        os.mkdir(output_loc)

    else:
        curr_time = datetime.now()
        create_timestamp = str(curr_time.hour) + "_" + str(curr_time.minute) + "_" + str(curr_time.second)
        bkup_dir = os.path.join(parent_dir, "backup_" + analysis_type + create_timestamp)
        os.rename(output_loc, bkup_dir)
        os.mkdir(output_loc)


def benchmark_clusters():
    if check_directory("clustering_benchmark") is False:
        print("Error in directory creation, exiting.")
        exit(0)

    print("#####################")
    print("Clustering Experiments")
    print("#####################")

    num_clusters = [10, 50, 100, 200, 500]

    # Get files
    input_loc = os.path.join(parent_dir, "benchmark", "clustering_data")
    file_list = sorted(glob(os.path.join(input_loc, "*")))

    temp = []
    for nclus in num_clusters:
        for i in range(len(file_list)):
            if file_list[i].split("/")[-1].split(".")[0].split("_")[0] == str(nclus):
                temp.append(file_list[i])
                break

    file_list = temp

    # Output directory
    output_loc = os.path.join(parent_dir, "clustering_benchmark")
    file_counter = 0

    DEKM_results = []
    KM_results = []

    for f in file_list:

        nclus = int(f.split("/")[-1].split(".")[0].split("_")[0])

        # Load the data
        data, labels = read_simulated_data(f)

        print("Number of clusters: ", nclus)
        DEKM_temp_results = []
        KM_temp_results = []
        seed_counter = 0

        for i in range(num_rep):
            # Test DEKmeans
            start_time = time.time()
            # centroids1, iter1 = DCKmeans(data, nclus, epsilon, data_prop, num_iters,
            #                              seeds[file_counter, seed_counter])
            centroids1, iter1 = DEKmeans_frequentist(data, nclus, epsilon, data_prop, num_iters,
                                         seeds[file_counter, seed_counter])

            TraningTime1 = time.time() - start_time

            # Test KMeans
            start_time2 = time.time()
            centroids2, iter2 = Kmeans(data, nclus, epsilon, num_iters, centroids1,
                                       False, seeds[file_counter, seed_counter])
            TraningTime2 = time.time() - start_time2

            ari1 = check_ARI(pred_membership(data, centroids1), labels)
            ari2 = check_ARI(pred_membership(data, centroids2), labels)

            DEKM_temp_results += [[nclus, TraningTime1, iter1, ari1]]
            KM_temp_results += [[nclus, TraningTime2, iter2, ari2]]

            seed_counter += 1

        # Take the average of the two lists
        DEKM_temp_results = np.array(DEKM_temp_results)
        KM_temp_results = np.array(KM_temp_results)

        DEKM_temp_results = np.round(np.mean(DEKM_temp_results, axis=0), 3)
        KM_temp_results = np.round(np.mean(KM_temp_results, axis=0), 3)

        # print(DEKM_results)
        DEKM_results += [DEKM_temp_results.tolist() + ["DEKMeans"]]
        KM_results += [KM_temp_results.tolist() + ["KMeans"]]

        file_counter += 1

    result = DEKM_results + KM_results
    # print(result)
    write_result_data(result, output_loc, "clustering_results")


def benchmark_dimensions():
    if check_directory("dimensions_benchmark") is False:
        print("Error in directory creation, exiting.")
        exit(0)

    nclus = 20
    num_dims = [10, 100, 200, 400, 500]

    print("#####################")
    print("Dimensionality Experiments")
    print("#####################")

    # Get files
    input_loc = os.path.join(parent_dir, "benchmark", "dims_data")
    file_list = sorted(glob(os.path.join(input_loc, "*")))

    temp = []
    for dims in num_dims:
        for i in range(len(file_list)):
            if file_list[i].split("/")[-1].split(".")[0].split("_")[0] == str(dims):
                temp.append(file_list[i])
                break

    file_list = temp

    # Output directory
    output_loc = os.path.join(parent_dir, "dimensions_benchmark")

    file_counter = 0
    DEKM_results = []
    KM_results = []

    for f in file_list:

        dims = int(f.split("/")[-1].split(".")[0].split("_")[0])

        # Load the data
        data, labels = read_simulated_data(f)

        print("Number of dimensions: ", dims)
        DEKM_temp_results = []
        KM_temp_results = []
        seed_counter = 0

        for i in range(num_rep):
            # DEKmeans
            start_time = time.time()
            centroids1, iter1 = DCKmeans(data, nclus, epsilon, data_prop, num_iters,
                                         seeds[file_counter, seed_counter])
            TraningTime1 = time.time() - start_time

            # KMeans
            start_time2 = time.time()
            centroids2, iter2 = Kmeans(data, nclus, epsilon, num_iters, centroids1,
                                       False, seeds[file_counter, seed_counter])
            TraningTime2 = time.time() - start_time2

            ari1 = check_ARI(pred_membership(data, centroids1), labels)
            ari2 = check_ARI(pred_membership(data, centroids2), labels)

            DEKM_temp_results += [[dims, TraningTime1, iter1, ari1]]
            KM_temp_results += [[dims, TraningTime2, iter2, ari2]]

            seed_counter += 1

        # Take the average of the two lists
        DEKM_temp_results = np.array(DEKM_temp_results)
        KM_temp_results = np.array(KM_temp_results)

        DEKM_temp_results = np.round(np.mean(DEKM_temp_results, axis=0), 3)
        KM_temp_results = np.round(np.mean(KM_temp_results, axis=0), 3)

        # print(DEKM_results)
        DEKM_results += [DEKM_temp_results.tolist() + ["DEKMeans"]]
        KM_results += [KM_temp_results.tolist() + ["KMeans"]]

        file_counter += 1

    result = DEKM_results + KM_results
    # print(result)
    write_result_data(result, output_loc, "dims_results")


def benchmark_size():
    if check_directory("scalability_benchmark") is False:
        print("Error in directory creation, exiting.")
        exit(0)

    print("#####################")
    print("Scalability Experiments")
    print("#####################")

    nclus = 10
    num_points = [1000000, 3000000, 5000000, 8000000, 10000000]

    # Get files
    input_loc = os.path.join(parent_dir, "benchmark", "scal_data")
    file_list = sorted(glob(os.path.join(input_loc, "*")))

    temp = []
    for npoints in num_points:
        for i in range(len(file_list)):
            if file_list[i].split("/")[-1].split(".")[0].split("_")[0] == str(npoints):
                temp.append(file_list[i])
                break

    file_list = temp

    # Output directory
    output_loc = os.path.join(parent_dir, "scalability_benchmark")
    file_counter = 0

    DEKM_results = []
    KM_results = []

    for f in file_list:
        npoints = int(f.split("/")[-1].split(".")[0].split("_")[0])

        # Load the data
        data, labels = read_simulated_data(f)

        print("Number of datapoints: ", npoints)
        DEKM_temp_results = []
        KM_temp_results = []
        seed_counter = 0

        # Test DEKmeans
        start_time = time.time()
        centroids1, iter1 = DCKmeans(data, nclus, epsilon, data_prop, num_iters,
                                     seeds[file_counter, seed_counter])
        TraningTime1 = time.time() - start_time

        # Test KMeans
        start_time2 = time.time()
        centroids2, iter2 = Kmeans(data, nclus, epsilon, num_iters, centroids1,
                                   False, seeds[file_counter, seed_counter])
        TraningTime2 = time.time() - start_time2

        ari1 = check_ARI(pred_membership(data, centroids1), labels)
        ari2 = check_ARI(pred_membership(data, centroids2), labels)

        DEKM_results += [[npoints, TraningTime1, iter1, ari1, "DEKMeans"]]
        KM_results += [[npoints, TraningTime2, iter2, ari2, "KMeans"]]

        file_counter += 1

    result = DEKM_results + KM_results
    # print(result)
    write_result_data(result, output_loc, "scal_results")


if __name__ == "__main__":

    benchmark_type = str(sys.argv[1])

    if benchmark_type == "clus":
        print("Starting benchmark at: ", datetime.now())
        benchmark_clusters()
        print("Completed benchmark at: ", datetime.now())

    elif benchmark_type == "dims":
        print("Starting benchmark at: ", datetime.now())
        benchmark_dimensions()
        print("Completed benchmark at: ", datetime.now())

    elif benchmark_type == "scal":
        print("Starting benchmark at: ", datetime.now())
        benchmark_size()
        print("Completed benchmark at: ", datetime.now())

    elif benchmark_type == "all":
        print("Warning: Very long running job\n")
        print("Starting benchmark at: ", datetime.now())
        benchmark_clusters()
        benchmark_dimensions()
        benchmark_size()
        print("Completed benchmark at: ", datetime.now())

    else:
        print("Valid benchmark types are: clus, dims, scal, all")
        print("example: python3 benchmark_simulated.py clus")
        exit(1)
