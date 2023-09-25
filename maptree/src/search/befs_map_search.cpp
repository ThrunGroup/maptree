#include <cassert>
#include <chrono>
#include <forward_list>
#include <queue>
#include <set>

#include "search/befs_map_search.h"

constexpr int BestFirstSearchMAPSearch::INF_EXPANSIONS;
constexpr int BestFirstSearchMAPSearch::INF_TIME_LIMIT;

Solution BestFirstSearchMAPSearch::search() {
    bool hasExpansionLimit = expansionLimit_ != BestFirstSearchMAPSearch::INF_EXPANSIONS;
    bool hasTimeLimit = timeLimit_ != BestFirstSearchMAPSearch::INF_TIME_LIMIT;

    size_t expansionsRemaining = static_cast<size_t>(expansionLimit_);
    long long timeLimit = static_cast<long long>(timeLimit_);
    std::chrono::time_point<std::chrono::steady_clock> startTime = std::chrono::steady_clock::now();
    long long secondsElapsed;

    while (!rootNode_->isSolved()) {
        subproblem_.reset();
        OrNode *leaf = findExpandableLeaf();
        expand(leaf);
        backpropagateLowerBound(leaf);
        backpropagateUpperBound(leaf);

        expansionsRemaining--;
        secondsElapsed = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - startTime).count();
        if (hasExpansionLimit && expansionsRemaining == 0) break;
        if (hasTimeLimit && secondsElapsed >= timeLimit) break;
    }

    DecisionTree *dt = buildDecisionTree(rootNode_);
    std::string treeRepresentation = dt->toString();
    delete dt;

    return {
        rootNode_->lowerBound,
        rootNode_->upperBound,
        treeRepresentation
    };
}

OrNode *BestFirstSearchMAPSearch::buildNode(const std::array<int, 2>& labelCounts, size_t depth) {
    OrNode *node = new OrNode();
    orNodes_.push_front(node);

    node->depth = depth;
    node->children = std::vector<AndNode *>(0);
    node->parents = std::forward_list<AndNode *>(0);
    node->childWithBestLB = node->childWithBestUB = nullptr;
    node->upperBound = getUpperBound(labelCounts, depth);
    node->lowerBound = getLowerBound(labelCounts, depth);
    assert(node->lowerBound > 0);
    node->expanded = false;

    return node;
}

OrNode *BestFirstSearchMAPSearch::findExpandableLeaf() {
    assert(subproblem_.getDepth() == 0);
    assert(!rootNode_->isSolved());

    OrNode *node = rootNode_;
    AndNode *markedChild;
    double leftSpread, rightSpread;
    bool value;
    while (node->expanded) {
        markedChild = node->childWithBestLB;
        leftSpread = markedChild->leftChild->upperBound - markedChild->leftChild->lowerBound;
        rightSpread = markedChild->rightChild->upperBound - markedChild->rightChild->lowerBound;
        value = leftSpread < rightSpread;
        node = value ? markedChild->rightChild : markedChild->leftChild;
        subproblem_.applySplit(markedChild->feature, value);
    }

    // we should not end up at a solved node
    assert(!node->isSolved());

    return node;
}

