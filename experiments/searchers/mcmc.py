import time
import json
from typing import Tuple, Dict, Any
import numpy as np
import random
from experiments.searchers.binary_classification_tree import BinaryClassificationTree
from .tree_smc.src.bdtmcmc import sample_tree, precompute, parser_add_common_options, parser_add_mcmc_options, parser_add_smc_options


def run(
        X_train,
        y_train,
        alpha: float = 0.95,
        beta: float = 0.5,
        rho: Tuple[float, float] = [2.5, 2.5],
        num_iterations: int = 10,
        seed: int = 42,
    ) -> Dict[str, Any]:
    assert(((X_train == 0) | (X_train == 1)).all())
    assert(((y_train == 0) | (y_train == 1)).all())
    assert(rho[0] == rho[1])
    
    parser = parser_add_common_options()
    parser = parser_add_smc_options(parser)
    parser = parser_add_mcmc_options(parser)
    settings = parser.parse_args([
        '--alpha_split', str(alpha),
        '--beta_split', str(beta),
        '--alpha', str(rho[0] + rho[1]),
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
    param, cache, cache_tmp = precompute(data, settings)
    best_node_info_dump = None
    best_tree_leaves = None
    best_post = -np.inf
    p = sample_tree(data, settings, param, cache, cache_tmp)
    for _ in range(num_iterations):
        p.sample(data, settings, param, cache)
        post = p.compute_logprob()
        if post > best_post:
            best_node_info_dump = json.dumps(p.node_info)
            best_tree_leaves = list(p.leaf_nodes)
            best_post = post
    end = time.perf_counter()

    best_node_info = {int(k):v for k, v in json.loads(best_node_info_dump).items()}
    tree = parse((best_node_info, best_tree_leaves))
    tree.fit(X_train, y_train)

    return {
        'tree': tree,
        'time': end - start,
    }


def parse(tree: Tuple[dict, list], node_idx: int=0) -> BinaryClassificationTree:
    node_info, leaves = tree
    if node_idx in leaves or node_idx not in node_info:
        return BinaryClassificationTree()
    return BinaryClassificationTree(
        parse(tree, 2 * node_idx + 1),
        parse(tree, 2 * node_idx + 2),
        int(node_info[node_idx][0]))