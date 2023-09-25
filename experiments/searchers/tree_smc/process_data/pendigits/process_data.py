#!/usr/bin/env python
# script to create a pickle file with the dataset in a dictionary

import numpy as np
import pickle as pickle

name = 'pendigits'

data = {}

tmp = np.loadtxt(name + '.tra', dtype='float', delimiter=',')
y = tmp[:, -1]
x = tmp[:, :-1]
y = y.astype('int')
data['x_train'] = x
data['n_train'] = x.shape[0]
data['y_train'] = y
tmp = np.loadtxt(name + '.tes', dtype='float', delimiter=',')
y = tmp[:, -1]
x = tmp[:, :-1]
y = y.astype('int')
data['x_test'] = x
data['n_test'] = x.shape[0]
data['y_test'] = y
data['n_dim'] = tmp.shape[1] - 1        # last column contains labels
data['n_class'] = len(np.unique(y))
data['is_sparse'] = False

pickle.dump(data, open(name + ".p", "wb"), protocol=pickle.HIGHEST_PROTOCOL)
