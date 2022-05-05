from utils.assign_clusters import calc_raw_accuracy
from base.expr_DEKmeans import *
# from base.kmeans import *
import time
from sklearn.cluster import KMeans as km


def run_algorithms(data, labels, dataset_name, result_dictionary, num_iterations,
                   epsilon, num_clusters, i_seed):

    data_threshold = 3

    algorithms = ['KMeans', 'DCKMeans']


    ###########
    # Conventional KMeans
    ###########

    km_start_time = time.time()
    km_centroids, km_iter = Kmeans(data, num_clusters, epsilon, num_iterations, [], False, i_seed)
    # km_centroids, km_iter = km_clustering(data, num_clusters, num_iterations, epsilon, i_seed)
    # centroids2 = init_centroids(train_data, num_clusters, i_seed)
    # res = km(n_clusters=num_clusters, init=centroids2, tol=threshold).fit(train_data)
    # km_centroids = res.cluster_centers_
    # km_iter = res.n_iter_
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
    result_dictionary['Dataset'].append(dataset_name)
    result_dictionary['Num_clusters'].append(num_clusters)


    ###########
    # DCKMeans
    ###########

    dckm_start_time = time.time()
    dckm_centroids, dckm_iter = DEKmeans_sto(data, num_clusters, num_iterations, i_seed)
    dckm_TraningTime = round(time.time() - dckm_start_time, 2)

    result_dictionary['Runtime'].append(dckm_TraningTime)
    result_dictionary['Num_iterations'].append(dckm_iter)
    result_dictionary['Dataset'].append(dataset_name)
    result_dictionary['Num_clusters'].append(num_clusters)

    dckm_acc, _ = calc_raw_accuracy(labels, pred_membership(data, dckm_centroids), data)
    dckm_ari = check_ARI(pred_membership(data, dckm_centroids), labels)
    dckm_amis = check_amis(pred_membership(data, dckm_centroids), labels)
    dckm_dev = round(np.sqrt(np.mean(np.square(km_centroids - dckm_centroids))), 3)

    result_dictionary['Accuracy'].append(dckm_acc)
    result_dictionary['ARI'].append(dckm_ari)
    result_dictionary['AMI'].append(dckm_amis)
    result_dictionary['Deviation'].append(dckm_dev)

    result_dictionary['Algorithm'] += algorithms

    return result_dictionary

