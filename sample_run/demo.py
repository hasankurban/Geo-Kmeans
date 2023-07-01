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


