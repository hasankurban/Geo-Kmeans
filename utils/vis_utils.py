import time

import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from sympy import Point, Line, Plane
from scipy.spatial import distance
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
        dataset.iloc[he_data_indices, dataset.shape[1] - 2] = 'black'
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


def find_points_vis(dataset, centroids_neighbor, centroids, bst_list, dist):

    he_data = []

    for curr_cluster in range(len(bst_list)):

        center1 = centroids[curr_cluster]

        # Determine the sign of other centroid
        for ot_cen in centroids_neighbor[curr_cluster]:

            cutoff_distances = dist[curr_cluster][ot_cen]
            index = bst_list[curr_cluster].bisect_right(cutoff_distances)
            center2 = centroids[ot_cen]

            # Find the vector orthogonal to the vector between centroids
            temp2 = find_ortho_vec(center1, center2)
            temp2 = [temp2[1][0], temp2[1][1]]

            lookup_sign = find_sign(center1, temp2, center2)
            # print("Finding HE data for centroids: ", curr_cluster, ot_cen, lookup_sign)

            # print("Starting index is: ", index)
            for j in range(index, len(bst_list[curr_cluster])):
                temp_index = [bst_list[curr_cluster].peekitem(j)][0]

                temp_index = temp_index[1]
                temp_point = dataset.iloc[temp_index, ].values

                if find_sign(center1, temp2, temp_point) == lookup_sign:

                    he_data += [temp_index]

    return he_data


def find_all_points(dataset, centroids_neighbor, centroids, bst_list, dist):

    he_data = []
    he_bst_index = {}

    for curr_cluster in range(len(bst_list)):

        center1 = centroids[curr_cluster]

        # Determine the sign of other centroid
        for ot_cen in centroids_neighbor[curr_cluster]:

            cutoff_distances = dist[curr_cluster][ot_cen]
            index = bst_list[curr_cluster].bisect_right(cutoff_distances)
            center2 = centroids[ot_cen]

            # Find the vector orthogonal to the vector between centroids
            temp2 = find_ortho_vec(center1, center2)
            temp2 = [temp2[1][0], temp2[1][1]]

            lookup_sign = find_sign(center1, temp2, center2)
            # print("Finding HE data for centroids: ", curr_cluster, ot_cen, lookup_sign)

            # print("Starting index is: ", index)
            for j in range(index, len(bst_list[curr_cluster])):
                temp_index = [bst_list[curr_cluster].peekitem(j)][0]

                # print(temp_index)
                temp_dist = temp_index[0]
                temp_index = temp_index[1]

                temp_point = dataset[temp_index, ]

                if find_sign(center1, temp2, temp_point) == lookup_sign:
                    # he_data[curr_cluster][ot_cen].append(temp_index)
                    he_bst_index[temp_index] = temp_dist
                    he_data += [temp_index]

    return np.unique(he_data), he_bst_index


def find_all_points_1(dataset, midpoints_mat, centroids_neighbor, motion,
                      new_centroids, assigned_clusters, distances):

    he_data = []

    for curr_cluster in range(len(new_centroids)):

        center1 = new_centroids[curr_cluster]
        temp_list_1 = np.where(assigned_clusters == curr_cluster)[0]
        # distances[temp_list_1] += motion[curr_cluster]

        # Determine the sign of other centroid
        for ot_cen in centroids_neighbor[curr_cluster]:

            center2 = new_centroids[ot_cen]

            # Extract points whose distance is more than midpoint
            # x = np.where(distances[temp_list_1] >= midpoints_mat[curr_cluster, ot_cen])[0]
            # temp_list = temp_list_1[x]

            # Find the vector orthogonal to the line passing through centroids
            # m, perp_line = find_ortho_vec(center1, center2, "line")

            # m = find_ortho_vec(center1, center2, "line")
            m = (center1 + center2)/2

            # lookup_sign = find_sign(m, perp_line, center2, "point")
            # print("Finding HE data for centroids: ", curr_cluster, ot_cen, len(temp_list_1), len(temp_list))

            test_data = dataset[temp_list_1, ]
            # point_sign = find_sign(m, perp_line, test_data, "col")

            point_sign = find_sign_by_product(m, center2, test_data)

            # same_sign = np.where(point_sign == lookup_sign)[0]

            same_sign = np.where(point_sign >= 0)[0]
            # on_line_index = list(np.where(point_sign == 0)[0])

            if len(same_sign) > 0:
                same_sign = list(temp_list_1[same_sign])
                he_data += same_sign
                # same_sign = temp_list_1[same_sign]

            # if len(on_line_index) > 0:
            #     on_line_index = list(temp_list[on_line_index])

            # temp_he = same_sign + on_line_index
            # temp_he = same_sign

            # if len(same_sign) > 0:
            #     he_data += same_sign

    return np.unique(he_data)


