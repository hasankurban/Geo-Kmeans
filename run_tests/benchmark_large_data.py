import sys
sys.path.append("..")

from utils.dataIO import *
from base.expr_DEKmeans import *
from pathlib import Path
from run_algorithms_real_data import run_algorithms

'''
Algo1: KMeans
Algo2: DCKMeans: Data Centric KMeans
'''

# Set parameters
epsilon = 0.001
num_iterations = 100


np.random.seed(45125)
seed_array = np.array(np.random.randint(1, 10000, 5)).reshape(5, 1)


avg_results_df = pd.DataFrame(columns=["Algorithm", 'Num_clusters', 'Dataset',  'Runtime', 'Num_iterations',
                          'Accuracy', 'ARI', 'AMI', 'Deviation'])

all_results_df = pd.DataFrame(columns=["Algorithm", 'Num_clusters', 'Dataset', 'Runtime', 'Num_iterations',
                          'Accuracy', 'ARI', 'AMI', 'Deviation'])

algorithms = ['KMeans', 'DCKMeans']


# Make changes for adjusting the current directory here
file_path = os.path.join(Path(__file__).parents[1], "benchmark", "real_data")

result_dictionary = {'Algorithm': [], 'Num_clusters': [], 'Runtime': [], 'Dataset':[],
                     'Num_iterations': [], 'Accuracy': [], 'ARI': [], 'AMI': [], 'Deviation': []}


num_clusters = [10, 50, 100, 200, 500]
run_counter = 0

# Load the data
data_file_path = os.path.join(file_path, '500_clusters.csv')
data, labels = read_simulated_data(data_file_path)


for numclus in num_clusters:

    print("######\nStarted the run on\n######\n", numclus, "\n#####\n")

    result_dictionary = run_algorithms(data, labels, '500_clusters',
                                               result_dictionary, num_iterations, epsilon,
                                               numclus, seed_array[run_counter])

    run_counter += 1


# for k in result_dictionary.keys():
#     print(k, result_dictionary[k])

all_results_df = pd.DataFrame(result_dictionary)

# for algo in algorithms:
#
#     for dataset in file_list:
#
#         for nclus in num_clusters:
#
#             subset = all_results_df.loc[(all_results_df['Algorithm'] == algo) & (all_results_df['Num_clusters'] == nclus) & (all_results_df['Dataset'] == dataset[1]),
#                                    ['Runtime', 'Num_iterations', 'Accuracy', 'ARI', 'AMI', 'Deviation']].mean().values
#
#             avg_results_df.loc[len(avg_results_df.index)] = [algo, nclus, dataset[1], subset[0], subset[1], subset[2],
#                                                        subset[3], subset[4], subset[5]]

# print(all_results_df)
print(all_results_df)

## Write the results to a file
# avg_results_df.to_csv("real_data_avg_clustering_experiments.csv", index=False, sep=",")
all_results_df.to_csv("real_data_all_results.csv", index=False, sep=",")