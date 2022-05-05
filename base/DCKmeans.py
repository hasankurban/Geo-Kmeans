from utils.BST import *
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

    while loop_counter <= num_iterations:

        # Re-calculate the centroids
        new_centroid = calculate_centroids(data, assigned_clusters)

        if check_convergence(new_centroid, centroids, threshold):
            print("Kmeans: Convergence at iteration: ", loop_counter)
            break

        # Calculate the cluster assignments for data points
        centroids = new_centroid

        # Calculate the cluster assignments for data points
        assigned_clusters, _ = calculate_distances(data, centroids)
        loop_counter += 1

    return new_centroid, loop_counter


def DCKmeans(data, num_clusters, conv_threshold, leaf_threshold, num_iterations, seed):

    loop_counter = 0

    # Get initial centroids
    centroids = init_centroids(data, num_clusters, seed)

    # Calculate the cluster assignments for data points
    assigned_clusters, distances = calculate_distances(data, centroids)
    converged = False

    while loop_counter <= 4:

        # Re-calculate the centroids
        new_centroids = calculate_centroids(data, assigned_clusters)

        if check_convergence(new_centroids, centroids, conv_threshold):
            print("DCKMeans-Convergence at iteration: ", loop_counter)
            converged = True
            break

        # Calculate the cluster assignments for data points
        centroids = new_centroids

        # Calculate the cluster assignments for data points
        assigned_clusters, distances = calculate_distances(data, centroids)
        loop_counter += 1

    # Create the Balanced Trees
    BST_List = create_BSTs(assigned_clusters, distances, num_clusters, data.shape[0])

    # Extract the high expressive data from the tree structure
    he_index, BST_List = extract_expressive_data(BST_List, num_clusters, leaf_threshold, data)

    if converged is False:

        while loop_counter < num_iterations:

            # Calculate the distances for HE data only
            he_cluster_assign, distances = calculate_distances(data[he_index, :], new_centroids)

            # Update the BSTs to accommodate the new assignments for HE data
            new_he_index, BST_List = update_BST(BST_List, he_index, he_cluster_assign, distances,
                                                num_clusters, leaf_threshold, data)

            # Check for convergence
            he_index = set(he_index)
            diff_he_index = [y for y in new_he_index if y not in he_index]
            dissimilarity = round(float(len(diff_he_index)) / len(he_index), 4)

            if dissimilarity <= 0.01:
                print("DCKMeans-Convergence at iteration: ", loop_counter)
                break

            he_index = new_he_index

            # Update the Tree membership only for HE data
            for i in range(0, len(he_index)):
                assigned_clusters[he_index[i]] = he_cluster_assign[i]

            new_centroids = calculate_centroids(data, assigned_clusters)

            loop_counter += 1

    new_centroids = calculate_centroids(data, assigned_clusters)
    return new_centroids, loop_counter


def DEKmeans_frequentist(data, num_clusters, conv_threshold, num_iterations, seed):

    loop_counter = 0

    # Get initial centroids
    centroids = init_centroids(data, num_clusters, seed)

    # Calculate the cluster assignments for data points
    assigned_clusters, distances = calculate_distances(data, centroids)

    new_assigned_clusters = np.zeros(len(assigned_clusters))
    swap_counts = np.zeros(len(assigned_clusters))
    converged = False

    while loop_counter <= 10:

        # Re-calculate the centroids
        new_centroids = calculate_centroids(data, new_assigned_clusters)

        if check_convergence(new_centroids, centroids, conv_threshold):
            print("DCKMeans-Convergence at iteration: ", loop_counter)
            converged = True
            break

        # Calculate the cluster assignments for data points
        new_assigned_clusters, distances = calculate_distances(data, new_centroids)

        # Calculate the number of swaps
        new_swap_counts, num_points = check_membership_swaps(new_assigned_clusters, assigned_clusters, swap_counts)

        # Reassign the data
        centroids[:] = new_centroids[:]
        swap_counts[:] = new_swap_counts[:]
        assigned_clusters[:] = new_assigned_clusters[:]

        print("iter: ", loop_counter, " #data: ", num_points)

        # Increase counter
        loop_counter += 1


    return new_centroids, loop_counter

def DEKmeans_prob():
    pass


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

            he_index = new_he_index

            # Update the Tree membership only for HE data
            for i in range(0, len(he_index)):
                assigned_clusters[he_index[i]] = he_cluster_assign[i]

            centroids = new_centroids
            new_centroids = calculate_centroids(data, assigned_clusters)

        loop_counter += 1

    new_centroids = calculate_centroids(data, assigned_clusters)
    return new_centroids, loop_counter


def Kmeans_baseline(data, num_clusters, conv_threshold, leaf_threshold, num_iterations, seed):

    loop_counter = 0

    # Get initial centroids
    centroids = init_centroids(data, num_clusters, seed)
    # print(centroids)

    # Calculate the cluster assignments for data points
    assigned_clusters, _ = calculate_distances(data, centroids)

    while loop_counter <= num_iterations:

        # Extract the 30% randomly from each cluster
        he_index = np.random.randint(0, data.shape[0], int(data.shape[0]/leaf_threshold)).tolist()
           
        # Calculate the distances for HE data only
        he_cluster_assign, distances = calculate_distances(data[he_index, :], centroids)
           
        # Update the Tree membership only for HE data
        for i in range(0, len(he_index)):
            assigned_clusters[he_index[i]] = he_cluster_assign[i]
           
        new_centroids = calculate_centroids(data, assigned_clusters)
           
        if check_convergence(new_centroids, centroids, conv_threshold):
            print("Convergence at iteration: ", loop_counter)
            break
           
        # Calculate the cluster assignments for data points
        centroids = new_centroids
        loop_counter += 1

    return new_centroids, loop_counter






