This repository contains the Python and C++ source code for traditional KMeans (Lloyd's version) and the Data-Centric 
implementation of KMeans.

#### Required packages

- The dependencies required for Python version are numpy, pandas, sklearn, matplotlib
- These can be installed in one line via pip as follows:

```
pip install numpy, pandas, sklearn, matplotlib
```

#### Basic Setup

- The relevant function can be called as follows:
- For KMeans, the primary function is called Kmeans(data, centers, threshold, num_iterations, centroids, seed)

__How to call KMeans/DCKMeans routine:__

```
km_centroids, km_iter, km_sse, km_calc = Kmeans(data, centers, threshold, num_iterations, centroids, seed)

dckm_centroids, dckm_iter, dckm_sse, dckm_calc = DCKMeans(data, centers, threshold, num_iterations, centroids, seed)
```

__Description of input arguments:__

1. data (2D matrix): A numpy matrix of in format n * d, where n = number of data points (in rows) and d = dimensions (columns).
2. centers (integer): number of clusters to partition the data.
3. threshold (float): Value to stop the algorithm. Default threshold of 0.001 is used but can be changed by the user as needed.
4. num_iterations (integer): Number of iterations to run the algorithm.
5. centroids (2D matrix): You can pass the centroids of your choice as 2d numpy array (data in rows and features in columns) or pass [] to indicate random centroid selection by the algorithm.
6. seed (integer): A seed to use in case of random centroid selection.

__Description of output:__

1. km_centroids: Final centroids upon convergence.
2. km_iter: Number of iterations until convergence.
3. km_sse: The sum of squared distances upon convergence. 
4. km_calc: Number of distance calculations performed by the algorithm.

Note: The fourth parameter (sse) returned by the function is used to evaluate the quality of clustering. If the centroids produced by the two
algorithms are same then both algorithms must have the same SSE value upon convergence.
