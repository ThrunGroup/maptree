#include <iostream>
#include <array>

#include "doctest/doctest.h"
// #include "search/bnb_map_search.h"
#include "search/befs_map_search.h"
#include "data/binary_data_loader.h"

using namespace std;

TEST_CASE("search test on small dataset")
{

    // test data generating tree
    //           x_2
    //          /   \z     
    //       0 /     \ 1
    //  (0: 0, 1: 19)  x_3
    //               /   \z      
    //           0  /     \ 1
    //     (0: 0, 1: 10) (0: 11, 1: 0)


    // alpha_s = 0.95
    // beta_s = 0.5
    // alpha = (1, 1)

    // log prior = ln((0.95/(sqrt(1) * 4)) * (0.95/(sqrt(2) * 3)) * (1 - 0.95/sqrt(2)) * (1 - 0.95/sqrt(3))^2) = -5.638
    // log likelihood = ln [(Beta(1, 20) * Beta(1, 11) * Beta(12, 1)) / Beta(1, 1)^3] = -7.879
    // log posterior = log prior + log likelihood = -13.517

    BinaryDataLoader bdl("data/test_data_small.txt");
    double alpha = 0.95;
    double beta = 0.5;
    array<double, 2> rho = {1, 1};

    DataManager dm(bdl.getFeatures(), bdl.getLabels());
    TreeLikelihood likelihood(rho);
    BCARTTreePrior prior(alpha, beta);

    // BranchAndBoundMAPSearch bnbSearch(bdl.getFeatures(), bdl.getLabels(), alpha, beta, rho);
    BestFirstSearchMAPSearch befsSearch(dm, likelihood, prior);

    // Solution bnbResult = bnbSearch.search();
    Solution befsResult = befsSearch.search();

    // CHECK(bnbResult.upperBound == doctest::Approx(13.517));
    CHECK(befsResult.upperBound == doctest::Approx(13.517));
}

TEST_CASE("search test on medium dataset")
{
    BinaryDataLoader bdl("data/test_data_medium.txt");
    double alpha = 0.95;
    double beta = 0.5;
    array<double, 2> rho = {2.5, 2.5};

    DataManager dm(bdl.getFeatures(), bdl.getLabels());
    TreeLikelihood likelihood(rho);
    BCARTTreePrior prior(alpha, beta);

    BestFirstSearchMAPSearch befsSearch(dm, likelihood, prior);
    Solution befsResult = befsSearch.search();
    CHECK(befsResult.upperBound == doctest::Approx(66.006945));

}
