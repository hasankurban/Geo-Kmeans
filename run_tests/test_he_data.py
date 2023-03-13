import sys
sys.path.append("../")

from utils.dataIO import *
from base.KMeans import *
from base.DCKmeans import *
import time
from utils.assign_clusters import *


'''
Algo1: KMeans
Algo2: DCKMeans
'''

# Set parameters

threshold = 0.001
num_iterations = 100

# file_list = ['test_100_2_3.csv']

file_list = ['crop.csv']
# file_list = ['user_knowledge_train.csv']
# file_list = ['hapt_train.csv']
# file_list = ['covertype.csv']
# file_list = ['spambase.csv']
# file_list = ['10_clusters.csv']

# file_list = ['ringnorm.csv']
cenfile = 'ringnormCentroids_5_0.txt'

DATA_PATH = "/Users/schmuck/Documents/Box Sync/Ph.D./DATASETS"
DATA_PATH = "/Users/schmuck/Documents/OneDrive - Indiana University/Box Sync/PhD/DATASETS/"

# Make changes for adjusting the current directory here
file_path = os.path.join(DATA_PATH, "benchmark", "clustering_data")
# file_path = os.path.join(Path(__file__).parents[1], "benchmark", "scal_data")
# file_path = os.path.join(DATA_PATH, "real_data")
file_path = os.path.join(DATA_PATH, "real_data", "experiment_data", "comma_seperated_files")

# file_path = os.path.join(DATA_PATH, "sample_data")


num_clusters = [5, 10]
seed = 1245

seeds = np.random.randint(1, 1200, 1000)
seeds = [1]
counter = 1


for data_file in file_list:

    # data, labels = read_simulated_data(os.path.join(file_path, data_file))
    data = read_simulated_data_test(os.path.join(file_path, data_file))
    # centroids1 = read_simulated_data_test(os.path.join(DATA_PATH, "real_data", "experiment_data", "comma_seperated_centroids", cenfile))
    # centroids2 = read_simulated_data_test(os.path.join(DATA_PATH, "real_data", "experiment_data", "comma_seperated_centroids", cenfile))
    # centroids = read_simulated_data_test(os.path.join(DATA_PATH, "real_data", "experiment_data", "test1", cenfile))

    print("Data Shape :", data.shape)

    for centers in num_clusters:

        km_start_time = time.time()
        km_centroids, km_iter, km_sse, km_calc = Kmeans(data, centers, threshold, num_iterations, [], seed)
        km_TraningTime = round(time.time() - km_start_time, 2)

        kmdc_start_time = time.time()
        dckm_centroids, dckm_iter, dckm_sse, dckm_calc = DCKMeans(data, centers, threshold, num_iterations, [], seed)
        kmdc_TraningTime = round(time.time() - kmdc_start_time, 2)

        print("Total runtime: Kmeans- ", km_TraningTime, "DCKMeans- ", kmdc_TraningTime)

        print("Distance calculations by KMeans: ", km_calc)
        print("Distance calculations by DCKMeans: ", dckm_calc)

        # acc, new_labels1 = calc_raw_accuracy(labels, assign1, data)
        # print("KM: ", acc, check_ARI(new_labels1, labels))
        #
        # acc, new_labels2 = calc_raw_accuracy(labels, assign2, data)
        # print("KMLB: ", acc, check_ARI(new_labels2, labels))
        #
        # print("Diff in clustering: ", len(np.where(new_labels1 != new_labels2)[0]))
        # print("Diff in clustering: ", len(np.where(assign1 != assign2)[0]))
        # print(assign1[np.where(assign1 != assign2)[0]], assign2[np.where(assign1 != assign2)[0]])

    counter += 1

