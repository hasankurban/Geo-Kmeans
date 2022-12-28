from sklearn.datasets import make_blobs
import sys
sys.path.append("..")
from utils.kmeans_utils import vis_PCA
import numpy as np
import pandas as pd

X, y = make_blobs(n_samples=50, centers=5, cluster_std=[2, 10, 4, 5, 4], center_box=[-20, 10], n_features=2, random_state=512)

df = pd.DataFrame(X)
df['labels'] = y

df.to_csv("../sample_data/50_2_5.csv", sep=",", index=False)



# np.savetxt("../sample_data/50_2_5.txt", X, delimiter=",")

# vis_PCA(X, y)