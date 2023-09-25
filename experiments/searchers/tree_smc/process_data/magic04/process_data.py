#!/usr/bin/env python
# script to create a pickle file with the dataset in a dictionary
# the script uses FRACTION_TRAINING for training and remaining as test data

import numpy as np
import pickle as pickle
import random
random.seed(123456789)          # fix for reproducibility

name = 'magic04'
DATA_PATH = '/'
FRACTION_TRAINING = 0.7

data = {}
filename = DATA_PATH + name + '.data'
x = np.loadtxt(filename, dtype='float', delimiter=',', usecols = list(range(0, 10)))
n = x.shape[0]
print('ndim = %d' % x.shape[1])
y = np.zeros(n, dtype = 'int')
for i, line in enumerate(open(filename, 'r')):
    a = line.rstrip('\n').split(',')[-1]
    if a == 'g':
        y[i] = 1 
    elif a == 'h':
        y[i] = 0 
    else:
        print('Unknown string %s' % a)
        print(line)
        raise Exception

y = y.astype('int')
data['n_dim'] = x.shape[1] 
idx = list(range(n))
random.shuffle(idx)

n_train = int(FRACTION_TRAINING * n)
idx_train = idx[:n_train]
idx_test = idx[n_train:]
data['n_train'] = n_train
data['n_test'] = len(idx_test)
data['y_train'] = y[idx_train]
data['y_test'] = y[idx_test]
data['x_train'] = x[idx_train, :]
data['x_test'] = x[idx_test, :]
data['n_class'] = len(np.unique(y))
data['is_sparse'] = False

#pickle.dump(data, open(DATA_PATH + name + ".p", "wb"))
pickle.dump(data, open(DATA_PATH + name + ".p", "wb"), protocol=pickle.HIGHEST_PROTOCOL)
