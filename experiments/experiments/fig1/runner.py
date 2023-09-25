import pandas as pd

from experiments.globals import get_stratified_k_folds_cp4im_dataset, run_search, save_results
from .constants import SEARCHERS_AND_PARAMS_LISTS, RHO


def run(dataset: str):
    print(f"Performance comparison on CP4IM dataset: {dataset}")
    print("=====================================================")

    results = []
    for i, fold in enumerate(get_stratified_k_folds_cp4im_dataset(dataset)):
        print(f"Fold: {i}")
        X_train, y_train, X_test, y_test = fold
        for searcher, params_list in SEARCHERS_AND_PARAMS_LISTS:
            print(f"Searcher: {searcher}")
            for j, params in enumerate(params_list):
                print(f"Params: {params}")

                result = run_search(searcher, X_train, y_train, **params)
                if result is None:
                    print("Run Failed!!!")
                    continue

                tree = result['tree']
                time = result['time']
                timeout = result['timeout']

                tree.fit(X_train, y_train)

                # add results to results queue
                train_acc = (tree.predict(X_train) == y_train).sum() / len(y_train)
                test_acc = (tree.predict(X_test) == y_test).sum() / len(y_test)
                train_sll = tree.log_likelihood(X_train, y_train, rho=RHO) / len(y_train)
                test_sll = tree.log_likelihood(X_test, y_test, rho=RHO) / len(y_test)
                size = tree.size()

                print(f"Timed Out: {timeout}")
                print(f"Test Accuracy: {test_acc}")
                print(f"Test SLL: {test_sll}")

                results.append({
                    'searcher': searcher,
                    'params_id': j,
                    'fold': i,
                    'tree': str(tree),
                    'time': time,
                    'train_acc': train_acc,
                    'test_acc': test_acc,
                    'train_sll': train_sll,
                    'test_sll': test_sll,
                    'size': size,
                    'timeout': timeout,
                })

    save_results(pd.DataFrame(results), "fig1", dataset)
