import numpy as np
from sortedcontainers import SortedList

def create_BSTs(assigned_clusters, distances, num_clusters, num_points):

    BST_List = {}

    # Add data to each sublist (each sublist correspond to one Tree structure)
    for i in range(num_points):
        if assigned_clusters[i] not in BST_List.keys():
            BST_List[assigned_clusters[i]] = []
        else:
            BST_List[assigned_clusters[i]].append([distances[i], i])

    # Use BST to sort the data in order
    for i in range(num_clusters):
        BST_List[i] = SortedList(BST_List[i])

    return BST_List


def extract_expressive_data(BST_List, num_clusters, data_threshold, data):

    leaves = [[] for i in range(num_clusters)]
    temp2 = []

    for index in range(num_clusters):
        # High expression data
        # print("Before removing the leaves: ", len(BST_List[index]))
        t = len(BST_List[index]) - int(len(BST_List[index])/data_threshold)
        leaves[index] = list(BST_List[index].islice(t, len(BST_List[index])))
        del BST_List[index][t:]
        # print("After removing the leaves: ", len(BST_List[index]))

        # Low expression data
        # BST_List[index] = SortedList(list(BST_List[index].islice(0, int(len(BST_List[index])/data_threshold))))

    # print("Leaves from the function")
    leaves = sum(leaves, [])
    leaves = np.array(leaves, dtype='int')

    return leaves[:, 1], BST_List


def update_BST(BST_List, he_index, he_cluster_assign, distances, num_clusters, data_threshold, data):

    for i in range(len(he_index)):
        BST_List[he_cluster_assign[i]].add([distances[i], he_index[i]])

    he_data_index, BST_List = extract_expressive_data(BST_List, num_clusters, data_threshold, data)

    return he_data_index, BST_List#, #temp

