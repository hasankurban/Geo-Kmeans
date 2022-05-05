from utils.BST import *
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


def Kmeans_test(data, num_clusters, threshold, num_iterations, seed):

    loop_counter = 0
    swap_counts = np.zeros(data.shape[0])

    some_temp = []

    centroids = init_centroids(data, num_clusters, seed)

    # Calculate the cluster assignments for data points
    assigned_clusters, distance = calculate_distances123(data, centroids)

    # assigned_clusters = check_empty_clusters(assigned_clusters, num_clusters)

    while loop_counter < num_iterations:

        loop_counter += 1

        # Re-calculate the centroids
        new_centroids = calculate_centroids(data, assigned_clusters)

        if check_convergence(new_centroids, centroids, threshold):
            print("Kmeans: Convergence at iteration: ", loop_counter)
            break

        # print(centroids, new_centroids)

        # Calculate the cluster assignments for data points
        centroids[:] = new_centroids[:]

        # Calculate the cluster assignments for data points
        new_assigned_clusters, dd = calculate_distances123(data, centroids)
        #
        # _, num_swaps, he_index = check_membership_swaps(assigned_clusters, new_assigned_clusters, swap_counts)
        # some_temp += he_index
        temp = np.where(assigned_clusters != new_assigned_clusters)[0]
        print(temp)

        # temp = []
        # for i in range(num_clusters):
        #     for index in np.sort(he_index):
        #         if assigned_clusters[index] == i:
        #             temp.append([index, assigned_clusters[index], new_assigned_clusters[index],
        #                         dd[index][assigned_clusters[index]], dd[index][new_assigned_clusters[index]]])
        #                          # distance[index][assigned_clusters[index]], distance[index][new_assigned_clusters[index]]])
        # # distance[:] = dd[:]
        #
        # print("Counter: ", loop_counter, "swaps: ", num_swaps, "indices: ", temp, "\n", he_index)
        # print(he_index)

        # for index in he_index:
        #     temp.append([index, assigned_clusters[index], new_assigned_clusters[index],
        #                         dd[index], dd[index]])

        # print("Counter: ", loop_counter, "swaps: ", num_swaps, "indices: ", np.sort(he_index))
        assigned_clusters[:] = new_assigned_clusters[:]

    # print("KMeans exiting at: ", loop_counter, " iterations")
    return new_centroids, loop_counter, some_temp, new_assigned_clusters



def kmeans_vis_with_he(data, num_clusters, threshold, num_iterations, seed):

    centroids = init_centroids(data, num_clusters, seed)

    # Calculate the cluster assignments for data points
    assigned_clusters, distances = calculate_distances(data, centroids)
    loop_counter = 0

    # view_3d(data, centroids, assigned_clusters)

    while loop_counter < num_iterations:

        loop_counter += 1

        # Re-calculate the centroids
        new_centroids = calculate_centroids(data, assigned_clusters)

        if check_convergence(new_centroids, centroids, threshold):
            print("Kmeans: Convergence at iteration: ", loop_counter)
            break

        he_data_indices = find_all_he_indices_1(data, centroids, new_centroids, distances, assigned_clusters)

        # break
        # Calculate the cluster assignments for data points
        # some_assigned_clusters, distances = calculate_distances(data, new_centroids)
        # data_changed = np.where(assigned_clusters != some_assigned_clusters)[0]
        # print(data_changed)
        # break
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



