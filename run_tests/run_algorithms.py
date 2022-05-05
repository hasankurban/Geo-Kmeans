from utils.assign_clusters import calc_raw_accuracy
from base.expr_DEKmeans import *
from base.kmeans import *
import time
from sklearn.cluster import KMeans as km


def run_algorithms(data, labels, result_dictionary, num_iterations,
                   threshold, num_clusters, ndims, npoints, i_seed):

    algorithms = ['KMeans', 'DEKMeans-LB', 'DEKMeans-PROB', 'DEKMeans-STO']

    km_start_time = time.time()
    # km_centroids, km_iter = km_clustering(data, num_clusters, num_iterations, threshold, i_seed)
    # km_centroids, km_iter = Kmeans(data, num_clusters, threshold, num_iterations, [], False, i_seed)
    centroids2 = init_centroids(data, num_clusters, i_seed)
    res = km(n_clusters=num_clusters, init=centroids2, tol=threshold).fit(data)
    km_centroids = res.cluster_centers_
    km_iter = res.n_iter_
    km_TraningTime = round(time.time() - km_start_time, 2)

    result_dictionary['Runtime'].append(km_TraningTime)
    result_dictionary['Num_iterations'].append(km_iter)

    # kmtree_start_time = time.time()
    # kmtree_centroids, kmtree_iter = DEKmeans_tree(data, num_clusters, data_threshold,
    #                                               num_iterations, i_seed)
    # # kmtree_centroids, kmtree_iter = kmheap_clustering(data, num_clusters, num_iterations, 0.01, i_seed)
    # kmtree_TraningTime = round(time.time() - kmtree_start_time, 2)

    kmlb_start_time = time.time()
    kmlb_centroids, kmlb_iter = DEKmeans_lb(data, num_clusters, num_iterations, i_seed)
    kmlb_TraningTime = round(time.time() - kmlb_start_time, 2)

    result_dictionary['Runtime'].append(kmlb_TraningTime)
    result_dictionary['Num_iterations'].append(kmlb_iter)

    kmprob_start_time = time.time()
    kmprob_centroids, kmprob_iter = DEKmeans_prob(data, num_clusters, num_iterations, threshold, i_seed)
    kmprob_TraningTime = round(time.time() - kmprob_start_time, 2)

    result_dictionary['Runtime'].append(kmprob_TraningTime)
    result_dictionary['Num_iterations'].append(kmprob_iter)

    kmsto_start_time = time.time()
    kmsto_centroids, kmsto_iter = DEKmeans_sto(data, num_clusters, num_iterations, i_seed)
    kmsto_TraningTime = round(time.time() - kmsto_start_time, 2)

    result_dictionary['Runtime'].append(kmsto_TraningTime)
    result_dictionary['Num_iterations'].append(kmsto_iter)

    km_acc, _ = calc_raw_accuracy(labels, pred_membership(data, km_centroids), data)
    # kmtree_acc, _ = calc_raw_accuracy(labels, pred_membership(data, kmtree_centroids), data)
    kmlb_acc, _ = calc_raw_accuracy(labels, pred_membership(data, kmlb_centroids), data)
    kmsto_acc, _ = calc_raw_accuracy(labels, pred_membership(data, kmsto_centroids), data)
    kmprob_acc, _ = calc_raw_accuracy(labels, pred_membership(data, kmprob_centroids), data)

    result_dictionary['Accuracy'].append(km_acc)
    result_dictionary['Accuracy'].append(kmlb_acc)
    result_dictionary['Accuracy'].append(kmsto_acc)
    result_dictionary['Accuracy'].append(kmprob_acc)
    result_dictionary['Num_clusters'].append(num_clusters)

    km_ari = check_ARI(pred_membership(data, km_centroids), labels)
    # kmtree_ari = check_ARI(pred_membership(data, kmtree_centroids), labels)
    kmlb_ari = check_ARI(pred_membership(data, kmlb_centroids), labels)
    kmsto_ari = check_ARI(pred_membership(data, kmsto_centroids), labels)
    kmprob_ari = check_ARI(pred_membership(data, kmprob_centroids), labels)

    result_dictionary['ARI'].append(km_ari)
    result_dictionary['ARI'].append(kmlb_ari)
    result_dictionary['ARI'].append(kmsto_ari)
    result_dictionary['ARI'].append(kmprob_ari)
    result_dictionary['Num_clusters'].append(num_clusters)

    km_amis = check_amis(pred_membership(data, km_centroids), labels)
    # kmtree_amis = check_amis(pred_membership(data, kmtree_centroids), labels)
    kmlb_amis = check_amis(pred_membership(data, kmlb_centroids), labels)
    kmsto_amis = check_amis(pred_membership(data, kmsto_centroids), labels)
    kmprob_amis = check_amis(pred_membership(data, kmprob_centroids), labels)

    result_dictionary['AMI'].append(km_amis)
    result_dictionary['AMI'].append(kmlb_amis)
    result_dictionary['AMI'].append(kmsto_amis)
    result_dictionary['AMI'].append(kmprob_amis)
    result_dictionary['Num_clusters'].append(num_clusters)

    # kmtree_dev = np.sqrt(np.mean(np.square(km_centroids - kmtree_centroids)))
    kmlb_dev = round(np.sqrt(np.mean(np.square(km_centroids - kmlb_centroids))), 3)
    kmsto_dev = round(np.sqrt(np.mean(np.square(km_centroids - kmsto_centroids))), 3)
    kmprob_dev = round(np.sqrt(np.mean(np.square(km_centroids - kmprob_centroids))), 3)

    result_dictionary['Deviation'].append(0)
    result_dictionary['Deviation'].append(kmlb_dev)
    result_dictionary['Deviation'].append(kmsto_dev)
    result_dictionary['Deviation'].append(kmprob_dev)
    result_dictionary['Num_clusters'].append(num_clusters)

    result_dictionary['Algorithm'] += algorithms

    return result_dictionary

