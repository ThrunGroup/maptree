import numpy as np
from typing import Tuple
from math import lgamma


def get_num_valid_splits(X):
    num_valid = 0
    for f in range(X.shape[1]):
        if 0 < np.count_nonzero(X[:, f]) < X.shape[0]:
            num_valid += 1
    return num_valid


def log_prob_split(depth: int, alpha: float, beta: float) -> float:
    return np.log(alpha) - beta * np.log(depth + 1)


def log_prob_stop(depth: int, alpha: float, beta: float) -> float:
    return np.log(1 - np.exp(log_prob_split(depth, alpha, beta)))


def log_beta(count: Tuple[float, float]) -> float:
    return lgamma(count[0]) + lgamma(count[1]) - lgamma(count[0] + count[1])


def log_likelihood(y, rho: Tuple[float, float]) -> float:
    count = np.bincount(y, minlength=2)
    return log_beta((count[0] + rho[0], count[1] + rho[1])) - log_beta(rho)


def split(X, feature: int) -> Tuple[np.ndarray, np.ndarray]:
    left = np.nonzero(X[:, feature] == False)
    right = np.nonzero(X[:, feature] == True)
    return left, right


class BinaryClassificationTree:
    def __init__(self,
                 left: 'BinaryClassificationTree' = None,
                 right: 'BinaryClassificationTree' = None,
                 feature: int = None):
        assert ((left is None) == (right is None) == (feature is None))
        self.left = left
        self.right = right
        self.feature = feature
        self.label_counts = None

    def __str__(self):
        if self.is_leaf():
            return ""
        return f"({self.left}{self.feature}{self.right})"

    @classmethod
    def parse(cls, tree: str) -> 'BinaryClassificationTree':
        if type(tree) == float:
            tree = str(tree)
        if tree in ['', 'nan']:
            return BinaryClassificationTree()

        def parse_feature(tree: str, i: int) -> Tuple[int, int]:
            j = i + 1
            while tree[j] not in ['(', ')']:
                j += 1
            return int(tree[i:j]), j

        def parse_node(tree: str, i: int = 0) -> Tuple[
                BinaryClassificationTree, int]:
            if tree == '':
                return BinaryClassificationTree(), i
            if tree[i] == '(':
                left, i = parse_node(tree, i + 1)
                feature, i = parse_feature(tree, i)
                right, i = parse_node(tree, i)
                return BinaryClassificationTree(left, right, feature), i + 1
            else:
                return BinaryClassificationTree(), i

        return parse_node(tree)[0]

    def is_leaf(self) -> bool:
        return self.feature is None

    def size(self) -> int:
        if self.is_leaf():
            return 1
        return 1 + self.left.size() + self.right.size()

    def depth(self) -> int:
        if self.is_leaf():
            return 0
        return max(self.left.depth(), self.right.depth())

    def fit(self, X, y):
        self.label_counts = np.bincount(y, minlength=2)
        if self.is_leaf():
            return
        left, right = split(X, self.feature)
        self.left.fit(X[left], y[left])
        self.right.fit(X[right], y[right])

    def predict(self, X):
        if self.is_leaf():
            assert (self.label_counts is not None)
            return np.argmax(self.label_counts)
        left, right = split(X, self.feature)
        y = np.zeros(X.shape[0], dtype=bool)
        y[left] = self.left.predict(X[left])
        y[right] = self.right.predict(X[right])
        return y

    def get_all_leaves(self):
        if self.is_leaf():
            return [self]
        return self.left.get_all_leaves() + self.right.get_all_leaves()

    def log_prior(self, X, alpha, beta, depth=0):
        if X.shape[0] == 0:
            return -np.inf
        num_valid_splits = get_num_valid_splits(X)
        if self.is_leaf():
            return log_prob_stop(depth, alpha,
                                 beta) if num_valid_splits else 0.0
        left, right = split(X, self.feature)
        return log_prob_split(depth, alpha, beta) - np.log(num_valid_splits) + \
            self.left.log_prior(X[left], alpha, beta, depth + 1) + \
            self.right.log_prior(X[right], alpha, beta, depth + 1)

    def log_likelihood(self, X, y, rho):
        if self.is_leaf():
            return log_likelihood(y, rho)
        left, right = split(X, self.feature)
        return self.left.log_likelihood(X[left], y[left], rho) + \
            self.right.log_likelihood(X[right], y[right], rho)

    def log_posterior(self, X, y, alpha, beta, rho):
        return self.log_prior(X, alpha, beta) + self.log_likelihood(X, y, rho)
