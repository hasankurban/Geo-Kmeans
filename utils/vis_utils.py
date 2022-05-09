import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from scipy.spatial.distance import cdist
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler


def find_centroids_dist(centroid1, centroids2):
    dist = np.sqrt(np.sum(np.square(centroid1 - centroids2)))
    return np.round(dist, 5)


def vis_data_with_he(data, centroids, assigned_clusters, distances,
                         loop_counter, data_changed, he_data_indices):

    dataset = pd.DataFrame(data, columns=['x', 'y'])
    temp = pd.DataFrame(centroids, columns=['x', 'y'])
    dataset = dataset.append(temp, ignore_index=True)

    temp1 = [str(x) for x in assigned_clusters]
    temp1 += ["Centroid" for i in range(centroids.shape[0])]

    temp2 = [10 for i in range(len(assigned_clusters))]
    temp2 += [20 for i in range(centroids.shape[0])]

    dataset['labels'] = temp1
    dataset['size'] = temp2

    x_vals = []
    y_vals = []

    dist = {}
    mid_points = []
    centroid_neighbor = {}

    for i in range(len(centroids)):

        s = np.where(assigned_clusters == i)[0]
        cen1_rad = np.max(distances[s])

        for j in range(len(centroids)):
            if i != j:
                cen1 = centroids[i]
                cen2 = centroids[j]

                mid_points.append([(cen1[0] + cen2[0]) / 2, (cen1[1] + cen2[1]) / 2])

                if i not in dist.keys():
                    dist[i] = {j: np.sqrt(np.sum(np.square(cen1 - cen2))) / 2}
                else:
                    dist[i][j] = np.sqrt(np.sum(np.square(cen1 - cen2))) / 2

                # If half the distance between the centroids is less than the radius of the current cluster
                if dist[i][j] < cen1_rad:
                    if i not in centroid_neighbor.keys():
                        centroid_neighbor[i] = [j]
                    else:
                        centroid_neighbor[i].append(j)

    # mark HE data separately
    if len(he_data_indices) != 0:
        dataset.iloc[he_data_indices, dataset.shape[1] - 2] = 'HE-Data'
    # exit()

    plt.axis("equal")
    ax = sns.scatterplot(data=dataset, x='x', y='y', hue="labels", size='size')
    for i in range(len(centroids)):
        x_vals += [centroids[i][0]]
        y_vals += [centroids[i][1]]
        plt.text(x=centroids[i][0], y=centroids[i][1], s=str(i))

    plt.plot(x_vals, y_vals)

    for i in data_changed:
        temp = dataset.iloc[i, 0:2].values
        plt.text(x=temp[0], y=temp[1], s=str(i))

    for i in centroid_neighbor.keys():
        for j in centroid_neighbor[i]:
            temp = dataset.loc[dataset['labels'] == 'Centroid'][['x', 'y']].values[i]
            d = plt.Circle(xy=(temp[0], temp[1]), radius=dist[i][j], color='green', fill=False)
            ax.add_patch(d)
            plt.plot(x_vals, y_vals)
            # plt.axvline(x=mid_points[i][0])
            # plt.axhline(y=mid_points[i][1])

    plt.title(str(loop_counter) + " 2D visualization for data")
    plt.savefig(str(loop_counter) + ".png")
    plt.show()


def vis_data_with_he_test(data, centroids, assigned_clusters, distances,
                         loop_counter, data_changed, he_data_indices):

    cols = ["col"+str(i) for i in range(data.shape[1])]

    dataset = pd.DataFrame(data)
    dataset.columns = [cols]
    temp = pd.DataFrame(centroids)
    temp.columns = [cols]

    pca = PCA(n_components=2)
    ss = StandardScaler()

    dataset = dataset.append(temp, ignore_index=True)

    dataset = ss.fit_transform(dataset)
    principalComponents = pca.fit_transform(dataset)
    pc_esc = pd.DataFrame(data=principalComponents, columns=['PC1', 'PC2'])

    temp1 = [str(x) for x in assigned_clusters]
    temp1 += ["Centroid" for i in range(centroids.shape[0])]

    temp2 = [10 for i in range(len(assigned_clusters))]
    temp2 += [20 for i in range(centroids.shape[0])]

    pc_esc['labels'] = temp1
    pc_esc['size'] = temp2

    dist = {}
    mid_points = []
    centroid_neighbor = {}

    for i in range(len(centroids)):

        s = np.where(assigned_clusters == i)[0]
        cen1_rad = np.max(distances[s])

        for j in range(len(centroids)):
            if i != j:
                cen1 = centroids[i]
                cen2 = centroids[j]

                mid_points.append([(cen1[0] + cen2[0]) / 2, (cen1[1] + cen2[1]) / 2])

                if i not in dist.keys():
                    dist[i] = {j: np.sqrt(np.sum(np.square(cen1 - cen2))) / 2}
                else:
                    dist[i][j] = np.sqrt(np.sum(np.square(cen1 - cen2))) / 2

                # If half the distance between the centroids is less than the radius of the current cluster
                if dist[i][j] < cen1_rad:
                    if i not in centroid_neighbor.keys():
                        centroid_neighbor[i] = [j]
                    else:
                        centroid_neighbor[i].append(j)

    # mark HE data separately
    if len(he_data_indices) != 0:
        pc_esc.iloc[he_data_indices, pc_esc.shape[1] - 2] = 'black'

    sns.scatterplot(data=pc_esc, x='PC1', y='PC2', hue="labels", size='size')

    for i in data_changed:
        temp = pc_esc.iloc[i, 0:2].values
        plt.text(x=temp[0], y=temp[1], s=str(i))

    plt.title("PCA_"+str(loop_counter) + " 2D visualization for data")
    plt.savefig("PCA_"+str(loop_counter) + ".png")
    plt.show()



