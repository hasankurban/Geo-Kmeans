from utils.kmeans_utils import *


def Kmeans(data, num_clusters, threshold, num_iterations, seed):

    loop_counter = 0

    centroids = init_centroids(data, num_clusters, seed)

    # Calculate the cluster assignments for data points
    assigned_clusters, _ = calculate_distances(data, centroids)

    while loop_counter < num_iterations:

        # Re-calculate the centroids
        new_centroids = calculate_centroids(data, assigned_clusters)

        if check_convergence(new_centroids, centroids, threshold):
            print("Kmeans: Convergence at iteration: ", loop_counter)
            break

        # Calculate the cluster assignments for data points
        centroids[:] = new_centroids[:]

        # Calculate the cluster assignments for data points
        assigned_clusters, _ = calculate_distances(data, centroids)

        loop_counter += 1

    # print("KMeans exiting at: ", loop_counter, " iterations")
    return new_centroids, loop_counter


