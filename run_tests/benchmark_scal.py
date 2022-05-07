import sys
sys.path.append("..")

from utils.dataIO import *
from base.expr_DEKmeans import *
from pathlib import Path
from run_algorithms_large_data import run_algorithms

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
file_path = os.path.join(Path(os.path.abspath(__file__)).parents[1], "benchmark", "scal_data")

result_dictionary = {'Algorithm': [], 'Num_Points': [], 'Runtime': [], 
                     'Num_iterations': [], 'Accuracy': [], 'ARI': [], 'AMI': [], 'Deviation': []}


num_points = [1000000, 3000000, 5000000, 8000000]
run_counter = 0
numclus = 10


for points in num_points:

	# Load the data
	file_name = str(points)+'_points.csv'
	data_file_path = os.path.join(file_path, file_name)
	data, labels = read_simulated_data(data_file_path)

	print("######\nSize: ", points, " points\n######\n")

	result_dictionary = run_algorithms(data, labels, result_dictionary, num_iterations, epsilon,
                                               numclus, str(points)+'_points', seed_array[run_counter])

	
	run_counter += 1

all_results_df = pd.DataFrame(result_dictionary)
print(all_results_df)

## Write the results to a file
all_results_df.to_csv("simulated_scal_all_results.csv", index=False, sep=",")



