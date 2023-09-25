#include <iostream>
#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <array>
#include <chrono>

#include "data/binary_data_loader.h"
#include "search/befs_map_search.h"

int main(int argc, char** argv) {
  std::string file;
  double alpha = 0.8;
  double beta = 1.0;
  std::array<double, 2> rho = {1.0, 1.0};

  struct option longopts[] = {
    { "file", required_argument, NULL, 'f' },
    { "alpha", optional_argument, NULL, 'a' },
    { "beta", optional_argument, NULL, 'b' },
    { "rho", optional_argument, NULL, 'r' },
    { NULL, 0, NULL, 0 }
  };

  while (true) {
    int opt = getopt_long(argc, argv, "f:a:b:r:", longopts, 0);
    if (opt == -1) break;
    switch (opt) {
      case 'f': {
        file = std::string(optarg);
        break;
      }
      case 'a': {
        alpha = std::atof(optarg);
        break;
      }
      case 'b': {
        beta = std::atof(optarg);
        break;
      }
      case 'r': {
        double total = std::atof(optarg);
        rho[0] = total / 2.0;
        rho[1] = total / 2.0;
        break;
      }
      case '?': {
        std::cout << "Usage: " << argv[0] << " -f <file> [-a <alpha>] [-b <beta>] [-r <rho>]" << std::endl;
        return EXIT_FAILURE;
        break;
      }
    }
  }

  BinaryDataLoader bdl(file);
  std::vector<std::vector<bool>> features = bdl.getFeatures();
  std::vector<bool> labels = bdl.getLabels();

  auto start = std::chrono::high_resolution_clock::now();
  BestFirstSearchMAPSearch search(features, labels, alpha, beta, rho);
  Solution result = search.search();
  auto stop = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  std::cout << "Tree: " << result.treeRepresentation << std::endl;
  std::cout << "Lower Bound: " << result.lowerBound << std::endl;
  std::cout << "Upper Bound: " << result.upperBound << std::endl;
  std::cout << "Training Time (ms): " << duration.count() << std::endl;

  return EXIT_SUCCESS;
}