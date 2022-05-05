import numpy as np
from utils.heaping import *
from utils.kmeans_utils import init_centroids

def distances(data, centroids,nclust):
    n,d= data.shape
    dist = np.zeros((nclust,n),dtype=float)
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


def kmheap_clustering(data, nclust, maxiter, epsilon, i_seed):

    n,d = data.shape
    centroids = init_centroids(data, nclust, i_seed)

    # centroids = data[0:nclust, ]

    # centroids = np.zeros((nclust,d),dtype=float)
    #
    # for t in range(nclust): # assigning the first nclust points to the mu
    #     centroids[t] = data[t]

    # buildHeap heaps :
    dist = distances(data, centroids, nclust)
    clusters = partition_data(data, dist, nclust)
    centroids = update_centroids(clusters, centroids, nclust)
    heaps, badDataIndex, badData, temp1 = buildHeap(data,dist, n, d, nclust)

    counter = 0

    # Updating Heap
    for i in range(maxiter):   
        dist1 = distances(badData, centroids, nclust)

        # Updating weight for bad data points:
        for k in range(len(badDataIndex)):
            for c in range(nclust):
                 dist[c, badDataIndex[k][1]] = dist1[c, k]

        clusters = partition_data(data, dist, nclust)
        centroids = update_centroids(clusters, centroids, nclust)
        heaps, badDataIndex, badData, temp2 = newHeap(data, heaps, dist1, badDataIndex, d, nclust)  # Updating heaps:

        s = set(temp1)
        temp3 = [y for y in temp2 if y not in s]
        dissimilarity = round(float(len(temp3))/len(temp2), 3)

        if dissimilarity <= epsilon:
            break
        temp1 = 1*temp2

        counter += 1
        print(counter, dissimilarity)

    return centroids, counter