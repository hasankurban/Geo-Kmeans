# import numpy as np
# from sortedcontainers import SortedList


# def create_BSTs(assigned_clusters, distances, num_clusters, num_points):

#     BST_List = {}

#     # Add data to each sublist (each sublist correspond to one Tree structure)
#     for i in range(num_points):
#         if assigned_clusters[i] not in BST_List.keys():
#             BST_List[assigned_clusters[i]] = []
#         else:
#             BST_List[assigned_clusters[i]].append([distances[i], i])

#     # Use BST to sort the data in order
#     for i in range(num_clusters):
#         BST_List[i] = SortedList(BST_List[i])

#     return BST_List


# def extract_expressive_data(BST_List, num_clusters, data_threshold):

#     leaves = [[] for i in range(num_clusters)]

#     for index in range(num_clusters):
#         t = len(BST_List[index]) - int(len(BST_List[index])/data_threshold)
#         leaves[index] = list(BST_List[index].islice(t, len(BST_List[index])))
#         del BST_List[index][t:]
#         BST_List[index] = SortedList(BST_List[index][0:t])

#     # print("Leaves from the function")
#     leaves1 = sum(leaves, [])
#     leaves1 = np.array(leaves1, dtype='int')
#     leaves1 = leaves1[:, 1]

#     return leaves1, BST_List, leaves


# def update_BST(BST_List, he_index, he_cluster_assign, distances, num_clusters, data_threshold):

#     # for clus_index in range(num_clusters):
#     #     data_point_indices = np.where(he_cluster_assign == clus_index)[0].tolist()
#     #
#     #     for index in data_point_indices:
#     #         BST_List[clus_index].add([distances[index], he_index[index]])

#     for i in range(len(he_index)):
#         BST_List[he_cluster_assign[i]].add([distances[i], he_index[i]])

#     he_data_index, BST_List, leaves = extract_expressive_data(BST_List, num_clusters, data_threshold)

#     return he_data_index, BST_List, leaves

