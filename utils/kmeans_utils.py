import numpy as np
from scipy.spatial import distance
from sklearn.metrics import adjusted_rand_score as ari
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt


def init_centroids(data, num_clusters, seed):
    # Randomly select points from the data as centroids
    np.random.seed(seed)
    indices = np.random.choice(data.shape[0], num_clusters, replace=False)
    return np.array(data[indices, :])


def calculate_distances(data, centroids):

    # Find pairwise distances
    dist_mat = distance.cdist(data, centroids, 'euclidean')

    # Find the closest centroid
    assigned_cluster = np.argmin(dist_mat, axis=1)
    distances = np.array(np.min(dist_mat, axis=1))

    return assigned_cluster, distances


def calculate_centroids(data, assigned_clusters):
    temp = []
    # Re-calculate centroids for each cluster
    # for i in np.unique(assigned_clusters):
    #     temp.append(np.mean(data[np.where(assigned_clusters == i), ], axis=1)[0])

    temp = [np.mean(data[np.where(assigned_clusters == i), ], axis=1)[0] for i in np.unique(assigned_clusters)]

    centroids = np.array(temp)
    return centroids


def check_convergence(current_centroids, centroids, threshold):
    rms = np.sqrt(np.mean(np.square(current_centroids-centroids)))
    # print("RMS: ", rms)
    if rms <= threshold:
        return True
    return False


def pred_membership(data, centroids):
    dist_mat = distance.cdist(data, centroids, 'euclidean')

    # Find the closest centroid
    assigned_cluster = np.argmin(dist_mat, axis=1).tolist()
    return assigned_cluster


def check_ARI(m1, m2):
    return round(ari(m1, m2), 2)


def check_membership_swaps(assign1, assign2, current_counts):

    indices = np.where(assign1 != assign2)[0]
    current_counts[indices] += 1
    return current_counts, len(indices)


def do_PCA(dataset, centroids1, centroids2, labels, title, file_name):

    pca = PCA(n_components=2)
    ss = StandardScaler()

    dataset = pd.DataFrame(dataset)
    centroids1 = pd.DataFrame(centroids1)
    centroids2 = pd.DataFrame(centroids2)

    dataset = dataset.append(centroids1, ignore_index=True)
    dataset = dataset.append(centroids2, ignore_index=True)

    dataset = ss.fit_transform(dataset)
    principalComponents = pca.fit_transform(dataset)
    pc_esc = pd.DataFrame(data=principalComponents, columns=['PC1', 'PC2'])

    temp1 = [str(x) for x in labels]
    temp1 += ['DCKM' for i in range(len(centroids1))]
    temp1 += ['KM' for i in range(len(centroids1))]

    temp2 = [10 for i in range(len(labels))]
    temp2 += [35 for i in range(len(centroids1))]
    temp2 += [15 for i in range(len(centroids2))]

    # print(len(temp1), len(temp2))

    pc_esc['labels'] = temp1
    pc_esc['size'] = temp2

    sns.color_palette("colorblind")
    sns.scatterplot(data=pc_esc, x='PC1', y='PC2', hue="labels", size='size')
    plt.title(title+" points: 2D visualization for data")

    #plt.show()
    plt.savefig(file_name+".png")
    plt.close()






