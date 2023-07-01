# Geometric-KMeans (geo-KMeans)

Geometry inspired, data-centric version of traditional Lloyd's KMeans clustering algorithm.

### Installation for Python version

#### Install dependencies

- The dependencies required for Python version are numpy, pandas, sklearn, matplotlib
- This release have been tested for Python3.10+. If you face any problem while using older version of Python then please open an issue.
- These can be installed in one line via pip as follows:

```
pip3 install numpy, pandas, sklearn, matplotlib
```

#### Sample run

- The following code can be directly executed by running the sample_run/demo.py.
- The test data is given inside the sample_run folder.

```
import sys
sys.path.append("../")

from utils.dataIO import *
from base.KMeans import *
from base.DCKmeans import *
import time


# Set parameters
threshold = 0.001
num_iterations = 100
num_clusters = 10


# Sample file name and target labels
fileName = 'BreastCancer.csv'
labelsName = 'labels_BreastCancer.csv'

# Seed for reproducing the results
seed = 8


# Load the data and labels
data= read_data(fileName)
labels = read_labels(labelsName)

print("Data Shape :", data.shape, "Labels: ", labels.shape)


# Run traditional KMeans and record time
km_start_time = time.time()
km_centroids, km_iter, km_sse, km_labels, km_calc = Kmeans(data, num_clusters, threshold, num_iterations, [], seed)
km_TraningTime = round(time.time() - km_start_time, 2)


# Run geometric-KMeans and record time
geo_start_time = time.time()
geo_centroids, geo_iter, geo_sse, geo_labels, geo_calc = DCKMeans(data, num_clusters, threshold, num_iterations, [], seed)
geo_TraningTime = round(time.time() - geo_start_time, 2)


print("\nRuntime-Kmeans:", km_TraningTime, "\nGeo-KMeans:", geo_TraningTime)
print("\n\nDistance calculations", "\nKMeans: ", km_calc, "\nGeo_Kmeans:", geo_calc)


# Calculate the Adjusted Rand Index
print("\n\nAdjusted Rand Index (ARI)")
print("KMeans: ", check_ARI(km_labels, labels))
print("Geo-KMeans: ", check_ARI(geo_labels, labels))
```

#### How to use Python implementation of geo-KMeans

- The relevant functions for data-centric and traditional KMeans (traditional is Lloyd's KMeans) are shown below.
- Description of input arguments and returned parameters is given below:

__How to call KMeans/DCKMeans routine:__

```
dckm_centroids, dckm_iter, dckm_sse, dckm_calc = DCKMeans(data, centers, threshold, num_iterations, centroids, seed)

km_centroids, km_iter, km_sse, km_calc = Kmeans(data, centers, threshold, num_iterations, centroids, seed)
```

__Description of input arguments:__

1. data (2D matrix): A numpy matrix in format n * d, where n = number of data points (in rows) and d = dimensions (columns).
2. centers (integer): number of clusters to partition the data.
3. threshold (float): Convergence threshold to stop the algorithm. Default threshold of 0.001 is used but can be changed by the user as needed.
4. num_iterations (integer): Maximum number of iterations.
5. centroids (2D matrix): You can either provide the centroids as 2d numpy array (data in rows and features in columns) or pass an empty list [] to randomly select the centroids.
6. seed (integer): For reprodicibility, a seed to use in case of random centroid selection.

__Description of output:__

1. dckm_centroids: Final centroids upon convergence.
2. dckm_iter: Number of iterations until convergence.
3. dckm_sse: The sum of squared distances upon convergence. 
4. dckm_calc: Number of distance calculations performed by the algorithm.

Note: The third parameter (sse) returned by the function is used to evaluate the quality of clustering. If the centroids produced by the two
algorithms are same then both algorithms must have the same SSE value upon convergence.