def kmeans_geom(data, num_clusters, threshold, num_iterations, seed):

    centroids = init_centroids(data, num_clusters, seed)

    # Calculate the cluster assignments for data points
    assigned_clusters, distances = calculate_distances(data, centroids)
    loop_counter = 0

    new_assigned_clusters = np.zeros((assigned_clusters.shape))
    bst_list = create_sorted_structure(assigned_clusters, distances, num_clusters)

    # for i in range(len(bst_list)):
    #     print(bst_list[i])

    # Neighbor wise HE data
    # he_data_indices, he_bst_indices, neighbors = find_he_indices(data, bst_list, centroids)

    # All HE data
    he_data_indices, he_bst_indices, _ = find_all_he_indices(data, bst_list, centroids)

    # for k in he_data_indices:
    #     print(k, he_bst_indices[k])

    new_assigned_clusters[:] = assigned_clusters[:]

    while loop_counter < num_iterations:

        loop_counter += 1
        print("Iter:", loop_counter)

        print(he_data_indices)

        # Re-calculate the centroids
        new_centroids = calculate_centroids(data, assigned_clusters)

        if check_convergence(new_centroids, centroids, threshold):
            print("Kmeans: Convergence at iteration: ", loop_counter)
            break

        # Calculate distances of HE data from neighboring centroids
        # for curr_center in neighbors.keys():
        #     for neighbor_center in neighbors[curr_center]:
        #         print("Checking for: ", curr_center, neighbor_center)
        #         he_points = he_data_indices[curr_center][neighbor_center]
        #
        #         he_new_assigned_centers, he_new_min_dist = calculate_distances_specific(data[he_points, ],
        #                                                    new_centroids[[curr_center, neighbor_center], ],
        #                                                    curr_center, neighbor_center,
        #                                                    assigned_clusters[he_points])
        #
        #         # membership changed: assign data to new cluster
        #         bst_list, new_assigned_clusters = move_data_around(bst_list, he_bst_indices, he_points,
        #                          assigned_clusters, new_assigned_clusters,
        #                          he_new_assigned_centers, he_new_min_dist)


        he_new_assigned_centers, he_new_min_dist = calculate_distances(data[he_data_indices, :], new_centroids)
        # new_assigned_clusters[he_data_indices] = he_new_assigned_centers

        bst_list, new_assigned_clusters = move_all_data_around(bst_list, he_bst_indices, he_data_indices,
                                 assigned_clusters, new_assigned_clusters,
                                 he_new_assigned_centers, he_new_min_dist)

        he_data_indices, he_bst_indices, _ = find_all_he_indices(data, bst_list, new_centroids)

        # Calculate the cluster assignments for data points
        centroids[:] = new_centroids[:]

        # he_data_indices, he_bst_indices, neighbors = find_he_indices(data, bst_list, centroids)

        # Calculate the cluster assignments for data points
        assigned_clusters[:] = new_assigned_clusters[:]

    # print("KMeans exiting at: ", loop_counter, " iterations")
    return new_centroids, loop_counter


# 2nd version - do full distance calculations by tossing a coin.
def DEKmeans_sto(data, num_clusters, num_iterations, seed):

    loop_counter = 0
    swap_counts = np.zeros(data.shape[0])

    # Get initial centroids
    centroids = init_centroids(data, num_clusters, seed)

    # Initial assignment of data to clusters
    assigned_clusters, _ = calculate_distances(data, centroids)

    # assigned_clusters = check_empty_clusters(assigned_clusters, num_clusters)

    # Calculate centroids for whole data
    new_centroids = calculate_centroids(data, assigned_clusters)
    new_assigned_clusters, distances = calculate_distances(data, new_centroids)

    # Reassign variables
    new_swap_counts, num_points, he_index = check_membership_swaps(new_assigned_clusters,
                                                                   assigned_clusters, swap_counts)

    # Iterate while convergence
    while loop_counter <= num_iterations:

        # Increase counter
        loop_counter += 1

        if np.random.binomial(1, 0.5) == 1:

            counter = 0
            while counter < 2:

                new_centroids = calculate_centroids(data, new_assigned_clusters)

                # Calculate the cluster assignments for data points
                new_assigned_clusters, _ = calculate_distances(data, new_centroids)
                assigned_clusters[:] = new_assigned_clusters[:]
                counter += 1

        else:

            new_centroids = calculate_centroids(data, new_assigned_clusters)

            # Calculate distances for only HE data
            he_cluster_assign, _ = calculate_distances(data[he_index, :], new_centroids)

            # Updating the membership for HE data
            new_assigned_clusters[he_index] = he_cluster_assign

            # Reassign variables
            new_swap_counts, num_points, he_index = check_membership_swaps(new_assigned_clusters,
                                                                           assigned_clusters, swap_counts)

            # Check for convergence
            dissimilarity = round((num_points / len(swap_counts)), 4)
            # print("Iter: ", loop_counter, "#data points: ", num_points, "dissim: ", dissimilarity)

            if dissimilarity <= 0.0001:
                print("DEKMeans Sto - Data Convergence at iteration: ", loop_counter)
                break

            assigned_clusters[:] = new_assigned_clusters[:]
            swap_counts[:] = new_swap_counts[:]

    # new_centroids = calculate_centroids(data, new_assigned_clusters)
    return new_centroids, loop_counter


