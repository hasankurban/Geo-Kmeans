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

file_list = ['test_100_2_3.csv']

file_list = ['census.csv']
# label_file = 'census_labels.csv'
# file_list = ['user_knowledge_train.csv']
# file_list = ['hapt_train.csv']
# file_list = ['covertype.csv']
# file_list = ['spambase.csv']
# file_list = ['10_clusters.csv']
file_list = ['CodonUsage.csv']
labels_list = ['labels_CodonUsage.csv']

DATA_PATH = "/Users/schmuck/Library/CloudStorage/OneDrive-IndianaUniversity/PhD/DATASETS/"


# Make changes for adjusting the current directory here
# file_path = os.path.join(DATA_PATH, "benchmark", "clustering_data")
# file_path = os.path.join(Path(__file__).parents[1], "benchmark", "scal_data")
file_path = os.path.join(DATA_PATH, "real_data")
# file_path = os.path.join(DATA_PATH, "sample_data")


# seeds = np.random.randint(1, 1200, 1000)

num_clusters = [4, 5, 6, 7, 8]
seed = 1245


for index in range(0, len(file_list)):

    data_file = file_list[index]
    label_file = labels_list[index]

    data= read_data(os.path.join(file_path, data_file))
    labels = read_labels(os.path.join(file_path, label_file))

    print("Data Shape :", data.shape, "Labels: ", labels.shape)

    for centers in num_clusters:

        km_start_time = time.time()
        km_centroids, km_iter, km_sse, km_labels, km_calc = Kmeans(data, centers, threshold, num_iterations, [], seed)
        km_TraningTime = round(time.time() - km_start_time, 2)

        kmdc_start_time = time.time()
        dckm_centroids, dckm_iter, dckm_sse, dckm_labels, dckm_calc = DCKMeans(data, centers, threshold, num_iterations, [], seed)
        kmdc_TraningTime = round(time.time() - kmdc_start_time, 2)

        print("\nRuntime-Kmeans:", km_TraningTime, "DCKMeans:", kmdc_TraningTime)

        # print("Distance calculations by KMeans: ", km_calc)
        # print("Distance calculations by DCKMeans: ", dckm_calc, "\n")

        # acc, new_labels1 = calc_raw_accuracy(labels, km_labels, data)
        # print("KM: ", check_ARI(km_labels, labels))
        
        # _, new_labs = calc_raw_accuracy(labels, dckm_labels, data)

        # acc, new_labels2 = calc_raw_accuracy(labels, dckm_labels, data)
        # print(np.unique(dckm_labels), np.unique(labels))
        print("KM: ", check_ARI(km_labels, labels))
        print("KMDC: ", check_ARI(dckm_labels, labels))
        
        # print("Diff in clustering: ", len(np.where(new_labels1 != new_labels2)[0]))
        # print("Diff in clustering: ", len(np.where(assign1 != assign2)[0]))
        # print(assign1[np.where(assign1 != assign2)[0]], assign2[np.where(assign1 != assign2)[0]])

    #counter += 1

