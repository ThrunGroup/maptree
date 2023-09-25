#!/usr/bin/env python
# script to create a pickle file with the dataset in a dictionary
# the script uses *_train for training and *_valid as test
# Example usage: ./process_data_feat_challenge madelon

import numpy as np
import pickle as pickle
import sys


name = sys.argv[1]
DATA_PATH = name + '/'      # tweak this if process_data_feat_challenge is in the same directory
# DATA_PATH = ''

data = {}

def load_files(name, tag):
    x = np.loadtxt(DATA_PATH + name + '_' + tag + '.data', dtype='float')
    y = np.loadtxt(DATA_PATH + name + '_' + tag + '.labels', dtype='float')
    y = (y + 1) / 2.0
    y = y.astype('int')
    return (x, y)

x_train, y_train = load_files(name, 'train')
x_test, y_test = load_files(name, 'valid')      # labels are not available for test data

data['n_dim'] = x_train.shape[1]

n_train = len(y_train)
n_test = len(y_test)
data['n_train'] = n_train
data['n_test'] = n_test
data['y_train'] = y_train
data['y_test'] = y_test
data['x_train'] = x_train
data['x_test'] = x_test
data['n_class'] = len(np.unique(y_train))
data['is_sparse'] = False

print('dataset statistics:')
print('n_train = %d, n_test = %d, n_dim = %d' % (n_train, n_test, data['n_dim']))

pickle.dump(data, open(DATA_PATH + name + ".p", "wb"), protocol=pickle.HIGHEST_PROTOCOL)