def DEKmeans_prob(data, num_clusters, num_iterations, conv_threshold, seed):

    counter = 0
    loop_counter = 0
    converged = False
    swap_counts = np.zeros(data.shape[0])
    data_indices = np.array(list(range(data.shape[0])))

    # Get initial centroids
    centroids = init_centroids(data, num_clusters, seed)

    # Initial assignment of data to clusters
    assigned_clusters, _ = calculate_distances(data, centroids)

    # assigned_clusters = check_empty_clusters(assigned_clusters, num_clusters)

    while counter <= 2:

        # Calculate centroids for whole data
        new_centroids = calculate_centroids(data, assigned_clusters)
        new_assigned_clusters, _ = calculate_distances(data, new_centroids)

        # Reassign variables
        new_swap_counts, num_swaps = find_swap_counts(new_assigned_clusters, assigned_clusters, swap_counts)

        if check_convergence(centroids, new_centroids, conv_threshold):
            print("DEKMeans prob: Convergence in initial exploration phase")
            converged = True
            break

        swap_counts[:] = new_swap_counts[:]
        centroids[:] = new_centroids[:]
        assigned_clusters[:] = new_assigned_clusters[:]
        counter += 1

    if converged is False:

        # Get the probability vector
        prob_vector = get_prob_vector(new_swap_counts)
        # print(prob_vector)

        # Sample the HE data
        he_index = np.random.choice(data_indices, p=prob_vector, size=num_swaps+10, replace=False)

        # Iterate while convergence
        while loop_counter <= num_iterations:

            # Increase counter
            loop_counter += 1

            new_centroids = calculate_centroids(data, new_assigned_clusters)

            # Calculate distances for only HE data
            he_cluster_assign, _ = calculate_distances(data[he_index, :], new_centroids)

            # Updating the membership for HE data
            new_assigned_clusters[he_index] = he_cluster_assign

            # Reassign variables
            new_swap_counts, num_points, he_index = sample_HE_data(new_assigned_clusters, assigned_clusters,
                                                                   swap_counts, data_indices)

            # Check for convergence
            # dissimilarity = round((num_points / len(swap_counts)), 4)
            print("Iter: ", loop_counter, "#data points: ", num_points, "dissim: ", he_index)

            # if dissimilarity <= 0.01:
            #     print("DEKMeans Prob - Data Convergence at iteration: ", loop_counter)
            #     break

            if check_convergence(new_centroids, centroids, 0.001):
                print("DEKMeans lb - Data Convergence at iteration: ", loop_counter)
                break

            centroids[:] = new_centroids[:]
            assigned_clusters[:] = new_assigned_clusters[:]
            swap_counts[:] = new_swap_counts[:]

    # new_centroids = calculate_centroids(data, new_assigned_clusters)
    return new_centroids, loop_counter


def DCKmeans_improved(data, num_clusters, conv_threshold, leaf_threshold, num_iterations, seed):
    loop_counter = 0

    # Get initial centroids
    centroids = init_centroids(data, num_clusters, seed)

    # Calculate the cluster assignments for data points
    assigned_clusters, distances = calculate_distances(data, centroids)

    # Re-calculate the centroids
    new_centroids = calculate_centroids(data, assigned_clusters)

    # Create the Balanced Trees
    BST_List = create_BSTs(assigned_clusters, distances, num_clusters, data.shape[0])

    # Extract the high expressive data from the tree structure
    he_index, BST_List = extract_expressive_data(BST_List, num_clusters, leaf_threshold, data)

    while loop_counter <= num_iterations:

        if np.random.binomial(1, 0.5) == 1:
            i = 0
            while i < 2:
                # Re-calculate the centroids
                new_centroids = calculate_centroids(data, assigned_clusters)

                if check_convergence(new_centroids, centroids, conv_threshold):
                    print("DCKmeans-imp: Convergence at iteration: ", loop_counter)
                    break
                # Calculate the cluster assignments for data points
                assigned_clusters, distances = calculate_distances(data, new_centroids)
                # Calculate the cluster assignments for data points
                centroids = new_centroids
                i += 1
        else:
            # Calculate the distances for HE data only
            he_cluster_assign, distances = calculate_distances(data[he_index, :], new_centroids)

            # Update the BSTs to accommodate the new assignments for HE data
            new_he_index, BST_List = update_BST(BST_List, he_index, he_cluster_assign, distances,
                                                num_clusters, leaf_threshold, data)

            # Check for convergence
            he_index = set(he_index)
            diff_he_indices = [y for y in new_he_index if y not in he_index]
            dissimilarity = round(float(len(diff_he_indices)) / len(he_index), 4)

            if dissimilarity <= 0.01:
                print("DCKMeans-Convergence at iteration: ", loop_counter)
                break



def Kmeans_baseline(data, num_clusters, conv_threshold, num_iterations, seed):

    loop_counter = 0
    leaf_threshold = 3

    # Get initial centroids
    centroids = init_centroids(data, num_clusters, seed)

    # Calculate the cluster assignments for data points
    assigned_clusters, _ = calculate_distances(data, centroids)

    while loop_counter <= num_iterations:

        # Extract the 30% randomly from each cluster
        he_index = np.random.randint(0, data.shape[0], int(data.shape[0] / leaf_threshold)).tolist()

        # Calculate the distances for HE data only
        he_cluster_assign, distances = calculate_distances(data[he_index, :], centroids)

        # Update the Tree membership only for HE data
        #for i in range(0, len(he_index)):
        assigned_clusters[he_index] = he_cluster_assign

        new_centroids = calculate_centroids(data, assigned_clusters)

        if check_convergence(new_centroids, centroids, conv_threshold):
            print("Convergence at iteration: ", loop_counter)
            break

        # Calculate the cluster assignments for data points
        centroids[:] = new_centroids[:]
        loop_counter += 1

    return new_centroids, loop_counter


