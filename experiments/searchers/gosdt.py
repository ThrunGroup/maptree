import time
from typing import Dict, Any
import pandas as pd
from gosdt import GOSDT

from experiments.searchers.binary_classification_tree import BinaryClassificationTree


def run(
        X_train,
        y_train,
        max_depth: int = 0,
        regularization: float = 0.01,
        time_limit: int = 0,
    ) -> Dict[str, Any]:
    assert(((X_train == 0) | (X_train == 1)).all())
    assert(((y_train == 0) | (y_train == 1)).all())
    
    df_X_train = pd.DataFrame(X_train)
    df_y_train = pd.DataFrame(y_train)
    config = {
        'depth_budget': max_depth,
        'regularization': regularization,
        'time_limit': time_limit,
        'allow_small_reg': True,
    }

    start = time.perf_counter()
    clf = GOSDT(config)
    clf.fit(df_X_train, df_y_train)
    end = time.perf_counter()

    tree = parse(clf)
    tree.fit(X_train, y_train)

    return {
        'tree': tree,
        'time': end - start,
        'timeout': clf.timeout
    }


def parse(clf: GOSDT) -> BinaryClassificationTree:
    def parse_node(node: dict) -> BinaryClassificationTree:
        if "prediction" in node:
            return BinaryClassificationTree()
        assert(node["relation"] == "==")
        assert(node["reference"] == 1.0)
        return BinaryClassificationTree(
                parse_node(node["false"]),
                parse_node(node["true"]),
                int(node["feature"]))

    root = clf.tree.source
    return parse_node(root)