from utils.dataIO import *
from base.KMeans import *
import time
import gc
from pathlib import Path

'''
Algo1: KMeans
Algo2: DEKMeans, 1 step look-back
Algo3: DEKMeans, probabilistic 1 step look back
Algo4: DEKMeans, Stochastic 1 step look back
'''

file_dicts = {0: ['10k_50_5.txt', '10k_50_5_centers.txt'],
              1: ['50k_50_5.txt', '50k_50_5_centers.txt'],
              2: ['100k_50_5.txt', '100k_50_5_centers.txt'],
              3: ['200k_50_5.txt', '200k_50_5_centers.txt'],
              4: ['300k_50_5.txt', '300k_50_5_centers.txt'],
              5: ['400k_50_5.txt', '400k_50_5_centers.txt'],
              6: ['500k_50_5.txt', '500k_50_5_centers.txt'],
              7: ['500k_50_20.txt', '500k_50_20_centers.txt']}

file_dicts = {0: ['100k_50_5.csv', '100k_50_5_centers.csv']}

# Set parameters
threshold = 0.0001

# Make changes for adjusting the current directory here
file_path = os.path.join(Path(__file__).parents[1], "sample_data")


for i in file_dicts.keys():

    data_file = file_dicts[i][0]
    centroids_file = file_dicts[i][1]

    data, labels = read_simulated_data(os.path.join(file_path, data_file))
    centroids_file = pd.read_csv(os.path.join(file_path, centroids_file))

    do_PCA(data, np.array(centroids_file), 0, labels, "Size: 10,000, Dimensions: 50", "sample_file.jpeg")

