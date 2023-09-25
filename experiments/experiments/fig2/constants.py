POSTERIOR = {
    'alpha': 0.95,
    'beta': 0.5,
    'rho': [2.5, 2.5],
}

FINAL_RUN_TIME_LIMIT = 180

RANDOM_SEARCHER_SEEDS = list(range(42, 52))
RANDOM_SEARCHERS = ["MCMC", "SMC"]

MCMC_PARAMS_LIST = [
    {"num_iterations": 10, **POSTERIOR},
    {"num_iterations": 30, **POSTERIOR},
    {"num_iterations": 100, **POSTERIOR},
    {"num_iterations": 300, **POSTERIOR},
    {"num_iterations": 1000, **POSTERIOR},
]
SMC_PARAMS_LIST = [
    {"num_particles": 10, **POSTERIOR},
    {"num_particles": 30, **POSTERIOR},
    {"num_particles": 100, **POSTERIOR},
    {"num_particles": 300, **POSTERIOR},
    {"num_particles": 1000, **POSTERIOR},
]
MAPTREE_PARAMS_LIST = [
    {"num_expansions": 10, **POSTERIOR},
    {"num_expansions": 30, **POSTERIOR},
    {"num_expansions": 100, **POSTERIOR},
    {"num_expansions": 300, **POSTERIOR},
    {"num_expansions": 1000, **POSTERIOR},
    {"num_expansions": 3000, **POSTERIOR},
    {"num_expansions": 10000, **POSTERIOR},
    {"num_expansions": 30000, **POSTERIOR},
    {"num_expansions": 100000, **POSTERIOR},
    {"time_limit": FINAL_RUN_TIME_LIMIT, **POSTERIOR},
]

SEARCHERS_AND_PARAMS_LISTS = [
    ("MAPTree", MAPTREE_PARAMS_LIST),
    ("MCMC", MCMC_PARAMS_LIST),
    ("SMC", SMC_PARAMS_LIST),
]
