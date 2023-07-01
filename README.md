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

#### How to use geo-KMeans

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
