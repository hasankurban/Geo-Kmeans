from utils.kmeans_utils import *
from utils.vis_utils import *


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



def DCKMeans(data, num_clusters, threshold, num_iterations, seed):

    centroids = init_centroids(data, num_clusters, seed)

    # Calculate the cluster assignments for data points
    assigned_clusters, distances = calculate_distances(data, centroids)
    loop_counter = 0
    assign_dict = {}
    dist_mat = np.zeros((num_clusters, num_clusters))

    dckm_calc = num_clusters * data.shape[0]

    while loop_counter < num_iterations:

        loop_counter += 1

        # Re-calculate the centroids
        new_centroids = calculate_centroids(data, assigned_clusters)

        if check_convergence(new_centroids, centroids, threshold):
            print("Kmeans: Convergence at iteration: ", loop_counter)
            break

        assign_dict = get_membership(assigned_clusters, assign_dict, num_clusters)

        he_data_indices = find_all_he_indices(data, new_centroids, distances,
                                                assign_dict, dist_mat)

        # break
        # Calculate the cluster assignments for data points
        # some_assigned_clusters, distances = calculate_distances(data, new_centroids)
        # data_changed = np.where(assigned_clusters != some_assigned_clusters)[0]
        #print(data_changed)
        #break
        # for j in data_changed:
        #     if j not in he_data_indices:
        #         print("Point: ", j, " previous center", assigned_clusters[j],
        #               " New center: ", some_assigned_clusters[j], " Point: ", data[j,])
        #
        # vis_data_with_he(data, new_centroids, assigned_clusters, distances,
        #                  loop_counter, data_changed, he_data_indices)

        # vis_data_with_he_test(data, new_centroids, assigned_clusters, distances,
        #                  loop_counter, data_changed, he_data_indices)

        # view_3d(data, new_centroids, some_assigned_clusters)

        if len(he_data_indices) > 0:
            temp, dist123 = calculate_distances(data[he_data_indices, ], new_centroids)
            distances[he_data_indices] = dist123
            assigned_clusters[he_data_indices] = temp
            dckm_calc += len(he_data_indices) * num_clusters


        # Calculate the cluster assignments for data points
        centroids[:] = new_centroids[:]
        # assigned_clusters[:] = some_assigned_clusters[:]

    # print("KMeans exiting at: ", loop_counter, " iterations")
    return new_centroids, loop_counter, dckm_calc


def DCKMeans_tree(data, num_clusters, threshold, num_iterations, seed):

    loop_counter = 0
    assign_dict = {}
    dist_mat = np.zeros((num_clusters, num_clusters))

    centroids = init_centroids(data, num_clusters, seed)

    # Calculate the cluster assignments for data points
    assigned_clusters, distances = calculate_distances(data, centroids)

    tree = create_sorted_structure(assigned_clusters, distances, num_clusters)

    while loop_counter < num_iterations:

        loop_counter += 1

        # Re-calculate the centroids
        new_centroids = calculate_centroids(data, assigned_clusters)

        if check_convergence(new_centroids, centroids, threshold):
            print("Kmeans: Convergence at iteration: ", loop_counter)
            break

        assign_dict = get_membership(assigned_clusters, assign_dict, num_clusters)

        he_data_indices = find_all_he_indices(data, new_centroids, distances,
                                                assign_dict, dist_mat, tree)

        # break
        # Calculate the cluster assignments for data points
        # some_assigned_clusters, distances = calculate_distances(data, new_centroids)
        # data_changed = np.where(assigned_clusters != some_assigned_clusters)[0]
        #print(data_changed)
        #break
        # for j in data_changed:
        #     if j not in he_data_indices:
        #         print("Point: ", j, " previous center", assigned_clusters[j],
        #               " New center: ", some_assigned_clusters[j], " Point: ", data[j,])
        #
        # vis_data_with_he(data, new_centroids, assigned_clusters, distances,
        #                  loop_counter, data_changed, he_data_indices)

        # vis_data_with_he_test(data, new_centroids, assigned_clusters, distances,
        #                  loop_counter, data_changed, he_data_indices)

        # view_3d(data, new_centroids, some_assigned_clusters)

        if len(he_data_indices) > 0:
            temp, dist123 = calculate_distances(data[he_data_indices, ], new_centroids)
            distances[he_data_indices] = dist123
            assigned_clusters[he_data_indices] = temp


        # Calculate the cluster assignments for data points
        centroids[:] = new_centroids[:]
        # assigned_clusters[:] = some_assigned_clusters[:]

    # print("KMeans exiting at: ", loop_counter, " iterations")
    return new_centroids, loop_counter




