import pandas as pd

from utils.dataIO import *
from base.KMeans import *
from pathlib import Path
from run_algorithms import run_algorithms

'''
Algo1: KMeans
Algo2: DEKMeans, 1 step look-back
Algo3: DEKMeans, probabilistic 1 step look back
Algo4: DEKMeans, Stochastic 1 step look back
'''

# Set parameters
threshold = 0.001
num_iterations = 100
data_threshold = 3

file_list = ['10_clusters.csv']
# file_list = ['1000000_points.csv']
# file_list = ['10k_50_5.txt', '100k_50_10.csv']
# file_list = ['10_clusters.csv']
# file_list = ['crop_320.csv']

# seed_array = np.array(np.random.randint(1, 10000, 70)).reshape(7, 10)
seeds = [112]


avg_results_df = pd.DataFrame(columns=["Algorithm", 'Num_clusters', 'Runtime', 'Num_iterations',
                          'Accuracy', 'ARI', 'AMI', 'Deviation'])

all_results_df = pd.DataFrame(columns=["Algorithm", 'Num_clusters', 'Runtime', 'Num_iterations',
                          'Accuracy', 'ARI', 'AMI', 'Deviation'])

algorithms = ['KMeans', 'DEKMeans-LB', 'DEKMeans-PROB', 'DEKMeans-STO']


# Make changes for adjusting the current directory here
file_path = os.path.join(Path(__file__).parents[1], "benchmark", "clustering_data")
# file_path = os.path.join(Path(__file__).parents[1], "sample_data")
# file_path = os.path.join(Path(__file__).parents[1], "benchmark", "scal_data")


result_dictionary = {'Algorithm': [], 'Num_clusters': [], 'Runtime': [],
                     'Num_iterations': [], 'Accuracy': [], 'ARI': [], 'AMI': [], 'Deviation': []}

num_clusters = []

for data_file in file_list:

    # data, labels = read_data(os.path.join(file_path, data_file))
    # numclus = int(data_file.split(".")[0].split("_")[2])

    # data, labels = read_crop_data(os.path.join(file_path, data_file), False)
    # numclus = 7

    data, labels = read_simulated_data(os.path.join(file_path, data_file))
    numclus = int(data_file.split(".")[0].split("_")[0])

    num_clusters.append(numclus)

    for i_seed in seeds:

        result_dictionary = run_algorithms(data, labels, result_dictionary, num_iterations,
                           threshold, numclus, 0, 0, i_seed)

all_results_df = pd.DataFrame(result_dictionary)
#
#
# for algo in algorithms:
#
#     for nclus in num_clusters:
#
#         subset = all_results_df.loc[(all_results_df['Algorithm'] == algo) & (all_results_df['Num_clusters'] == nclus),
#                                    ['Runtime', 'Num_iterations', 'Accuracy', 'ARI', 'AMI', 'Deviation']].mean().values
#
#         avg_results_df.loc[len(avg_results_df.index)] = [algo, nclus, subset[0], subset[1], subset[2],
#                                                        subset[3], subset[4], subset[5]]
# #
print(all_results_df)
# print(avg_results_df)
#
# ## Write the results to a file
# avg_results_df.to_csv("avg_clustering_experiments.csv", index=False, sep="\t")
# all_results_df.to_csv("all_results.csv", index=False, sep="\t")