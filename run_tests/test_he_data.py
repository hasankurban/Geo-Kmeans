import pandas as pd

from utils.dataIO import *
from base.expr_DEKmeans import *
from base.kmeans import *
from pathlib import Path
import time
from utils.assign_clusters import *
from sklearn.cluster import KMeans as km


'''
Algo1: KMeans
Algo2: DCKMeans
'''

# Set parameters
threshold = 0.0001
num_iterations = 100
data_threshold = 3
leaf_threshold = 3

# file_list = ['test_data_case1.csv']
# file_list = ['test_100k_500_5.csv']
# file_list = ['crop.csv']
file_list = ['magic.csv']
# file_list = ['user_knowledge_train.csv']
# file_list = ['spambase.csv']
# file_list = ['50_clusters.csv']

# Make changes for adjusting the current directory here
file_path = os.path.join(Path(__file__).parents[1], "benchmark", "clustering_data")
file_path = os.path.join(Path(__file__).parents[1], "benchmark", "real_data")
# file_path = os.path.join(Path(__file__).parents[1], "sample_data")

num_clusters = 10
seed = 127


for data_file in file_list:

    data, labels = read_simulated_data(os.path.join(file_path, data_file))
    # data, labels = read_real_data(os.path.join(file_path, data_file), ["user_knowledge_train.csv"])
    # vis_PCA(data, labels)
    # exit(0)

    print(data.shape)

    km_start_time = time.time()
    km_centroids, km_iter = Kmeans(data, num_clusters, threshold, num_iterations, [], False, seed)
    # km_centroids, km_iter = km_clustering(data, num_clusters, num_iterations, threshold, seed)
    km_TraningTime = round(time.time() - km_start_time, 2)

    kmlb_start_time = time.time()
    kmlb_centroids, kmlb_iter = DCKMeans(data, num_clusters, threshold, num_iterations, seed)
    kmlb_TraningTime = round(time.time() - kmlb_start_time, 2)

    # km_start_time = time.time()
    # kmlb_centroids, km_iter = kmeans_geom(data, num_clusters, threshold, num_iterations, seed)
    # km_TraningTime = round(time.time() - km_start_time, 2)
    #
    # km_start_time = time.time()
    # km_centroids, km_iter, _, _ = Kmeans_test(data, num_clusters, threshold, num_iterations, seed)
    # km_TraningTime = round(time.time() - km_start_time, 2)

    # print("Iterations: ", km_iter)
    # kmlb_start_time = time.time()
    # # kmlb_centroids, kmlb_iter = DEKmeans_tree(data, num_clusters, leaf_threshold, num_iterations, seed)
    # kmlb_centroids, kmlb_iter, assign2 = my_kmeans3(data, num_clusters, threshold, num_iterations, seed, [])
    # kmlb_TraningTime = round(time.time() - kmlb_start_time, 2)

    # acc, new_labels1 = calc_raw_accuracy(labels, assign1, data)
    # print("KM: ", acc, check_ARI(new_labels1, labels))
    #
    # acc, new_labels2 = calc_raw_accuracy(labels, assign2, data)
    # print("KMLB: ", acc, check_ARI(new_labels2, labels))
    #
    # print("Diff in clustering: ", len(np.where(new_labels1 != new_labels2)[0]))
    # print("Diff in clustering: ", len(np.where(assign1 != assign2)[0]))
    # print(assign1[np.where(assign1 != assign2)[0]], assign2[np.where(assign1 != assign2)[0]])

    print(km_TraningTime, kmlb_TraningTime)
    print("Dev: ", round(np.sqrt(np.mean(np.square(km_centroids - kmlb_centroids))), 3))
