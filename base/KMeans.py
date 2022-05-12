from utils.kmeans_utils import *


def Kmeans(data, num_clusters, threshold, num_iterations, my_centroids, use_centroids, seed):

    loop_counter = 0

    # Get initial centroids
    if use_centroids:
        centroids = my_centroids
    else:
        centroids = init_centroids(data, num_clusters, seed)

    # Calculate the cluster assignments for data points
    assigned_clusters, _ = calculate_distances(data, centroids)

    while loop_counter < num_iterations:

        loop_counter += 1

        # Re-calculate the centroids
        new_centroids = calculate_centroids(data, assigned_clusters)

        if check_convergence(new_centroids, centroids, threshold):
            print("Kmeans: Convergence at iteration: ", loop_counter)
            break

        # Calculate the cluster assignments for data points
        centroids[:] = new_centroids[:]

        # Calculate the cluster assignments for data points
        assigned_clusters, _ = calculate_distances(data, centroids)

    # print("KMeans exiting at: ", loop_counter, " iterations")
    return new_centroids, loop_counter


