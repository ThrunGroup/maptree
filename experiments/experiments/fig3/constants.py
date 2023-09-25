import numpy as np

NUM_SAMPLE_SIZE_VALUES = 10
SAMPLE_SIZE_VALUES = list(np.round(np.linspace(100, 1000, NUM_SAMPLE_SIZE_VALUES)).astype(np.int32))
NOISE_VALUES = [0.0, 0.1, 0.25]

TIME_LIMIT = 60
RHO = [2.5, 2.5]

CART_PARAMS_LIST = [
    {'max_depth': 2},
    {'max_depth': 3},
    {'max_depth': 4},
    {'max_depth': 5},
    {'max_depth': 6},
    {'max_depth': 7},
    {'max_depth': 8},
]
DL85_PARAMS_LIST = [
    {'max_depth': 2, 'time_limit': TIME_LIMIT},
    {'max_depth': 3, 'time_limit': TIME_LIMIT},
    {'max_depth': 4, 'time_limit': TIME_LIMIT},
    {'max_depth': 5, 'time_limit': TIME_LIMIT},
    {'max_depth': 6, 'time_limit': TIME_LIMIT},
]
GOSDT_PARAMS_LIST = [
    {'regularization': 0.03125, 'time_limit': TIME_LIMIT},
    {'regularization': 0.3125, 'time_limit': TIME_LIMIT},
]
MAPTREE_PARAMS_LIST = [
    {'alpha': 0.95, 'beta': 0.5, 'rho': RHO, 'time_limit': TIME_LIMIT},
]

SEARCHERS_AND_PARAMS_LISTS = [
    ("MAPTree", MAPTREE_PARAMS_LIST),
    ("CART", CART_PARAMS_LIST),
    ("DL8.5", DL85_PARAMS_LIST),
    ("GOSDT", GOSDT_PARAMS_LIST),
]
