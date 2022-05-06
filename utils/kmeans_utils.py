import numpy as np
from scipy.spatial import distance
from sklearn.metrics import adjusted_rand_score as ari
from sklearn.metrics.cluster import adjusted_mutual_info_score as amis
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from scipy.special import softmax
from sortedcontainers import SortedDict


def init_centroids(data, num_clusters, seed):

    # Randomly select points from the data as centroids
    np.random.seed(seed)

    indices = np.random.choice(data.shape[0], num_clusters, replace=False, )
    return np.array(data[indices, :])


def calculate_distances(data, centroids):

    # Find pairwise distances
    # dist_mat = distance.cdist(data, centroids, 'euclidean')
    #
    # # Find the closest centroid
    # assigned_cluster = np.argmin(dist_mat, axis=1)
    # distances = np.array(np.min(dist_mat, axis=1))

    n, d = data.shape
    dist_mat = np.zeros((n, len(centroids)), dtype=float)

    for i in range(dist_mat.shape[0]):
        for k in range(len(centroids)):
            dist_mat[i][k] = np.linalg.norm(data[i] - centroids[k])
        # dist[i, :] = np.linalg.norm(data[i, :] - centroids)

    # for k in range(len(centroids)):
    #     # dist_mat[:, k] = np.sqrt(np.sum(np.square(np.subtract(data, centroids[k])), 1))
    #     dist_mat[:, k] = np.linalg.norm(data - centroids[k], axis=1)

    return np.argmin(dist_mat, axis=1), np.round(dist_mat, 5)
    # return assigned_cluster, np.round(distances, 5)


def calculate_distances_specific(data, centroids, old_center, new_center, old_assign):

    # Find pairwise distances
    dist_mat = distance.cdist(data, centroids, 'euclidean')

    # Find the closest centroid
    assigned_cluster = np.argmin(dist_mat, axis=1)
    distances = np.array(np.min(dist_mat, axis=1))

    assigned_cluster[np.where(assigned_cluster == 0)[0]] = old_center
    assigned_cluster[np.where(assigned_cluster == 1)[0]] = new_center

    return assigned_cluster, np.round(distances, 5)


def find_centroids_dist(centroid1, centroids2):
    dist = np.sqrt(np.sum(np.square(centroid1 - centroids2)))
    return np.round(dist, 5)


def calculate_distances123(data, centroids):

    # Find pairwise distances
    dist_mat = distance.cdist(data, centroids, 'euclidean')

    # Find the closest centroid
    assigned_cluster = np.argmin(dist_mat, axis=1)

    return assigned_cluster, np.round(dist_mat, 5)


def calculate_centroids(data, assigned_clusters):

    temp = [np.mean(data[np.where(assigned_clusters == i), ], axis=1)[0] for i in np.sort(np.unique(assigned_clusters))]

    centroids = np.array(temp)
    return np.round(centroids, 4)


def create_sorted_structure(assigned_clusters, distances, num_clusters):

        BST_List = {i:[] for i in range(num_clusters)}

        # Add data to each sublist
        for i in range(len(assigned_clusters)):
            BST_List[assigned_clusters[i]].append([distances[i], i])

        # Create the balanced BST out of each list in the dictionary
        for i in range(num_clusters):
            BST_List[i] = SortedDict(BST_List[i])
        return BST_List