def find_all_points_nd(dataset, midpoints_mat, centroids_neighbor, new_centroids,
                       assigned_clusters, distances):

    he_data = []

    for curr_cluster in range(len(new_centroids)):

        center1 = new_centroids[curr_cluster]
        temp_list_1 = np.where(assigned_clusters == curr_cluster)[0]

        # Determine the sign of other centroid
        for ot_cen in centroids_neighbor[curr_cluster]:

            center2 = new_centroids[ot_cen]

            # Extract points whose distance is more than midpoint
            x = np.where(distances[temp_list_1] >= midpoints_mat[curr_cluster, ot_cen])[0]
            temp_list = temp_list_1[x]

            # Find the vector orthogonal to the line passing through centroids
            point_on_plane, nv = find_ortho_vec(center1, center2, "plane")

            lookup_sign = find_sign_test(nv, point_on_plane, center2, "point")
            # print("Finding HE data for centroids: ", curr_cluster, ot_cen, "Sign: ", lookup_sign)

            test_data = dataset[temp_list, ]
            point_sign = find_sign_test(nv, point_on_plane, test_data, "plane")

            same_sign = list(np.where(point_sign == lookup_sign)[0])
            on_line_index = list(np.where(point_sign == 0)[0])

            if len(same_sign) > 0:
                same_sign = list(temp_list[same_sign])
            if len(on_line_index) > 0:
                on_line_index = list(temp_list[on_line_index])

            temp_he = same_sign + on_line_index

            if len(temp_he) > 0:
                he_data += temp_he

    return np.unique(he_data)


def find_ortho_vec(p1, p2,  what):

    midpoint = p1 + p2
    midpoint /= 2

    # vec_point = p2-p1
    # norm_vec_point = (vec_point*2)/(np.sqrt(np.sum(np.square(vec_point))))
    # perp = [midpoint[0] + norm_vec_point[1], midpoint[1] - norm_vec_point[0]]

    # if what == "line":
    #     return midpoint, perp
    return midpoint

    # elif what == "plane":
    #     # print(point1, point2, l2)
    #     pl1 = Plane(point1, point2, l2.points[0])
    #     pl2 = pl1.perpendicular_plane(l2.points[0], l2.points[1])
    #     return list(m), pl2.normal_vector


def find_sign(a, b, points, what):

    A = b[0] - a[0]
    C = b[1] - a[1]
    if what == "point":
        B = points[0] - a[0]
        D = points[1] - a[1]
    else:
        B = np.subtract(points[:, 0], a[0])
        D = np.subtract(points[:, 1], a[1])

    return np.sign(np.multiply(A, D) - np.multiply(B, C))


def find_sign_by_product(a, center2, points):

    temp_vec = np.subtract(center2, a)
    points_vec = np.subtract(points, a)

    return np.dot(points_vec, temp_vec)


def find_sign_test(nv, point_on_plane, ot_point, what):

    if what == "point":
        return np.sign(np.dot(nv, ot_point-point_on_plane))
    else:
        return np.sign(np.dot(nv, (ot_point - point_on_plane).T))


def find_all_he_indices_1(dataset, old_centroids, new_centroids, distances, assigned_clusters):

    midpoints, centroids_neighbor, motion = get_midpoints_np(old_centroids, new_centroids, assigned_clusters, distances)

    he_indices = find_all_points_1(dataset, midpoints, centroids_neighbor, motion, new_centroids,
                                                   assigned_clusters, distances)

    # he_indices = find_all_points_nd(dataset, midpoints, centroids_neighbor, new_centroids,
    #                                 assigned_clusters, distances)

    return he_indices


def get_midpoints_np(old_centroids, new_centroids, assigned_clusters, distances):

    centroid_neighbor = {}
    centroid_motion = []

    dist_mat = np.zeros((len(new_centroids), len(new_centroids)), dtype=float)

    # dist_mat = distance.cdist(new_centroids, new_centroids, 'euclidean')

    for k in range(len(new_centroids)):
        # dist_mat[:, k] = np.sqrt(np.sum(np.square(np.subtract(data, centroids[k])), 1))
        dist_mat[:, k] = np.linalg.norm(new_centroids - new_centroids[k], axis=1)

    mid_point_mat = np.divide(dist_mat, 2)


    for i in range(len(new_centroids)):

        centroid_motion.append(np.sqrt(np.sum(np.square(new_centroids-old_centroids))))
        s = np.where(assigned_clusters == i)[0]

        cen1_rad = np.max(distances[s])

        neighbors = np.where(mid_point_mat[i] <= cen1_rad)[0]
        neighbors = list(neighbors[neighbors != i])
        centroid_neighbor[i] = neighbors

    return mid_point_mat, centroid_neighbor, centroid_motion


def get_midpoints_1(new_centroids, assigned_clusters, distances):

    centroid_neighbor = {}

    for i in range(len(new_centroids)):

        if i not in centroid_neighbor.keys():
            centroid_neighbor[i] = []

        for j in range(len(new_centroids)):
            if i != j:
                cen1 = new_centroids[i]
                cen2 = new_centroids[j]

                s = np.where(assigned_clusters == i)[0]
                cen1_rad = np.max(distances[s])

                temp = np.sqrt(np.sum(np.square(cen1 - cen2))) / 2

                # If half the distance between the centroids is less than the radius of the current cluster
                if temp <= cen1_rad:
                    centroid_neighbor[i].append(j)

    return centroid_neighbor


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


