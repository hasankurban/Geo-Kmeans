from utils.kmeans_utils import *
from utils.vis_utils import *
import sys
import time


def DCKMeans(data, num_clusters, threshold, num_iterations, centroids, seed):

    loop_counter = 0
    dckm_calc = 0
    assign_dict = {}

    dist_mat = np.zeros((num_clusters, num_clusters))
    new_assigned_clusters = np.zeros((data.shape[0]))

    if centroids == []:
        centroids = init_centroids(data, num_clusters, seed)
        print("Initialized centroids manually")

    # Calculate the cluster assignments for data points
    old_assigned_clusters, distances = calculate_distances(data, centroids)

    dckm_calc += data.shape[0]*num_clusters

    if len(np.unique(old_assigned_clusters)) < num_clusters:
        print("DCKMeans: Found less modalities, safe exiting with current centroids.")
        return centroids, loop_counter, sys.float_info.max, old_assigned_clusters, dckm_calc

    new_assigned_clusters[:] = old_assigned_clusters[:]

    # print("Initial centroids: ", centroids, "\n")
    # print(assigned_clusters[0:5])

    while loop_counter < num_iterations:

        loop_counter += 1
        # assign_dict = get_membership(assigned_clusters, assign_dict, num_clusters)
        #
        # neighbors, he_indices_dict = find_all_he_indices_neighbor(data, new_centroids, distances,
        #                                         assign_dict, dist_mat)

        # Re-calculate the centroids
        new_centroids = calculate_centroids(data, old_assigned_clusters)

        if check_convergence(new_centroids, centroids, threshold):
            print("DCKmeans: Convergence at iteration: ", loop_counter, "\n")
            break

        assign_dict, radius = get_membership(old_assigned_clusters, distances, num_clusters, assign_dict)
        neighbors, he_indices_dict = find_all_he_indices_neighbor(data, new_centroids, radius,
                                                assign_dict, dist_mat)

        # temp = []
        # for i in he_indices_dict.keys():
        #     print("iter: ", loop_counter, "\t center: ", i, "HE datas: ", len(he_indices_dict[i]), he_indices_dict[i])

        for center in neighbors:
            if len(he_indices_dict[center]) > 0:
                temp, dist123 = calculate_distances_specific(data[he_indices_dict[center]],
                                             new_centroids[neighbors[center]], neighbors[center])

                # for index in range(len(he_indices_dict[center])):
                #     point = he_indices_dict[center][index]
                #     print("Point: ", point, "\t", assigned_clusters[point], "-->", temp[index],
                #           " old dist: ", distances[point], " new distance: ", dist123[index])

                distances[he_indices_dict[center]] = dist123
                new_assigned_clusters[he_indices_dict[center]] = temp
                # dckm_calc += len(he_indices_dict[center]) * len(neighbors[center])

                dckm_calc += (len(he_indices_dict) * len(neighbors[center]))

        # if (new_assigned_clusters == old_assigned_clusters).all():
        #     print("DCKmeans: Convergence at iteration: ", loop_counter)
        #     break

        if len(np.unique(new_assigned_clusters)) < num_clusters:
            print("DCKMeans: Found less modalities, safe exiting with current centroids.")
            return centroids, loop_counter, sys.float_info.max, new_assigned_clusters, dckm_calc

        # Calculate the cluster assignments for data points
        centroids[:] = new_centroids[:]
        old_assigned_clusters[:] = new_assigned_clusters[:]

    # calculate the within cluster SSE
    sse = get_quality(data, new_assigned_clusters, new_centroids, num_clusters)
    return new_centroids, loop_counter, sse, new_assigned_clusters, dckm_calc






