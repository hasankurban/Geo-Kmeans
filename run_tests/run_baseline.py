from utils.dataIO import *
from utils.kmeans_utils import *
from base.DCKmeans import *
import time

file_dicts = {0: ['10k_50_5.txt', '10k_50_5_centers.txt'],
              1: ['50k_50_5.txt', '50k_50_5_centers.txt'],
              2: ['100k_50_5.txt', '100k_50_5_centers.txt'],
              3: ['200k_50_5.txt', '200k_50_5_centers.txt'],
              4: ['300k_50_5.txt', '300k_50_5_centers.txt'],
              5: ['400k_50_5.txt', '400k_50_5_centers.txt'],
              6: ['500k_50_5.txt', '500k_50_5_centers.txt']}

seed_array = np.array(np.random.randint(1, 10000, 70)).reshape(7, 10)

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
centroid_diff = []


for i in range(len(file_dicts.keys())):

    data_file = file_dicts[i][0]
    data, labels = read_data(data_file)

    num_clusters = int(data_file.split(".")[0].split("_")[2])
    data_size.append(str(data_file.split(".")[0].split("_")[0]))

    seeds = seed_array[i]

    for i_seed in seeds:

        temp1 = []
        temp2 = []
        temp3 = []
        temp4 = []
        temp5 = []
        temp6 = []
        temp7 = []

        start_time = time.time()
        centroids1, iter1 = DCKmeans(data, num_clusters, threshold, data_threshold, num_iterations, i_seed)
        TraningTime1 = time.time() - start_time

        start_time2 = time.time()
        centroids2, iter2 = DCKmeans(data, num_clusters, threshold, data_threshold, num_iterations, i_seed)
        TraningTime2 = time.time() - start_time2

        m1 = pred_membership(data, centroids1)
        m2 = pred_membership(data, centroids2)

        temp1.append(str(check_ARI(m1, labels)))
        temp2.append(str(check_ARI(m2, labels)))

        temp3.append(str(iter1))
        temp4.append(str(iter2))

        temp5.append(str(round(TraningTime1, 2)))
        temp6.append(str(round(TraningTime2, 2)))

        temp7.append(np.sqrt(np.mean(np.square(centroids1 - centroids2))))

    accuracy1.append(temp1)
    accuracy2.append(temp2)

    iterations1.append(temp3)
    iterations2.append(temp4)

    time1.append(temp5)
    time2.append(temp6)

    centroid_diff.append(temp7)

    clusters.append(str(num_clusters))

accuracy1 = np.mean(np.array(accuracy1), axis=1).tolist()
accuracy2 = np.mean(np.array(accuracy2), axis=1).tolist()

time1 = np.mean(np.array(time1), axis=1).tolist()
time2 = np.mean(np.array(time2), axis=1).tolist()

iter1 = np.mean(np.array(iter1), axis=1).tolist()
iter2 = np.mean(np.array(iter2), axis=1).tolist()

centroid_diff = np.mean(np.array(centroid_diff), axis=1).tolist()


with open("out_res.txt", 'w') as file1:
    file1.write("Size\tNum_clusters\tTime\tIterations\tARI\tCentroid_proximity\n")

    for i in range(len(clusters)):
        file1.write(data_size[i]+"\t"+clusters[i]+"\t"+time1[i]+"\t"+iterations1[i]+"\t"+accuracy1[i]+"\t"+centroid_diff[i]+"\t"+"DCKM"+"\n")

    for i in range(len(clusters)):
        file1.write(data_size[i]+"\t"+clusters[i]+"\t"+time2[i]+"\t"+iterations2[i]+"\t"+accuracy2[i]+"\t"+centroid_diff[i]+"\t"+"KM"+"\n")