def move_data_around(bst_list, he_bst_indices, he_points, assigned_clusters, new_assigned_clusters,
                     he_new_assigned_centers, he_new_min_dist):

    data_indices = np.where(assigned_clusters[he_points] != he_new_assigned_centers)[0]
    # print(data_indices, len(data_indices))
    # print(he_points, len(he_points))

    if data_indices.size > 0:
        old_assign = assigned_clusters[he_points]
        # print("Debug-1: Data that changed: ", len(data_indices), "out of ", len(assigned_clusters[he_points]))
        # print("old center: ", assigned_clusters[he_points])
        # print("new center: ", he_new_assigned_centers)

        for index in data_indices:

            old_center = old_assign[index]
            new_center = he_new_assigned_centers[index]

            actual_point = he_points[index]

            # print(bst_list[old_center])

            # print("Debug-2: ", he_points[index], he_new_min_dist[index],
            #       he_bst_indices[actual_point],
            #       old_center, new_center)

            if new_assigned_clusters[actual_point] == old_center:
                bst_list[old_center].pop(he_bst_indices[actual_point])
                bst_list[new_center].update({he_new_min_dist[index]: actual_point})
                he_bst_indices[actual_point] = he_new_min_dist[index]
                new_assigned_clusters[actual_point] = new_center
            else:
                if he_new_min_dist[index] < he_bst_indices[actual_point]:
                    old_center = new_assigned_clusters[actual_point]
                    bst_list[old_center].pop(he_bst_indices[actual_point])
                    bst_list[new_center].update({he_new_min_dist[index]: actual_point})
                    he_bst_indices[actual_point] = he_new_min_dist[index]
                    new_assigned_clusters[actual_point] = new_center

    return bst_list, new_assigned_clusters


def move_all_data_around(bst_list, he_bst_indices, he_points, assigned_clusters,
                     he_new_assigned_centers, he_new_min_dist):

    moved_indices = np.where(assigned_clusters[he_points] != he_new_assigned_centers)[0]
    non_moved_indices = np.where(assigned_clusters[he_points] == he_new_assigned_centers)[0]

    # Handling the non moved data
    if non_moved_indices.size > 0:
        # print("Debug-1.0", assigned_clusters[he_points][non_moved_indices], he_points[non_moved_indices])
        for index in non_moved_indices:
            actual_point = he_points[index]
            if he_new_min_dist[index] < he_bst_indices[actual_point]:

                # print("Debug-1.1", "Point:", actual_point, "old: ", assigned_clusters[actual_point],
                #       "new: ", he_new_assigned_centers[index],
                #       "old dist: ", he_bst_indices[actual_point],
                #       "new dist: ", he_new_min_dist[index])
                #
                # for i in range(len(bst_list)):
                #     print(i, bst_list[i])

                old_center = assigned_clusters[actual_point]
                # print(bst_list[old_center])
                bst_list[old_center].pop(he_bst_indices[actual_point])
                bst_list[old_center].update({he_new_min_dist[index]: actual_point})
                he_bst_indices[actual_point] = he_new_min_dist[index]

    # Handling the moved data
    if moved_indices.size > 0:
        old_assign = assigned_clusters[he_points]
        # print("Debug-0:", he_points, len(he_points), len(moved_indices), np.array(he_points)[moved_indices])

        for index in moved_indices:

            old_center = old_assign[index]
            new_center = he_new_assigned_centers[index]

            actual_point = he_points[index]

            # print("Debug-1:", old_center, new_center, actual_point, he_bst_indices[actual_point], he_new_min_dist[index])

            # print(bst_list[old_center])
            # print(bst_list[new_center])

            bst_list[old_center].pop(he_bst_indices[actual_point])
            bst_list[new_center].update({he_new_min_dist[index]: actual_point})
            he_bst_indices[actual_point] = he_new_min_dist[index]

    return bst_list


def calculate_my_distances(point, centroids):

    temp = []
    for i in centroids:
        temp.append(np.sqrt(np.mean(np.square(point-i))))

    return np.argmin(temp), np.min(temp)


def check_convergence(current_centroids, centroids, threshold):

    rms = round(np.sqrt(np.mean(np.square(current_centroids-centroids))), 5)

    # print("RMSD: ", rms)
    if rms <= threshold:
        return True
    return False


def pred_membership(data, centroids):
    dist_mat = distance.cdist(data, centroids, 'euclidean')

    # Find the closest centroid
    assigned_cluster = np.argmin(dist_mat, axis=1).tolist()
    return assigned_cluster


