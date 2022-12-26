import sys
sys.path.append("..")

from utils.dataIO import *
from base.KMeans import *
from pathlib import Path
from run_algorithms import run_algorithms

'''
Algo1: KMeans
Algo2: DCKMeans: Data Centric KMeans
'''

# Set parameters
epsilon = 0.001
num_iterations = 100

np.random.seed(45125)
seed_array = np.array(np.random.randint(1, 10000, 5)).reshape(5, 1)


all_results_df = pd.DataFrame(columns=["Algorithm", 'Num_clusters', 'Dataset', 'Runtime', 'Num_iterations',
                          'Accuracy', 'ARI', 'Dist_Calc'])

algorithms = ['KMeans', 'DCKMeans']


# Make changes for adjusting the current directory here
file_path = os.path.join(Path(os.path.abspath(__file__)).parents[2], "scratch", "DATASETS", "scal_data")


result_dictionary = {'Algorithm': [], 'Num_clusters': [], 'Runtime': [], 'Dataset':[],
                     'Num_iterations': [], 'Accuracy': [], 'ARI': [], 'Dist_Calc': []}


num_points = [1000000, 3000000, 5000000, 8000000, 10000000]
# num_points = [1000000]
run_counter = 0
numclus = 10


for points in num_points:

	# Load the data
	file_name = str(points)+'_points.csv'
	data_file_path = os.path.join(file_path, file_name)
	data, labels = read_simulated_data(data_file_path)

	print("######\nSize: ", points, " points\n######\n")
	result_dictionary = run_algorithms(data, labels, str(numclus)+'_points', result_dictionary, num_iterations, epsilon,
                                        points, seed_array[run_counter])

	run_counter += 1

all_results_df = pd.DataFrame(result_dictionary)
all_results_df.rename(columns={"Num_clusters": "Num_Points"})
# print(all_results_df)

## Write the results to a file
all_results_df.to_csv("simulated_scal_all_results.csv", index=False, sep=",")



