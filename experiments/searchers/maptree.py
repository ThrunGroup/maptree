import time
from typing import Tuple, Dict, Any
from maptree import search as maptree_search
from experiments.searchers.binary_classification_tree import BinaryClassificationTree


def run(
        X_train,
        y_train,
        alpha: float = 0.95,
        beta: float = 0.5,
        rho: Tuple[float, float] = (2.5, 2.5),
        num_expansions: int = -1,
        time_limit: int = -1,
    ) -> Dict[str, Any]:
    assert(((X_train == 0) | (X_train == 1)).all())
    assert(((y_train == 0) | (y_train == 1)).all())

    start = time.perf_counter()
    sol = maptree_search(X_train, y_train, alpha, beta, rho, num_expansions, time_limit)
    end = time.perf_counter()

    tree = parse(sol.tree)
    tree.fit(X_train, y_train)

    return {
        'tree': tree,
        'time': end - start,
        'timeout': sol.lb < sol.ub,
        'lower_bound': sol.lb,
        'upper_bound': sol.ub
    }


def parse(tree: str) -> BinaryClassificationTree:
    return BinaryClassificationTree.parse(tree)
