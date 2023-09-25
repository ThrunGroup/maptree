import os
import pandas as pd
import numpy as np
from multiprocessing import Process, Queue, TimeoutError
from queue import Empty
from datetime import datetime
from glob import glob
from sklearn.model_selection import StratifiedKFold

from experiments.searchers.maptree import run as maptree_search
from experiments.searchers.mcmc import run as mcmc_search
from experiments.searchers.smc import run as smc_search
from experiments.searchers.cart import run as cart_search
from experiments.searchers.dl85 import run as dl85_search
from experiments.searchers.gosdt import run as gosdt_search

DIR_DATA_CP4IM = os.path.join("data", "cp4im")
DIR_DATA_SYNTH = os.path.join("data", "synth")

CP4IM_DATASET_URL = 'https://dtai.cs.kuleuven.be/CP4IM/datasets/data/{dataset}.txt'
CP4IM_DATASET_NAMES = sorted([
    'zoo-1',
    'vote',
    'tic-tac-toe',
    'splice-1',
    'soybean',
    'primary-tumor',
    'mushroom',
    'lymph',
    'kr-vs-kp',
    'hypothyroid',
    'hepatitis',
    'heart-cleveland',
    'german-credit',
    'australian-credit',
    'audiology',
    'anneal',
])

DIR_RESULTS_DATA = os.path.join("experiments", "results", "data")
DIR_RESULTS_FIGS = os.path.join("experiments", "results", "figures")
TIMESTAMP_FORMAT = "%Y-%m-%d-%H:%M:%S"

CP4IM_NUM_FOLDS = 10

SYNTH_NUM_TREES = 20
SYNTH_TREE_NUM_INTERNAL_NODES = 15
SYNTH_TOTAL_SAMPLES_PER_TREE = 1000
SYNTH_NUM_FEATURES = 40

SEED_CP4IM_STRATIFIED_FOLD_CONSTRUCTOR = 84
SEED_SYNTH_TREE_GENERATOR = 42
SEED_SYNTH_DATA_GENERATOR = 21


def search_target_decorator(search, queue: Queue):
    def search_target_func(*args, **kwargs):
        queue.put(search(*args, **kwargs))
    return search_target_func


# need to create actual functions in order to pickle :(
def maptree_search_wrapper(queue: Queue, *args, **kwargs):
    return search_target_decorator(maptree_search, queue)(*args, **kwargs)


def mcmc_search_wrapper(queue: Queue, *args, **kwargs):
    return search_target_decorator(mcmc_search, queue)(*args, **kwargs)


def smc_search_wrapper(queue: Queue, *args, **kwargs):
    return search_target_decorator(smc_search, queue)(*args, **kwargs)


def cart_search_wrapper(queue: Queue, *args, **kwargs):
    return search_target_decorator(cart_search, queue)(*args, **kwargs)


def dl85_search_wrapper(queue: Queue, *args, **kwargs):
    return search_target_decorator(dl85_search, queue)(*args, **kwargs)


def gosdt_search_wrapper(queue: Queue, *args, **kwargs):
    return search_target_decorator(gosdt_search, queue)(*args, **kwargs)


ALL_SEARCHERS = {
    "CART": cart_search_wrapper,
    "DL8.5": dl85_search_wrapper,
    "GOSDT": gosdt_search_wrapper,
    "MAPTree": maptree_search_wrapper,
    "MCMC": mcmc_search_wrapper,
    "SMC": smc_search_wrapper,
}


def run_search(searcher: str, *args, **kwargs):
    if searcher not in ALL_SEARCHERS:
        raise ValueError(f"{searcher} is not a valid searcher")

    queue = Queue()
    p = Process(target=ALL_SEARCHERS[searcher], args=(queue,) + args, kwargs=kwargs)

    process_timeout = kwargs['time_limit'] * 2 if 'time_limit' in kwargs else None
    p.start()
    result = None
    try:
        result = queue.get(timeout=process_timeout)
        p.join(timeout=5)  # give some time for a nice close
    except (TimeoutError, Empty):
        print("Process timed out")
    finally:
        p.close()
        return result


def load_binary_data(path):
    assert os.path.exists(path)
    binary_data = np.loadtxt(path, delimiter=' ', dtype=np.int32)
    assert np.all((binary_data == 0) | (binary_data == 1))
    X = binary_data[:, :-1]
    y = binary_data[:, -1]
    return X, y


def get_cp4im_data_path(dataset: str):
    assert dataset in CP4IM_DATASET_NAMES
    return os.path.join(DIR_DATA_CP4IM, f"{dataset}.txt")


def get_full_cp4im_dataset(dataset: str):
    file = get_cp4im_data_path(dataset)
    return load_binary_data(file)


def get_stratified_k_folds_cp4im_dataset(dataset: str, k: int = CP4IM_NUM_FOLDS):
    file = get_cp4im_data_path(dataset)
    X, y = load_binary_data(file)
    skf = StratifiedKFold(n_splits=k, shuffle=True, random_state=SEED_CP4IM_STRATIFIED_FOLD_CONSTRUCTOR)
    for train_idxs, test_idxs in skf.split(X, y):
        X_train = X[train_idxs]
        y_train = y[train_idxs]
        X_test = X[test_idxs]
        y_test = y[test_idxs]
        yield X_train, y_train, X_test, y_test


def get_synth_data_samples(tree_id: int, sample_size: int, noise: float):
    assert sample_size <= SYNTH_TOTAL_SAMPLES_PER_TREE

    path_train = os.path.join(DIR_DATA_SYNTH, f"tree{tree_id}-train.txt")
    path_test = os.path.join(DIR_DATA_SYNTH, f"tree{tree_id}-test.txt")
    X_train, y_train = load_binary_data(path_train)
    X_test, y_test = load_binary_data(path_test)

    # apply noise to the training data
    rng = np.random.default_rng(SEED_SYNTH_DATA_GENERATOR)
    flip = rng.random((SYNTH_NUM_TREES, SYNTH_TOTAL_SAMPLES_PER_TREE)) < noise
    y_train ^= flip[tree_id]

    return X_train[:sample_size], y_train[:sample_size], X_test, y_test


def save_results(results: pd.DataFrame, experiment: str, dataset: str):
    df = pd.DataFrame(results)
    timestamp = datetime.now().strftime(TIMESTAMP_FORMAT)
    dir_dataset_results = os.path.join(DIR_RESULTS_DATA, experiment, dataset)
    if not os.path.exists(dir_dataset_results):
        os.makedirs(dir_dataset_results)

    file = os.path.join(dir_dataset_results, f"results-{timestamp}.csv")
    df.to_csv(file)
    print(f"Saved results for experiment {experiment} on dataset {dataset} to file {file}")


def get_latest_results(experiment: str, dataset: str) -> pd.DataFrame:
    results_dir = os.path.join(DIR_RESULTS_DATA, experiment, dataset)
    if not os.path.exists(results_dir):
        raise ValueError(f"Results directory does not exist: {results_dir}")

    results_files = [
        os.path.basename(f) for f in
        glob(os.path.join(results_dir, 'results-*.csv'))
    ]
    timestamps = [
        datetime.strptime(f[len('results-'):-len('.csv')], TIMESTAMP_FORMAT)
        for f in results_files
    ]
    most_recent_results_file = results_files[timestamps.index(max(timestamps))]
    return pd.read_csv(os.path.join(results_dir, most_recent_results_file))


