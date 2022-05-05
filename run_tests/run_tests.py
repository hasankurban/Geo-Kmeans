from utils.dataIO import *
from utils.kmeans_utils import *
from base.DCKmeans import *
import time
from sklearn.cluster import KMeans

file_dicts = {0: ['10k_50_5.txt', '10k_50_5_centers.txt'],
              1: ['50k_50_5.txt', '50k_50_5_centers.txt'],
              2: ['100k_50_5.txt', '100k_50_5_centers.txt'],
              3: ['200k_50_5.txt', '200k_50_5_centers.txt'],
              4: ['300k_50_5.txt', '300k_50_5_centers.txt'],
              5: ['400k_50_5.txt', '400k_50_5_centers.txt'],
              6: ['500k_50_5.txt', '500k_50_5_centers.txt'],
              7: ['500k_50_20.txt', '500k_50_20_centers.txt']}


# seed_array = np.array(np.random.randint(1, 10000, 70)).reshape(7, 10)
seeds = [546, 123, 4, 67, 874, 32, 412, 65478]

# Set parameters
threshold = 0.0001
num_iterations = 100
data_threshold = 3

clusters = []
time1 = []
time2 = []
iterations1 = []
iterations2 = []
accuracy1 = []
accuracy2 = []
data_size = []

sample_data_path = '/Users/schmuck/Documents/Box Sync/Ph.D./KDD_2021/sample_data'


# for i in range(len(file_dicts.keys())):
for i in range(1):

    data_file = file_dicts[i][0]
    if data_file not in ["10k_50_5.txt"]:
        continue

    data, labels = read_data(os.path.join(sample_data_path, data_file))

    # data, labels = read_crop_data(os.path.join(sample_data_path, "crop_320.csv"), False)
    # num_clusters = 5

    num_clusters = int(data_file.split(".")[0].split("_")[2])
    data_size.append(str(data_file.split(".")[0].split("_")[0]))

    start_time = time.time()
    centroids1, iter1 = DEKmeans_frequentist(data, num_clusters, threshold, num_iterations, seeds[i])
    TraningTime1 = time.time() - start_time

    # centroids2 = init_centroids(data, num_clusters, seeds[i])

    # start_time2 = time.time()
    # centroids2, iter2 = Kmeans(data, num_clusters, threshold, num_iterations, centroids1, False, seeds[i])
    # out = KMeans(n_clusters=num_clusters, init=centroids2).fit(data)
    # TraningTime2 = time.time() - start_time2

    # iter2 = out.n_iter_
    # centroids2 = out.cluster_centers_
    #
    # print("Distance between centroids: ", np.sqrt(np.mean(np.square(centroids1 - centroids2))))
    #
    # clusters.append(str(num_clusters))

    m1 = pred_membership(data, centroids1)
    # m2 = pred_membership(data, centroids2)

    accuracy1.append(str(check_ARI(m1, labels)))
    # accuracy2.append(str(check_ARI(m2, labels)))

    iterations1.append(str(iter1))
    # iterations2.append(str(iter2))

    time1.append(str(round(TraningTime1, 2)))
    # time2.append(str(round(TraningTime2, 2)))

    # do_PCA(data, centroids1, centroids2, labels, str(data_file.split(".")[0].split("_")[0]), data_file.split(".")[0])
    # do_PCA(data, centroids1, centroids2, labels, "hello", "myfile")


print(iterations1)
print(accuracy1)
print(time1)

# print(iterations1, "\t", iterations2)
# print(accuracy1, "\t", accuracy2)
# print(time1, "\t", time2)


# with open("out_res.txt", 'w') as file1:
#     file1.write("Size\tNum_clusters\tTime\tIterations\tARI\n")
#
#     for i in range(len(clusters)):
#         file1.write(data_size[i]+"\t"+clusters[i]+"\t"+time1[i]+"\t"+iterations1[i]+"\t"+accuracy1[i]+"\t"+"DCKM"+"\n")
#
#     for i in range(len(clusters)):
#         file1.write(data_size[i]+"\t"+clusters[i]+"\t"+time2[i]+"\t"+iterations2[i]+"\t"+accuracy2[i]+"\t"+"KM"+"\n")
