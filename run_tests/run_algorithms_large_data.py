import sys
sys.path.append("..")

from utils.assign_clusters import calc_raw_accuracy
from base.expr_DEKmeans import *
from base.kmeans import *
import time


def run_algorithms(data, labels, result_dictionary, num_iterations,
                   epsilon, num_clusters, dataset_name, i_seed):

    algorithms = ['KMeans', 'DCKMeans']


    ###########
    # Conventional KMeans
    ###########

    km_start_time = time.time()
    km_centroids, km_iter = Kmeans(data, num_clusters, epsilon, num_iterations, [], False, i_seed)
    km_TraningTime = round(time.time() - km_start_time, 2)

    km_acc, _ = calc_raw_accuracy(labels, pred_membership(data, km_centroids), data)
    km_ari = check_ARI(pred_membership(data, km_centroids), labels)
    km_amis = check_amis(pred_membership(data, km_centroids), labels)

    result_dictionary['Accuracy'].append(km_acc)
    result_dictionary['ARI'].append(km_ari)
    result_dictionary['AMI'].append(km_amis)
    result_dictionary['Deviation'].append(0)

    result_dictionary['Runtime'].append(km_TraningTime)
    result_dictionary['Num_iterations'].append(km_iter)
    result_dictionary['Num_Points'].append(dataset_name)


    #$#########
    # DCKMeans
    ###########

    dckm_start_time = time.time()
    dckm_centroids, dckm_iter = DCKMeans(data, num_clusters, epsilon, num_iterations, i_seed)
    dckm_TraningTime = round(time.time() - dckm_start_time, 2)

    result_dictionary['Runtime'].append(dckm_TraningTime)
    result_dictionary['Num_iterations'].append(dckm_iter)
    result_dictionary['Num_Points'].append(dataset_name)

    dckm_acc, _ = calc_raw_accuracy(labels, pred_membership(data, dckm_centroids), data)
    dckm_ari = check_ARI(pred_membership(data, dckm_centroids), labels)
    dckm_amis = check_amis(pred_membership(data, dckm_centroids), labels)
    dckm_dev = round(np.sqrt(np.mean(np.square(km_centroids - dckm_centroids))), 3)

    result_dictionary['Accuracy'].append(dckm_acc)
    result_dictionary['ARI'].append(dckm_ari)
    result_dictionary['AMI'].append(dckm_amis)
    result_dictionary['Deviation'].append(dckm_dev)


    return result_dictionary

