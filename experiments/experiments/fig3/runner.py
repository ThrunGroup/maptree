import pandas as pd

from experiments.globals import get_synth_data_samples, run_search, save_results
from .constants import SEARCHERS_AND_PARAMS_LISTS, NOISE_VALUES, SAMPLE_SIZE_VALUES, RHO


def run(tree_id: int):
    print(f"Performance comparison on synthetic tree-generated data for tree {tree_id}")
    print("=====================================================")

    results = []
    for i, sample_size in enumerate(SAMPLE_SIZE_VALUES):
        print(f"Sample Size: {sample_size}")
        for j, noise in enumerate(NOISE_VALUES):
            print(f"Noise: {noise}")
            data = get_synth_data_samples(tree_id, sample_size, noise)
            X_train, y_train, X_test, y_test = data
            for searcher, params_list in SEARCHERS_AND_PARAMS_LISTS:
                print(f"Searcher: {searcher}")
                for k, params in enumerate(params_list):
                    print(f"Params: {params}")

                    result = run_search(searcher, X_train, y_train, **params)
                    if result is None:
                        print("Run Failed!!!")
                        continue

                    tree = result['tree']
                    time = result['time']

                    tree.fit(X_train, y_train)

                    # add results to results queue
                    train_acc = (tree.predict(X_train) == y_train).sum() / len(y_train)
                    test_acc = (tree.predict(X_test) == y_test).sum() / len(y_test)
                    train_sll = tree.log_likelihood(X_train, y_train, rho=RHO) / len(y_train)
                    test_sll = tree.log_likelihood(X_test, y_test, rho=RHO) / len(y_test)
                    size = tree.size()

                    print(f"Test Acc: {test_acc}")

                    results.append({
                        'sample_size_id': i,
                        'noise_id': j,
                        'tree_data_id': tree_id,
                        'searcher': searcher,
                        'params_id': k,
                        'tree': str(tree),
                        'time': time,
                        'train_acc': train_acc,
                        'test_acc': test_acc,
                        'train_sll': train_sll,
                        'test_sll': test_sll,
                        'size': size,
                    })

    save_results(pd.DataFrame(results), "fig3", f"tree{tree_id}")
