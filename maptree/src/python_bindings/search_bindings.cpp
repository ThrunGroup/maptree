/**
 * @file search_bindings.cpp
 * @brief Set up bindings for search functions to Python with pybind11.
 *
 * This file contains the bindings for the search functions to Python with
 * pybind11. The bindings are exported as a Python module called maptree.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <vector>
#include <array>
#include <chrono>

#include "search/befs_map_search.h"
#include "solution/solution.h"
#include "data/data_manager.h"
#include "posterior/tree_prior.h"
#include "posterior/tree_likelihood.h"

namespace py = pybind11;

/**
 * @brief MAP Tree search function
 * @param features (num samples) x (num features) 2D boolean vector of features.
 * @param labels (num samples) 1D boolean vector of labels.
 * @param alpha The alpha parameter of the constructive BCART prior.
 * @param beta The beta parameter of the constructive BCART prior.
 * @param rho A 2-item array indexing BCART's Beta distribution prior for the
 * Bernoulli distributions in each of the leaf nodes.
 * @param numExpansions The maximum number of expansions to perform.
 * @param timeLimit The time limit in seconds. If -1, no time limit.
 * @param degen Whether or not the BCART prior should support degenerate trees.
 * Note that it is still guaranteed that a degenerate tree will not be returned.
 * @returns A Solution object containing the unnormalized log posterior upper/
 * lower bound and a string representation of the output tree.
 *
 * The search function uses the best-first search algorithm to find the MAP
 * tree.
 *
 * @see BestFirstSearchMAPSearch
*/
Solution searchBeFS(
    std::vector<std::vector<bool>> features,
    std::vector<bool> labels,
    double alpha,
    double beta,
    std::array<double, 2> rho,
    int numExpansions,
    int timeLimit,
    bool degen
)
{
    DataManager dm(features, labels);

    TreePrior *prior;
    prior = degen
        ? static_cast<TreePrior *>(new BCARTDegenTreePrior(alpha, beta))
        : static_cast<TreePrior *>(new BCARTTreePrior(alpha, beta));

    TreeLikelihood likelihood(rho);
    BestFirstSearchMAPSearch searchObj(dm, likelihood, *prior, numExpansions, timeLimit);

    Solution result = searchObj.search();
    delete prior;

    return result;
}

//! Here, we define the maptree Python module, binding the search function.
PYBIND11_MODULE(maptree, m) {
    m.doc() = "MAP tree search binding";

    m.def(
        "search",
        &searchBeFS,
        "Best first search",
        py::arg("features"),
        py::arg("labels"),
        py::arg("alpha"),
        py::arg("beta"),
        py::arg("rho"),
        py::arg("numExpansions")=BestFirstSearchMAPSearch::INF_EXPANSIONS,
        py::arg("timeLimit")=BestFirstSearchMAPSearch::INF_TIME_LIMIT,
        py::arg("degen")=false
    );

    py::class_<Solution>(m, "Solution") \
            .def_readwrite("lb", &Solution::lowerBound) \
            .def_readwrite("ub", &Solution::upperBound) \
            .def_readwrite("tree", &Solution::treeRepresentation);
}