def check_ARI(label1, label2):
    return round(ari(label1, label2), 2)


def check_amis(label1, label2):
    return round(amis(label1, label2), 2)


def test_swaps(assign1, assign2, current_counts):

    indices = np.where(assign1 != assign2)[0].tolist()
    return indices


def check_membership_swaps(assign1, assign2, current_counts):

    indices = np.where(assign1 != assign2)[0].tolist()
    current_counts[indices] = current_counts[indices] + 1

    return current_counts, len(indices), indices


def get_swap_indices(assign1, assign2, current_counts):
    indices = np.where(assign1 != assign2)[0]
    return indices


def find_swap_counts(assign1, assign2, current_counts):

    indices = np.where(assign1 != assign2)[0].tolist()
    current_counts[indices] = current_counts[indices] + 1

    return current_counts, len(indices)


def get_prob_vector(swap_counts):
    return softmax(swap_counts)


def sample_HE_data(assign1, assign2, current_counts, data_indices):

    indices = np.where(assign1 != assign2)[0].tolist()
    current_counts[indices] = current_counts[indices] + 1

    # Get probabilities
    prob_dist = softmax(current_counts)

    # Sample points
    indices = np.random.choice(data_indices, p=prob_dist, size=len(indices)+10, replace=False)

    return current_counts, len(indices), indices #len(np.where(assign1 == assign2)[0])


def check_empty_clusters(assignment, num_clusters):

    counter = 0

    while counter < 4:
        if len(np.unique(assignment)) < num_clusters:
            assignment = np.random.randint(0, len(assignment), num_clusters)
        else:
            break
        counter +=1

    return assignment


def vis_PCA(dataset, labels):

    pca = PCA(n_components=2)
    ss = StandardScaler()

    dataset = pd.DataFrame(dataset)

    dataset = ss.fit_transform(dataset)
    principalComponents = pca.fit_transform(dataset)
    pc_esc = pd.DataFrame(data=principalComponents, columns=['PC1', 'PC2'])

    pc_esc['labels'] = labels

    sns.color_palette("colorblind")
    sns.scatterplot(data=pc_esc, x='PC1', y='PC2', hue="labels")

    plt.show()


def view_3d(data, labels):

    fig = plt.figure()
    ax = fig.add_subplot(projection='3d')
    p1 = data[:,0]
    p2 = data[:,1]
    p3 = data[:,2]
    ax.scatter(p1, p2, p3, c=labels, s=100)
    #ax.plot_surface(X, Y, Z)
    plt.show()


def do_PCA(dataset, centroids1, centroids2, labels, title, file_name):

    pca = PCA(n_components=2)
    ss = StandardScaler()

    dataset = pd.DataFrame(dataset)
    centroids1 = pd.DataFrame(centroids1)
    # centroids2 = pd.DataFrame(centroids2)

    dataset = dataset.append(centroids1, ignore_index=True)
    # dataset = dataset.append(centroids2, ignore_index=True)

    dataset = ss.fit_transform(dataset)
    principalComponents = pca.fit_transform(dataset)
    pc_esc = pd.DataFrame(data=principalComponents, columns=['PC1', 'PC2'])

    temp1 = [str(x) for x in labels]
    temp1 += ["Centroid" for i in range(5)]

    temp2 = [10 for i in range(len(labels))]
    temp2 += [20 for i in range(len(centroids1))]
    # temp2 += [15 for i in range(len(centroids2))]

    print(len(temp1), len(temp2))

    pc_esc['labels'] = temp1
    pc_esc['size'] = temp2

    sns.color_palette("colorblind")
    sns.scatterplot(data=pc_esc, x='PC1', y='PC2', hue="labels", size='size')
    plt.title(title+" points: 2D visualization for data")

    plt.savefig(file_name + ".png")
    plt.show()
    #plt.close()






