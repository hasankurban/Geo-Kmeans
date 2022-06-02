from utils.kmeans_utils import *
from utils.vis_utils import *
import sys


def DCKMeans(data, num_clusters, threshold, num_iterations, seed):

    loop_counter = 0
    assign_dict = {}
    dist_mat = np.zeros((num_clusters, num_clusters))

    centroids = init_centroids(data, num_clusters, seed)

    # Calculate the cluster assignments for data points
    assigned_clusters, distances = calculate_distances(data, centroids)

    if len(np.unique(assigned_clusters)) < num_clusters:
        print("DCKMeans: Found less modalities, safe exiting with current centroids.")
        return centroids, loop_counter, sys.float_info.max

    # dckm_calc = num_clusters * data.shape[0]

    while loop_counter < num_iterations:

        loop_counter += 1

        # Re-calculate the centroids
        new_centroids = calculate_centroids(data, assigned_clusters)

        if check_convergence(new_centroids, centroids, threshold):
            print("Kmeans: Convergence at iteration: ", loop_counter)
            break

        assign_dict, radius = get_membership(assigned_clusters, distances, num_clusters, assign_dict)
        neighbors, he_indices_dict = find_all_he_indices_neighbor(data, new_centroids, radius,
                                                assign_dict, dist_mat)

        # temp = []
        # for i in he_indices_dict.keys():
        #     print(i, he_indices_dict[i])

        for center in neighbors:
            if len(he_indices_dict[center]) > 0:
                temp, dist123 = calculate_distances_specific(data[he_indices_dict[center]],
                                             new_centroids[neighbors[center]], neighbors[center])

                distances[he_indices_dict[center]] = dist123
                assigned_clusters[he_indices_dict[center]] = temp
                # dckm_calc += len(he_indices_dict[center]) * len(neighbors[center])

        if len(np.unique(assigned_clusters)) < num_clusters:
            print("DCKMeans: Found less modalities, safe exiting with current centroids.")
            return centroids, loop_counter, sys.float_info.max

        # Calculate the cluster assignments for data points
        centroids[:] = new_centroids[:]

    # calculate the within cluster SSE
    sse = get_quality(data, assigned_clusters, new_centroids, num_clusters)
    return new_centroids, loop_counter, sse






