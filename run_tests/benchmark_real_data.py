import numpy as np

from utils.dataIO import *
from base.expr_DEKmeans import *
from pathlib import Path
from run_algorithms_real_data import run_algorithms

'''
Algo1: KMeans
Algo2: DEKMeans, 1 step look-back
Algo3: DEKMeans, probabilistic 1 step look back
Algo4: DEKMeans, Stochastic 1 step look back
'''

# Set parameters
epsilon = 0.0001
num_iterations = 100


file_list = [['user_knowledge_train.csv', 'User_Knowledge_Data', 'user_knowledge_test.csv'],
             ['spambase.csv', 'Spambase_Data'], ['magic.csv', 'MagicGamma_Data'],
             ['hapt_train.csv', 'Smartphone_Posture_Data', 'hapt_test.csv'],
             ['crop.csv', 'Cropland_Map_Data']]


file_list = [['spambase.csv', 'Spambase_Data']]


np.random.seed(45125)
seed_array = np.array(np.random.randint(1, 10000, 10)).reshape(5, 2)


avg_results_df = pd.DataFrame(columns=["Algorithm", 'Num_clusters', 'Dataset',  'Runtime', 'Num_iterations',
                          'Accuracy', 'ARI', 'AMI', 'Deviation'])

all_results_df = pd.DataFrame(columns=["Algorithm", 'Num_clusters', 'Dataset', 'Runtime', 'Num_iterations',
                          'Accuracy', 'ARI', 'AMI', 'Deviation'])

algorithms = ['KMeans', 'DCKMeans']

# algorithms = ['KMeans']

# Make changes for adjusting the current directory here
file_path = os.path.join(Path(__file__).parents[1], "benchmark", "real_data")

result_dictionary = {'Algorithm': [], 'Num_clusters': [], 'Runtime': [], 'Dataset':[],
                     'Num_iterations': [], 'Accuracy': [], 'ARI': [], 'AMI': [], 'Deviation': []}


num_clusters = [5, 10, 15, 20]
run_counter = 0


for dataset in file_list:

    print("######\nStarted the run on\n######\n", dataset[1], "\n#####\n")

    # Load the data
    data_file_path = os.path.join(file_path, dataset[0])
    data, labels = read_real_data(data_file_path, dataset)

    for numclus in num_clusters:

        print(numclus, "\n")
        run_counter = 0

        for seed in seed_array[run_counter]:

            result_dictionary = run_algorithms(data, labels, dataset[1],
                                               result_dictionary, num_iterations, epsilon,
                                               numclus, seed)

        run_counter += 1


# for k in result_dictionary.keys():
#     print(k, result_dictionary[k])

all_results_df = pd.DataFrame(result_dictionary)


for algo in algorithms:

    for dataset in file_list:

        for nclus in num_clusters:

            subset = all_results_df.loc[(all_results_df['Algorithm'] == algo) & (all_results_df['Num_clusters'] == nclus) & (all_results_df['Dataset'] == dataset[1]),
                                   ['Runtime', 'Num_iterations', 'Accuracy', 'ARI', 'AMI', 'Deviation']].mean().values

            avg_results_df.loc[len(avg_results_df.index)] = [algo, nclus, dataset[1], subset[0], subset[1], subset[2],
                                                       subset[3], subset[4], subset[5]]

# print(all_results_df)
print(avg_results_df)

## Write the results to a file
avg_results_df.to_csv("real_data_avg_clustering_experiments.csv", index=False, sep=",")
all_results_df.to_csv("real_data_all_results.csv", index=False, sep=",")