void BestFirstSearchMAPSearch::expand(OrNode *node) {
    assert(!node->expanded);

    node->expanded = true;

    const std::vector<size_t>& validSplits = subproblem_.getValidSplits();
    if (validSplits.empty()) {
        node->upperBound = node->lowerBound = getUpperBound(subproblem_.getLabelCounts(), node->depth, 0);
        return;
    } else {
        node->children.resize(validSplits.size());
    }

    double splitPenalty = -prior_.logSplitProb(node->depth, validSplits.size(), dm_.getNumFeatures());
    std::array<int, 2> outerLabelCounts = subproblem_.getLabelCounts();

    double splitValue;
    AndNode *child;
    OrNode *subChild;
    std::array<int, 2> subChildLabelCounts;
    size_t childIdx = 0;
    for (size_t feature : validSplits) {
        child = new AndNode();
        andNodes_.push_front(child);
        child->feature = feature;
        child->parent = node;
        for (bool value : {true, false}) {
            subproblem_.applySplit(feature, value);

            if (value) {
                subChildLabelCounts = subproblem_.getLabelCounts();
            } else {
                subChildLabelCounts[0] = outerLabelCounts[0] - subChildLabelCounts[0];
                subChildLabelCounts[1] = outerLabelCounts[1] - subChildLabelCounts[1];
            }

            subChild = static_cast<OrNode *>(cache_.get(subproblem_));
            if (subChild == nullptr) {
                subChild = buildNode(subChildLabelCounts, node->depth + 1);
                cache_.put(subproblem_, subChild);
            }

            subChild->parents.push_front(child);
            if (value) {
                child->rightChild = subChild;
            } else {
                child->leftChild = subChild;
            }

            subproblem_.revertSplit();
        }

        splitValue = child->leftChild->upperBound + child->rightChild->upperBound + splitPenalty;
        if (splitValue < node->upperBound) {
            node->upperBound = splitValue;
            node->childWithBestUB = child;
        }

        node->children[childIdx++] = child;
    }
}

bool BestFirstSearchMAPSearch::updateLowerBound(OrNode *node) {
    assert(node->expanded);

    double bestLowerBound = node->upperBound;
    node->childWithBestLB = nullptr;
    double splitPenalty = -prior_.logSplitProb(node->depth, node->children.size(), dm_.getNumFeatures());

    double splitValueLowerBound;
    for (AndNode *child : node->children) {
        splitValueLowerBound = child->leftChild->lowerBound + child->rightChild->lowerBound + splitPenalty;
        if (splitValueLowerBound < bestLowerBound) {
            bestLowerBound = splitValueLowerBound;
            node->childWithBestLB = child;
        }
    }

    //! check perfect split heuristic addmissibility
    assert(bestLowerBound >= node->lowerBound);

    bool improvedLowerBound = bestLowerBound > node->lowerBound;
    node->lowerBound = bestLowerBound;

    return improvedLowerBound;
}

void BestFirstSearchMAPSearch::backpropagateLowerBound(OrNode *source) {
    std::set<OrNode *> visited;
    std::queue<OrNode *> toVisit;
    toVisit.push(source);
    visited.insert(source);
    OrNode *front;
    while (!toVisit.empty()) {
        front = toVisit.front();
        toVisit.pop();
        if (!updateLowerBound(front)) continue;
        for (AndNode *parent : front->parents) {
            if (visited.find(parent->parent) == visited.end()
                && !parent->parent->isSolved()
                && parent->parent->childWithBestLB == parent
                ) {
                toVisit.push(parent->parent);
                visited.insert(parent->parent);
            }
        }
    }
}

void BestFirstSearchMAPSearch::backpropagateUpperBound(OrNode *source) {
    std::set<OrNode *> visited;
    std::queue<OrNode *> toVisit;
    toVisit.push(source);
    visited.insert(source);
    OrNode *front;
    double splitPenalty;
    double splitValue;
    while (!toVisit.empty()) {
        front = toVisit.front();
        toVisit.pop();
        for (AndNode *parent : front->parents) {
            splitPenalty = -prior_.logSplitProb(parent->parent->depth, parent->parent->children.size(), dm_.getNumFeatures());
            splitValue = parent->leftChild->upperBound + parent->rightChild->upperBound + splitPenalty;
            if (splitValue < parent->parent->upperBound) {
                parent->parent->upperBound = splitValue;
                parent->parent->childWithBestUB = parent;
                if (visited.find(parent->parent) == visited.end()) {
                    toVisit.push(parent->parent);
                    visited.insert(parent->parent);
                }
            }
        }
    }
}

DecisionTree *BestFirstSearchMAPSearch::buildDecisionTree(OrNode *node) {
    // no possible splits — return leaf
    if (node->children.empty() || !node->expanded || node->childWithBestUB == nullptr) {
        return new DecisionTree();
    }

    AndNode *markedChild = node->childWithBestUB;
    return new DecisionTree(
        markedChild->feature,
        buildDecisionTree(markedChild->leftChild),
        buildDecisionTree(markedChild->rightChild)
    );
}
