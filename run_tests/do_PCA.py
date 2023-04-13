import sys
sys.path.append("../")

from utils.dataIO import *
from utils.kmeans_utils import *
import matplotlib.pyplot as plt


DATA_PATH = "/Users/schmuck/Documents/Box Sync/PhD/DATASETS/real_data"

# file_list = 'census.csv'
# data, labels = read_data(os.path.join(DATA_PATH, file_list), labels="yes", header="yes")
# vis_PCA(data, labels)

# file_list = 'birch.csv'
# data = read_data(os.path.join(DATA_PATH, file_list), labels="no", header="no")
# plt.scatter(data[:, 0], data[:, 1], alpha=0.7, edgecolors='b')
# plt.show()

file_list = 'Twitter.csv'
data = read_data(os.path.join(DATA_PATH, file_list), labels="no", header="no")
plt.scatter(data[:, 0], data[:, 1], alpha=0.7, edgecolors='b')
plt.show()