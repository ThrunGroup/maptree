import numpy as np
from typing import List
import os

from experiments.searchers.binary_classification_tree import BinaryClassificationTree
from ..globals import SYNTH_NUM_TREES, SYNTH_TOTAL_SAMPLES_PER_TREE, \
    SYNTH_NUM_FEATURES, SEED_SYNTH_TREE_GENERATOR, SEED_SYNTH_DATA_GENERATOR, \
    SYNTH_TREE_NUM_INTERNAL_NODES, DIR_DATA_SYNTH


def generate_random_tree(
        num_internal_nodes: int,
        available_features: List[int],
        rng: np.random.Generator):

    if num_internal_nodes == 0:
        return BinaryClassificationTree()

    # select feature from features not used by ancestors to avoid creating degenerate tree
    feature = rng.choice(available_features)
    available_features = [f for f in available_features if f != feature]

    # use 1 internal node for root, send uniform random amount left and the rest right
    num_internal_nodes_left = rng.integers(num_internal_nodes)
    left_subtree = generate_random_tree(
        num_internal_nodes_left,
        available_features,
        rng,
    )
    right_subtree = generate_random_tree(
        num_internal_nodes - 1 - num_internal_nodes_left,
        available_features,
        rng,
    )

    return BinaryClassificationTree(left_subtree, right_subtree, feature)


def assign_random_labels(tree: BinaryClassificationTree, rng: np.random.Generator):
    assert not tree.is_leaf()
    leaves = tree.get_all_leaves()

    # assign alternating labels to leaf nodes
    label = rng.choice([False, True])
    for leaf in leaves:
        leaf.label_counts = [0, 1] if label else [1, 0]
        label = not label


def generate_synthetic_tree_data():
    tree_rng = np.random.default_rng(SEED_SYNTH_TREE_GENERATOR)
    data_rng = np.random.default_rng(SEED_SYNTH_DATA_GENERATOR)

    X = data_rng.integers(2, size=(2, SYNTH_NUM_TREES, SYNTH_TOTAL_SAMPLES_PER_TREE, SYNTH_NUM_FEATURES))
    for i in range(SYNTH_NUM_TREES):
        tree = generate_random_tree(
            SYNTH_TREE_NUM_INTERNAL_NODES,
            list(range(SYNTH_NUM_FEATURES)),
            tree_rng,
        )
        assign_random_labels(tree, tree_rng)

        print(f"Generated Tree: {tree}")

        X_train = X[0][i]
        X_test = X[1][i]
        y_train = tree.predict(X_train)
        y_test = tree.predict(X_test)

        if not os.path.exists(DIR_DATA_SYNTH):
            os.makedirs(DIR_DATA_SYNTH)

        train_data_path = os.path.join(DIR_DATA_SYNTH, f'tree{i}-train.txt')
        with open(train_data_path, 'w') as fp:
            A = np.column_stack((X_train, y_train))
            np.savetxt(fp, A, fmt='%d')

        test_data_path = os.path.join(DIR_DATA_SYNTH, f'tree{i}-test.txt')
        with open(test_data_path, 'w') as fp:
            A = np.column_stack((X_test, y_test))
            np.savetxt(fp, A, fmt='%d')
