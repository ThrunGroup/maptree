#include <array>
#include "doctest/doctest.h"

#include "posterior/tree_likelihood.h"
#include "posterior/tree_prior.h"

TEST_CASE("prior/likelihood test")
{
    std::array<int, 2> labelCounts = {3, 5};
    std::array<double, 2> rho = {1.0, 1.0};

    BCARTTreePrior prior = BCARTTreePrior(.95, .5);
    TreeLikelihood likelihood = TreeLikelihood(rho);

    CHECK(likelihood.logBeta(1, 1) == doctest::Approx(0));
    CHECK(likelihood.logBeta(3, 5) == doctest::Approx(-4.65396));
    CHECK(likelihood.logLikelihood(labelCounts) == doctest::Approx(-6.222576));
    CHECK(prior.logSplitProb(5, 1, 1) == doctest::Approx(-0.94717));
    CHECK(prior.logStopProb(5, 1, 1) == doctest::Approx(-0.490755));
}
