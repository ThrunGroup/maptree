import time
from typing import Tuple, Dict, Any
import numpy as np
import random
from .mcmc import parse
from .tree_smc.src.bdtsmc import init_smc, run_smc, parser_add_common_options, parser_add_smc_options


def run(
        X_train,
        y_train,
        alpha: float = 0.95,
        beta: float = 0.5,
        rho: Tuple[float, float] = [2.5, 2.5],
        num_particles: int = 10,
        seed: int = 42,
    ) -> Dict[str, Any]:
    assert(((X_train == 0) | (X_train == 1)).all())
    assert(((y_train == 0) | (y_train == 1)).all())
    assert(rho[0] == rho[1])

    parser = parser_add_common_options()
    parser = parser_add_smc_options(parser)
    settings = parser.parse_args([
        '--alpha_split', str(alpha),
        '--beta_split', str(beta),
        '--alpha', str(rho[0] + rho[1]),
        '--n_particles', str(num_particles),
        '--n_islands', '1',
        '--verbose', '0',
    ])[0]

    data = {
        'x_train': X_train,
        'y_train': y_train,
        'n_train': X_train.shape[0],
        'n_dim': X_train.shape[1],
        'n_class': 2,
    }

    np.random.seed(seed)
    random.seed(seed)

    start = time.perf_counter()
    (particles, param, log_weights, cache, cache_tmp) = init_smc(data, settings)
    (particles, ess_itr, log_weights_itr, log_pd, particle_stats_itr_d, particles_itr_d, log_pd_islands) = \
            run_smc(particles, data, settings, param, log_weights, cache)
    best_particle = None
    best_post = -np.inf
    for p in particles:
        post = p.compute_logprob()
        if post > best_post:
            best_particle = p
            best_post = post
    end = time.perf_counter()

    tree = parse((best_particle.node_info, best_particle.leaf_nodes))
    tree.fit(X_train, y_train)
    
    return {
        'tree': tree,
        'time': end - start,
    }
