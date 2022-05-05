import sklearn.datasets as skd
import pandas as pd
from pathlib import Path
import os
from utils.kmeans_utils import vis_PCA, view_3d

file_path = os.path.join(Path(__file__).parents[1], "sample_data")

data, labels, centers = skd.make_blobs(n_samples=100000,
                             n_features=500,
                             centers=5,
                             return_centers=True)


# vis_PCA(data, label)
# view_3d(data, labels)

data = pd.DataFrame(data)
data['labels'] = labels
centers = pd.DataFrame(centers)
data.to_csv(os.path.join(file_path, "test_100k_500_5.csv"), sep=",", index=False)