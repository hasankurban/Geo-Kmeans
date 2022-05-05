# @Parichit Sharma, 2022, IUB, Department of Computer Science

import numpy as np
from scipy.spatial.distance import cdist


def calc_raw_accuracy(true_labels, predicted_labels, data):

    # unique classes
    unique_labels = np.sort(np.unique(true_labels))

    # number of unique clusters
    num_classes = len(unique_labels)

    # to store mean of the true clusters
    true_means = np.zeros((num_classes, data.shape[1]), dtype=float)

    # means for true clusters
    for i in range(len(unique_labels)):
        data_indices = np.where(true_labels == unique_labels[i])[0]
        true_means[i] = np.mean(data[data_indices, :], 1)[0]


    #THIS PART IS FOR PREDICTION, HANDLING PREDICTIONS

    unique_pred_labels = np.sort(np.unique(predicted_labels))
    num_pred_classes = len(unique_pred_labels)

    pred_means = np.zeros((num_pred_classes, data.shape[1]), dtype=float)

    # Calculating the mean for test data based on predicted classes
    for j in range(len(unique_pred_labels)):
        data_indices = np.where(unique_pred_labels == unique_pred_labels[j])[0]
        pred_means[j] = np.mean(data[data_indices, :], 1)[0]


    # Arrange cluster numbers
    dist_mat = cdist(pred_means, true_means, 'euclidean')

    # Reassign membership in the clustered data based on the closest centroid
    assigned_cluster = np.argmin(dist_mat, axis=1).tolist()

    reassign_predictions = np.zeros(len(predicted_labels))

    for i in range(len(unique_pred_labels)):
        label_indices = np.where(predicted_labels == unique_pred_labels[i])[0]
        reassign_predictions[label_indices] = assigned_cluster[i]

    accuracy = float(np.sum(true_labels == reassign_predictions)) / len(predicted_labels)
    accuracy = round(accuracy, 2)

    return round(accuracy, 2), reassign_predictions