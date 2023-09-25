/**
 * @file decision_tree.h
 * 
 * @brief Defines the DecisionTree class.
 * 
 * This file contains the definition of the DecisionTree class, which is used to
 * represent a decision tree. The decision tree is represented as a binary tree
 * where each internal node represent a binary split on a feature.
*/

#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <limits>
#include <string>
#include <cassert>
#include <vector>
#include <array>

/**
 * @class DecisionTree
 * @brief Represents a decision tree.
 * 
 * This class contains the definition of the DecisionTree class, consisting of
 * the feature that the root node splits on and pointers to the left and right
 * subtrees. If the tree is a leaf, then the feature is set to NO_FEATURE and
 * the left and right subtrees are null.
 */
class DecisionTree {
    public:
        static constexpr size_t NO_FEATURE = std::numeric_limits<size_t>::max();

        size_t feature;
        DecisionTree *left, *right;

        DecisionTree(
            size_t feature = NO_FEATURE,
            DecisionTree *left = nullptr,
            DecisionTree *right = nullptr
        )
        : feature(feature)
        , left(left)
        , right(right)
        {
            assert((left == nullptr) == (right == nullptr));
            assert((feature == NO_FEATURE) == isLeaf());
        };

        ~DecisionTree() {
            if (left != nullptr) delete left;
            if (right != nullptr) delete right;
        };

        /**
         * @brief Checks if the tree is a leaf
         */
        bool isLeaf() const;

        /**
         * @brief Returns a string representation of the tree.
         * @returns A string representation of the tree.
         * 
         * Leaf nodes are represented as an empty string, and internal nodes are
         * represented as a string of the form:
         * 
         *      "(<left subtree><feature><right subtree>)".
         * 
         * For example, see the following tree:
         * 
         *         1
         *       /   \
         *      5     9   ------>  "((5)1(9))"
         *     / \   / \
         *    .   . .   .
         * 
         */
        std::string toString() const;

        /**
         * @brief Returns a string representation of the tree.
         * @see DecisionTree::toString
         */
        friend std::ostream& operator<<(std::ostream& os, const DecisionTree& tree) {
            os << tree.toString();
            return os;
        };
};

#endif