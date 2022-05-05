import time
from glob import glob
import sys
from datetime import datetime

# Append the previous folder to import files
sys.path.append("..")

from utils.kmeans_utils import *
from base.DCKmeans import *
from utils.dataIO import *

# Setting up parameters
epsilon = 0.001
num_iters = 200
num_rep = 3
data_prop = 3

# Set the current directory
parent_dir = os.path.dirname(os.getcwd())

# generate the array of seeds
np.random.seed(7415)
seeds = np.array(np.random.randint(1, 10000, 15)).reshape(5, 3)


def check_directory(type):

    output_loc = os.path.join(parent_dir, type)

    if os.path.exists(output_loc) is False:
        os.mkdir(output_loc)

    else:
        curr_time = datetime.now()
        create_timestamp = str(curr_time.hour) + "_" + str(curr_time.minute) + "_" + str(curr_time.second)
        bkup_dir = os.path.join(parent_dir, "backup_clus_results" + create_timestamp)
        os.rename(output_loc, bkup_dir)
        os.mkdir(output_loc)


def benchmark_clusters():

    if check_directory("clustering_results") is False:
        print("Error in directory creation, exiting.")
        exit(0)

    num_clusters = [10, 20, 60, 100, 500]
    num_clusters = [10]

    print("#####################")
    print("DClustering Experiments")
    print("#####################")

    # Get files
    input_loc = os.path.join(parent_dir, "benchmark", "clustering_data")
    file_list = glob(os.path.join(input_loc, "*"))

    # Output directory
    output_loc = os.path.join(parent_dir, "clustering_results")

    file_counter = 0

    DEKM_results = []
    KM_results = []

    for f in file_list:

        # Load the data
        data, labels = read_simulated_data(f)

        for nclus in num_clusters:

            print("Number of clusters: ", nclus)
            DEKM_temp_results = []
            KM_temp_results = []
            seed_counter = 0

            for i in range(num_rep):

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
    print(result)
    write_result_data(result, output_loc, "clustering_results")


def benchmark_dimensions():

    if check_directory("dims_results") is False:
        print("Error in directory creation, exiting.")
        exit(0)

    num_dims = [10, 100, 200, 400, 500]
    num_dims = [10]
    nclus = 20

    print("#####################")
    print("Dimensionality Experiments")
    print("#####################")

    # Get files
    input_loc = os.path.join(parent_dir, "benchmark", "dims_data")
    file_list = glob(os.path.join(input_loc, "*"))

    # Output directory
    output_loc = os.path.join(parent_dir, "dims_results")

    file_counter = 0

    DEKM_results = []
    KM_results = []

    for f in file_list:

        print(f)

        # Load the data
        data, labels = read_simulated_data(f)

        for dims in num_dims:

            print("Number of dimensions: ", dims)
            DEKM_temp_results = []
            KM_temp_results = []
            seed_counter = 0

            for i in range(num_rep):

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
    print(result)
    write_result_data(result, output_loc, "dims_results")


def benchmark_size():

    if check_directory("scal_results") is False:
        print("Error in directory creation, exiting.")
        exit(0)

    print("#####################")
    print("Scalability Experiments")
    print("#####################")

    num_points = [1000000, 3000000, 5000000, 8000000, 100000000]
    nclus = 10

    # Get files
    input_loc = os.path.join(parent_dir, "benchmark", "scal_data")
    file_list = glob(os.path.join(input_loc, "*"))

    # Output directory
    output_loc = os.path.join(parent_dir, "scal_results")

    file_counter = 0

    DEKM_results = []
    KM_results = []

    for f in file_list:

        print(f)

        # Load the data
        data, labels = read_simulated_data(f)

        for npoints in num_points:

            print("Number of datapoints: ", npoints)
            DEKM_temp_results = []
            KM_temp_results = []
            seed_counter = 0

            for i in range(num_rep):

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

                DEKM_temp_results += [[npoints, TraningTime1, iter1, ari1]]
                KM_temp_results += [[npoints, TraningTime2, iter2, ari2]]

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
    print(result)
    write_result_data(result, output_loc, "scal_results")


# benchmark_clusters()
# benchmark_dimensions()
# benchmark_size()