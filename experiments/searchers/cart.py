import time
from typing import Dict, Any
from sklearn.tree import DecisionTreeClassifier

from experiments.searchers.binary_classification_tree import BinaryClassificationTree


def run(
        X_train,
        y_train,
        max_depth: int = None,
        max_leaf_nodes: int = None,
    ) -> Dict[str, Any]:
    assert(((X_train == 0) | (X_train == 1)).all())
    assert(((y_train == 0) | (y_train == 1)).all())

    start = time.perf_counter()
    clf = DecisionTreeClassifier(max_depth=max_depth, max_leaf_nodes=max_leaf_nodes)
    clf.fit(X_train, y_train)
    end = time.perf_counter()

    tree = parse(clf.tree_)
    tree.fit(X_train, y_train)

    return {
        'tree': tree,
        'time': end - start,
        'timeout': False,
    }


def parse(tree, idx: int=0) -> BinaryClassificationTree:
    if tree.children_left[idx] == -1:
        return BinaryClassificationTree()
    return BinaryClassificationTree(
            parse(tree, tree.children_left[idx]),
            parse(tree, tree.children_right[idx]),
            tree.feature[idx])
