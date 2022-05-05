import numpy as np
import pandas as pd
import os

def read_data(input_loc):

    data = pd.read_csv(input_loc, header=0, sep="\t")

    # Get the label column from the data
    labels = list(data['labels'].values)
    data.drop(['labels'], inplace=True, axis=1)

    # Subset the feature columns from the data
    data = np.array(data, dtype=float)

    # Cast labels to a numpy array
    labels = np.array(labels)
    print("Data shape: ", data.shape)
    return data, labels


def read_crop_data(input_loc, raw):

    if raw:
        seperator = ","
    else:
        seperator = "\t"

    data = pd.read_csv(input_loc, header=0, sep=seperator)
    data = data.replace('?', np.NaN)
    data = data.dropna()

    # Get the label column from the data
    labels = list(data['label'].values)
    data.drop(['label'], inplace=True, axis=1)

    # Subset the feature columns from the data
    data = np.array(data, dtype=float)

    # Cast labels to a numpy array
    labels = np.array(labels)
    print("Data shape: ", data.shape)
    return data, labels


def read_simulated_data(file_path):

    data = pd.read_csv(file_path, sep=",")
    labels = data['labels'].to_list()
    data.drop(["labels"], inplace=True, axis=1)

    return np.array(data), np.array(labels)


def write_result_data(result, output_loc, result_type):

    if result_type == "clustering_results":
        header = "Clusters,Time,Iterations,ARI,Algorithm\n"
        outfile = "clustering_benchmark.csv"
    elif result_type == "scal_results":
        header = "Num_Points,Time,Iterations,ARI,Algorithm\n"
        outfile = "scalability_benchmark.csv"
    elif result_type == "dims_results":
        header = "Dimensions,Time,Iterations,ARI,Algorithm\n"
        outfile = "dimsionality_benchmark.csv"

    with open(os.path.join(output_loc, outfile), "w") as file:

        file.write(header)

        for i in range(len(result)):
            file.write(str(result[i][0])+","+str(result[i][1])+","+str(result[i][2]) + "," +
                              str(result[i][3])+","+result[i][4] + "\n")

    print(result_type, ": File written to disk")


def read_song_data(input_loc):

    data = pd.read_csv(input_loc, header=None, sep=",")
    data = data.replace('?', np.NaN)
    data = data.dropna()

    # Get the label column from the data
    train_data = data.iloc[0:463715, ].copy()
    test_data = data.iloc[463715:, ].copy()

    train_labels = train_data.iloc[:, 0].to_list()
    test_labels = test_data.iloc[:, 0].to_list()

    train_labels = np.array(train_labels)
    test_labels = np.array(test_labels)

    train_data.drop(train_data.columns[0], inplace=True, axis=1)
    test_data.drop(test_data.columns[0], inplace=True, axis=1)

    # Subset the feature columns from the data
    train_data = np.array(train_data, dtype=float)
    test_data = np.array(test_data, dtype=float)

    temp_index = [i for i in range(len(train_labels)) if train_labels[i] in test_labels]

    train_labels = train_labels[temp_index]
    train_data = train_data[temp_index, ]

    u_train_labels = np.sort(np.unique(train_labels))
    u_test_labes = np.sort(np.unique(test_labels))

    for i in range(len(u_train_labels)):
        train_labels[np.where(train_labels == u_train_labels[i])] = i

    for i in range(len(u_test_labes)):
        test_labels[np.where(test_labels == u_test_labes[i])] = i

    # print(len(train_labels), train_data.shape)

    return train_data, train_labels, test_data, test_labels


def read_real_data(data_dir_loc, dataset_name):

    if dataset_name == "spambase":
        label_col = 57
    elif dataset_name == "magicgamma":
        label_col = 11

    pass


def read_cropland_data(input_loc, raw):

    if raw:
        seperator = ","
    else:
        seperator = "\t"

    data = pd.read_csv(input_loc, header=0, sep=seperator)
    data = data.replace('?', np.NaN)
    data= data.dropna()

    # Get the label column from the data
    labels = list(data['label'].values)
    data.drop(['label'], inplace=True, axis=1)

    # Subset the feature columns from the data
    data = np.array(data, dtype=float)

    # Cast labels to a numpy array
    labels = np.array(labels)
    print("Data shape: ", data.shape)
    return data, labels

