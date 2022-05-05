import numpy as np
from utils.kmeans_utils import init_centroids

def distances(data, centroids,nclust):
    n,d= data.shape
    dist  = np.zeros((nclust,n),dtype=float) 
    for k in range(nclust):
        for i in range(len(data)):
            dist[k][i] = np.linalg.norm(data[i]-centroids[k])
    return dist

def partition_data(data,dist,nclust):
    temp = dist.argmin(axis=0)
    uniqueItems = np.unique(temp)
    clusters = [[] for i in range(nclust)]
    for i in range(nclust):
        try:
            item = uniqueItems[i]
            indices = [t for t, x in enumerate(temp) if x == item]
            clusters[i] = data[indices]
        except IndexError:
            clusters[i] = data[np.random.randint(0, len(data), size=1)]    
    return clusters

def update_centroids(clusters,centroids,nclust):
    for j in range(nclust):
        centroids[j] = clusters[j].mean(axis=0)
    return centroids


def km_clustering(data, nclust, maxiter, epsilon, i_seed):

    centroids = init_centroids(data, nclust, i_seed)
    counter = 0

    for i in range(maxiter):
        old_centroids = 1 * centroids
        dist = distances(data, centroids, nclust)
        clusters = partition_data(data,dist,nclust)
        centroids = update_centroids(clusters,centroids,nclust)

        temp = 0

        for j in range(nclust):
            temp = temp + np.sqrt(np.power((centroids[j] - old_centroids[j]), 2).sum())

        temp = round(temp, 3)
        if temp <= epsilon:
            break 

        counter += 1

    return centroids, counter