# def kmeans_geom(data, num_clusters, threshold, num_iterations, seed):
#
#     centroids = init_centroids(data, num_clusters, seed)
#
#     # Calculate the cluster assignments for data points
#     assigned_clusters, distances = calculate_distances(data, centroids)
#     loop_counter = 0
#
#     new_assigned_clusters = np.zeros((assigned_clusters.shape))
#     bst_list = create_sorted_structure(assigned_clusters, distances, num_clusters)
#
#     # for i in range(len(bst_list)):
#     #     print(bst_list[i])
#
#     # Neighbor wise HE data
#     # he_data_indices, he_bst_indices, neighbors = find_he_indices(data, bst_list, centroids)
#
#     # All HE data
#     he_data_indices, he_bst_indices, _ = find_all_he_indices(data, bst_list, centroids)
#
#     # for k in he_data_indices:
#     #     print(k, he_bst_indices[k])
#
#     new_assigned_clusters[:] = assigned_clusters[:]
#
#     while loop_counter < num_iterations:
#
#         loop_counter += 1
#         print("Iter:", loop_counter)
#
#         print(he_data_indices)
#
#         # Re-calculate the centroids
#         new_centroids = calculate_centroids(data, assigned_clusters)
#
#         if check_convergence(new_centroids, centroids, threshold):
#             print("Kmeans: Convergence at iteration: ", loop_counter)
#             break
#
#         # Calculate distances of HE data from neighboring centroids
#         # for curr_center in neighbors.keys():
#         #     for neighbor_center in neighbors[curr_center]:
#         #         print("Checking for: ", curr_center, neighbor_center)
#         #         he_points = he_data_indices[curr_center][neighbor_center]
#         #
#         #         he_new_assigned_centers, he_new_min_dist = calculate_distances_specific(data[he_points, ],
#         #                                                    new_centroids[[curr_center, neighbor_center], ],
#         #                                                    curr_center, neighbor_center,
#         #                                                    assigned_clusters[he_points])
#         #
#         #         # membership changed: assign data to new cluster
#         #         bst_list, new_assigned_clusters = move_data_around(bst_list, he_bst_indices, he_points,
#         #                          assigned_clusters, new_assigned_clusters,
#         #                          he_new_assigned_centers, he_new_min_dist)
#
#
#         he_new_assigned_centers, he_new_min_dist = calculate_distances(data[he_data_indices, :], new_centroids)
#         # new_assigned_clusters[he_data_indices] = he_new_assigned_centers
#
#         bst_list, new_assigned_clusters = move_all_data_around(bst_list, he_bst_indices, he_data_indices,
#                                  assigned_clusters, new_assigned_clusters,
#                                  he_new_assigned_centers, he_new_min_dist)
#
#         he_data_indices, he_bst_indices, _ = find_all_he_indices(data, bst_list, new_centroids)
#
#         # Calculate the cluster assignments for data points
#         centroids[:] = new_centroids[:]
#
#         # he_data_indices, he_bst_indices, neighbors = find_he_indices(data, bst_list, centroids)
#
#         # Calculate the cluster assignments for data points
#         assigned_clusters[:] = new_assigned_clusters[:]
#
#     # print("KMeans exiting at: ", loop_counter, " iterations")
#     return new_centroids, loop_counter