def find_all_points(dataset, centroids_neighbor, new_centroids, assign_dict):

    he_data = []

    for curr_cluster in range(len(new_centroids)):

        center1 = new_centroids[curr_cluster]
        temp_list_1 = np.array(assign_dict[curr_cluster])
        # distances[temp_list_1] += motion[curr_cluster]

        # Determine the sign of other centroid
        for ot_cen in centroids_neighbor[curr_cluster]:

            if curr_cluster != ot_cen:

                center2 = new_centroids[ot_cen]

                # Extract points whose distance is more than midpoint
                #x = np.where(distances[temp_list_1] > midpoints_mat[curr_cluster, ot_cen])[0]
                # temp_list = temp_list_1[x]
                # print(len(temp_list_1), len(temp_list))

                mid_point = np.divide(np.add(center1, center2), 2)

                test_data = dataset[temp_list_1, ]
                point_sign = find_sign_by_product(mid_point, center2, test_data)
                same_sign = np.where(point_sign >= 0)[0]

                #if len(same_sign) > 0:
                # same_sign = list(temp_list_1[same_sign])
                he_data += list(temp_list_1[same_sign])


    return np.unique(he_data)


def find_sign_by_product(mid_point, center2, points):

    temp_vec = np.subtract(center2, mid_point)
    points_vec = np.subtract(points, mid_point)
    return points_vec.dot(temp_vec)


def find_sign_test(nv, point_on_plane, ot_point, what):

    if what == "point":
        return np.sign(np.dot(nv, ot_point-point_on_plane))
    else:
        return np.sign(np.dot(nv, (ot_point - point_on_plane).T))


def find_all_he_indices_1(dataset, new_centroids, distances, assign_dict):

    midpoints, centroids_neighbor = get_midpoints_np(new_centroids, assign_dict, distances)
    he_indices = find_all_points(dataset, centroids_neighbor, new_centroids, assign_dict)

    return he_indices


def get_midpoints_np(new_centroids, assign_dict, distances):

    centroid_neighbor = {}

    # dist_mat = np.zeros((len(new_centroids), len(new_centroids)), dtype=float)

    dist_mat = cdist(new_centroids, new_centroids)

    # for k in range(len(new_centroids)):
    #     # dist_mat[:, k] = np.sqrt(np.sum(np.square(np.subtract(data, centroids[k])), 1))
    #     dist_mat[:, k] = np.sqrt(np.sum(np.square(np.subtract(new_centroids, new_centroids[k])), 1))

    dist_mat = np.divide(dist_mat, 2)

    for i in range(len(new_centroids)):

        s = assign_dict[i]

        cen1_rad = np.max(distances[s])
        neighbors = np.where(dist_mat[i] <= cen1_rad)[0]

        #neighbors = list(neighbors[neighbors != i])

        centroid_neighbor[i] = neighbors

    return dist_mat, centroid_neighbor


def get_membership(assigned_clusters, assign_dict, num_clusters):

    for i in range(num_clusters):
        assign_dict[i] = np.where(assigned_clusters == i)[0].tolist()

    return assign_dict


def view_3d(data, centroids, assigned_clusters):

    cols = ["col" + str(i) for i in range(data.shape[1])]

    dataset = pd.DataFrame(data)
    dataset.columns = [cols]
    temp = pd.DataFrame(centroids)
    temp.columns = [cols]

    num = len(centroids)

    labels = [i for i in assigned_clusters]
    labels += [num for i in range(len(centroids))]

    dataset = dataset.append(temp, ignore_index=True)

    fig = plt.figure()
    ax = fig.add_subplot(projection='3d')
    p1 = dataset.iloc[:, 0]
    p2 = dataset.iloc[:, 1]
    p3 = dataset.iloc[:, 2]
    ax.scatter(p1, p2, p3, c=labels, s=100)
    plt.show()


