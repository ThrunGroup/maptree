import pandas as pd
from math import isclose

from experiments.globals import get_full_cp4im_dataset, run_search, save_results
from .constants import SEARCHERS_AND_PARAMS_LISTS, POSTERIOR, RANDOM_SEARCHERS, RANDOM_SEARCHER_SEEDS


def run(dataset: str):
    print(f"Speed comparison on CP4IM dataset: {dataset}")
    print("=====================================================")

    data = get_full_cp4im_dataset(dataset)
    X, y = data

    results = []

    for searcher, params_list in SEARCHERS_AND_PARAMS_LISTS:
        print(f"Searcher: {searcher}")
        for j, params in enumerate(params_list):
            print(f"Params: {params}")
            for k, seed in enumerate(RANDOM_SEARCHER_SEEDS):
                if searcher not in RANDOM_SEARCHERS and k > 0:
                    continue

                if searcher in RANDOM_SEARCHERS:
                    print(f"Seed: {seed}")
                    params['seed'] = seed

                result = run_search(searcher, X, y, **params)
                if result is None:
                    print("Run Failed!!!")
                    continue

                tree = result['tree']
                time = result['time']
                tree.fit(X, y)
                size = tree.size()

                post = tree.log_posterior(X, y, **POSTERIOR)
                best_post = 0.0

                if "lower_bound" in result:
                    best_post = -result["lower_bound"]
                if "upper_bound" in result:
                    assert(isclose(post, -result["upper_bound"]))

                print(f"Time: {time}")
                print(f"Log Posterior: {post}")

                results.append({
                    'searcher': searcher,
                    'params_id': j,
                    'tree': str(tree),
                    'time': time,
                    'post': post,
                    'best_post': best_post,
                    'size': size,
                    'seed': k,
                })

    save_results(pd.DataFrame(results), "fig2", dataset)

        