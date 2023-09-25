RHO = [2.5, 2.5]
TIME_LIMIT = 60

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
    {'alpha': 0.999, 'beta': 0.1, 'rho': RHO, 'time_limit': TIME_LIMIT},
    {'alpha': 0.99, 'beta': 0.2, 'rho': RHO, 'time_limit': TIME_LIMIT},
    {'alpha': 0.95, 'beta': 0.5, 'rho': RHO, 'time_limit': TIME_LIMIT},
    {'alpha': 0.9, 'beta': 1.0, 'rho': RHO, 'time_limit': TIME_LIMIT},
    {'alpha': 0.8, 'beta': 2.0, 'rho': RHO, 'time_limit': TIME_LIMIT},
    {'alpha': 0.5, 'beta': 4.0, 'rho': RHO, 'time_limit': TIME_LIMIT},
    {'alpha': 0.2, 'beta': 8.0, 'rho': RHO, 'time_limit': TIME_LIMIT},
]

SEARCHERS_AND_PARAMS_LISTS = [
    ("MAPTree", MAPTREE_PARAMS_LIST),
    ("CART", CART_PARAMS_LIST),
    ("DL8.5", DL85_PARAMS_LIST),
    ("GOSDT", GOSDT_PARAMS_LIST),
]
