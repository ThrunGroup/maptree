import time
from typing import Dict, Any
from pydl85 import DL85Classifier

from experiments.searchers.binary_classification_tree import BinaryClassificationTree

def run(
        X_train,
        y_train,
        max_depth: int = 3,
        time_limit: int = 0,
    ) -> Dict[str, Any]:
    assert(((X_train == 0) | (X_train == 1)).all())
    assert(((y_train == 0) | (y_train == 1)).all())
    
    start = time.perf_counter()
    clf = DL85Classifier(max_depth=max_depth, time_limit=time_limit)
    clf.fit(X_train, y_train)
    end = time.perf_counter()

    tree = parse(clf)
    tree.fit(X_train, y_train)

    return {
        'tree': tree,
        'time': end - start,
        'timeout': clf.timeout_,
    }


def parse(clf: DL85Classifier) -> BinaryClassificationTree:
    def parse_node(node: dict) -> BinaryClassificationTree:
        if "value" in node:
            return BinaryClassificationTree()
        return BinaryClassificationTree(
                parse_node(node["right"]),
                parse_node(node["left"]),
                int(node["feat"]))
    return parse_node(clf.base_tree_)
