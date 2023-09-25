/**
 * @file befs_map_search.h
 * @brief Best-first search for MAP tree.
 * 
 * This file contains the BestFirstSearchMAPSearch class, which implements a
 * best-first search for the MAP tree. The algorithm is adapted from AO* and
 * utilizes an admissible heuristic based on the "perfect split" lower bound.
 */

#ifndef BEFS_MAP_SEARCH_H
#define BEFS_MAP_SEARCH_H

#include <forward_list>
#include <vector>
#include <array>

#include "constants.h"
#include "subproblem.h"
#include "cache/approx_bitset_cache.h"
#include "search/base_map_search.h"
#include "solution/decision_tree.h"

//! Forward declaration of AndNode for use in OrNode.
struct AndNode;

/**
 * @struct OrNode
 * @brief Represents an OR node in the explicit AND/OR search graph or a 
 * subproblem in the optimal decision tree search problem.
 * 
 * This struct represents an OR node in the explicit AND/OR search graph or a
 * subproblem in the optimal decision tree search problem. The OR node contains
 * the lower and upper bounds on the unnormalized log posterior for the
 * subproblem, as well as the children of the OR node. The children of the OR
 * node are AND nodes which represents valid splits of the subproblem. The OR
 * node also contains a pointer to the AND nodes with the best upper/lower
 * bounds. These are used to efficiently identify the next tree to expand and
 * the best tree found so far. The OR node also contains a list of pointers to
 * its parents, which is used to efficiently backpropagate upper/lower bounds
 * through the explicit search graph.
*/
struct OrNode {
    size_t depth;
    double lowerBound;
    double upperBound;
    bool expanded;
    AndNode *childWithBestLB;
    AndNode *childWithBestUB;
    std::vector<AndNode *> children;
    std::forward_list<AndNode *> parents;
    bool isSolved() {
        return lowerBound == upperBound;
    };
};

/**
 * @struct AndNode
 * @brief Represents an AND node in the explicit AND/OR search graph or a
 * split of its parent subproblem in the optimal decision tree search problem.
 * 
 * This struct represents an AND node in the explicit AND/OR search graph or a
 * split of its parent subproblem in the optimal decision tree search problem.
 * The AND node contains the feature it splits on, as well as the two resulting
 * OR Nodes representing child subproblems (left is feature = 0, right is
 * feature = 1). The AND node also contains a pointer to its parent OR node,
 * which is used to efficiently backpropagate upper/lower bounds through the
 * explicit search graph.
 */
struct AndNode {
    size_t feature;
    OrNode *leftChild;
    OrNode *rightChild;
    OrNode *parent;
    bool isSolved() {
        return leftChild != nullptr && leftChild->isSolved() && rightChild != nullptr && rightChild->isSolved();
    };
};

/**
 * @class BestFirstSearchMAPSearch
 * @brief Best-first search for MAP tree.
 * @implements BaseMAPSearch
 * 
 * This class implements a best-first search for the MAP tree. The algorithm is
 * adapted from AO* and utilizes an admissible heuristic based on the "perfect
 * split" lower bound. It can be summarized loosely as follows:
 * 
 * 1. Initialize the explicit graph with the full subproblem as the root OR
 *    Node.
 * 2. While the root OR Node is not solved and the expansion limit and time
 *    limit have not been reached:
 *      2a. Find an unexpanded leaf of the tree in the explicity graph with the
 *          lowest lower bound.
 *      2b. Expand this leaf, adding its children to the explicit graph.
 *      2c. Update the bounds of this leaf and its descendants.
 * 3. Return the best tree found so far.
 */
class BestFirstSearchMAPSearch : BaseMAPSearch {
    public:
        static constexpr int INF_EXPANSIONS = -1;
        static constexpr int INF_TIME_LIMIT = -1;

        BestFirstSearchMAPSearch(
            const DataManager& dm,
            const TreeLikelihood& likelihood,
            const TreePrior& prior,
            int numExpansions = INF_EXPANSIONS,
            int timeLimit = INF_TIME_LIMIT
        )
        : BaseMAPSearch(dm, likelihood, prior)
        , cache_(NUM_BLOCKS(dm_.getNumSamples()))
        , expansionLimit_(numExpansions)
        , timeLimit_(timeLimit)
        , subproblem_(dm_)
        , rootNode_(buildNode(subproblem_.getLabelCounts(), 0))
        {};
        ~BestFirstSearchMAPSearch() override { 
            for (OrNode *orNode : orNodes_) delete orNode;
            for (AndNode *andNode : andNodes_) delete andNode;
        };
        Solution search() override;

    private:
        ApproxBitsetCache cache_;
        std::forward_list<OrNode *> orNodes_ = std::forward_list<OrNode *>();
        std::forward_list<AndNode *> andNodes_ = std::forward_list<AndNode *>();
        int expansionLimit_;
        int timeLimit_;
        Subproblem subproblem_;
        OrNode *rootNode_;

        OrNode *buildNode(const std::array<int, 2>& labelCounts, size_t depth);
        OrNode *findExpandableLeaf();
        void expand(OrNode *node);
        bool updateLowerBound(OrNode *node);
        void backpropagateLowerBound(OrNode *source);
        void backpropagateUpperBound(OrNode *source);
        DecisionTree *buildDecisionTree(OrNode *node);
};

#endif
