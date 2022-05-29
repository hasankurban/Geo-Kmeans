from utils.kmeans_utils import *


def Kmeans(data, num_clusters, threshold, num_iterations, seed):

    loop_counter = 0

    centroids = init_centroids(data, num_clusters, seed)

    # Calculate the cluster assignments for data points
    assigned_clusters, _, centroids, num_clusters = \
        calculate_distances_less_modalities(data, centroids, num_clusters)

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
        # print(loop_counter, ":", get_quality(data, assigned_clusters, new_centroids, num_clusters))

    # print("KMeans exiting at: ", loop_counter, " iterations")
    sse = get_quality(data, assigned_clusters, new_centroids, num_clusters)
    return new_centroids, loop_counter, sse


