#include "solution/decision_tree.h"

constexpr size_t DecisionTree::NO_FEATURE;

bool DecisionTree::isLeaf() const {
    return left == nullptr && right == nullptr;
}

std::string DecisionTree::toString() const {
    return isLeaf() ? "" : ("(" + left->toString() + std::to_string(feature) + right->toString() + ")");
}




