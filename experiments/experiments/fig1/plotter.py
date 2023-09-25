import matplotlib.pyplot as plt
import seaborn as sns
import os
import pandas as pd

from experiments.globals import get_latest_results, CP4IM_DATASET_NAMES, DIR_RESULTS_FIGS


def boxplot(model_data, cart_data, file):
    data = pd.concat(model_data)

    cart_data = cart_data[["test_acc", "test_sll", "dataset"]].rename(columns={"test_acc": "base_acc", "test_sll": "base_sll"})
    data = data.merge(cart_data, on="dataset")
    data["test_acc"] = data["test_acc"] - data["base_acc"]
    data["test_sll"] = data["test_sll"] - data["base_sll"]

    figure = plt.figure(layout="constrained", figsize=(8, 6))
    ax = figure.subplots(1, 3)

    sns.boxplot(
        data[(data["searcher"] != "CART")],
        x="Model",
        y="test_acc",
        hue="searcher",
        ax=ax[0],
    )

    sns.boxplot(
        data[(data["searcher"] != "CART")],
        x="Model",
        y="test_sll",
        hue="searcher",
        ax=ax[1],
    )

    sns.boxplot(
        data,
        x="Model",
        y="size",
        hue="searcher",
        ax=ax[2]
    )

    for axis in ax:
        axis.get_legend().remove()
        axis.set_xlabel("Model")
        axis.set_xticklabels(axis.get_xticklabels(), rotation=90)

    ax[0].set_ylabel("Relative Test Accuracy")
    ax[1].set_ylabel("Relative Per-Sample Test Log Likelihood")
    ax[2].set_ylabel("Tree Size")

    fig_file = os.path.join(DIR_RESULTS_FIGS, file)
    if not os.path.exists(DIR_RESULTS_FIGS):
        os.makedirs(DIR_RESULTS_FIGS)
    figure.savefig(fig_file, format='pdf', bbox_inches='tight')


def run():
    print(f"Plotting performance comparison of algorithms on CP4IM datasets...")

    all_results = []
    for dataset in CP4IM_DATASET_NAMES:
        results = get_latest_results("fig1", dataset)
        results["dataset"] = dataset
        all_results.append(results)

    data = pd.concat(all_results) \
        .groupby(['searcher', 'params_id', 'dataset'])[['test_acc', 'test_sll', 'size']] \
        .agg('mean') \
        .reset_index()

    dl85_depth_4_data = data[(data["searcher"] == "DL8.5") & (data["params_id"] == 2)]
    dl85_depth_5_data = data[(data["searcher"] == "DL8.5") & (data["params_id"] == 3)]
    dl85_depth_6_data = data[(data["searcher"] == "DL8.5") & (data["params_id"] == 4)]

    gosdt_slow_data = data[(data["searcher"] == "GOSDT") & (data["params_id"] == 0)]
    gosdt_fast_data = data[(data["searcher"] == "GOSDT") & (data["params_id"] == 1)]

    maptree_data_default = data[(data["searcher"] == "MAPTree") & (data["params_id"] == 2)]
    all_maptree_data = []
    for params_id in range(7):
        all_maptree_data.append(data[(data["searcher"] == "MAPTree") & (data["params_id"] == params_id)])
        all_maptree_data[-1]["Model"] = f"MAPTree (params={params_id})"

    cart_depth_4_data = data[(data["searcher"] == "CART") & (data["params_id"] == 2)]
    cart_depth_5_data = data[(data["searcher"] == "CART") & (data["params_id"] == 3)]
    cart_depth_6_data = data[(data["searcher"] == "CART") & (data["params_id"] == 4)]

    dl85_depth_4_data["Model"] = "DL8.5 (depth=4)"
    dl85_depth_5_data["Model"] = "DL8.5 (depth=5)"
    dl85_depth_6_data["Model"] = "DL8.5 (depth=6)"

    gosdt_slow_data["Model"] = "GOSDT (reg=0.03125)"
    gosdt_fast_data["Model"] = "GOSDT (reg=0.3125)"

    maptree_data_default["Model"] = "MAPTree"

    cart_depth_4_data["Model"] = "CART (depth=4)"
    cart_depth_5_data["Model"] = "CART (depth=5)"
    cart_depth_6_data["Model"] = "CART (depth=6)"

    all_model_data = [
        maptree_data_default,
        dl85_depth_4_data,
        dl85_depth_5_data,
        gosdt_slow_data,
        gosdt_fast_data,
        cart_depth_4_data,
    ]

    boxplot(all_model_data, cart_depth_4_data, "fig1.pdf")
    boxplot(all_maptree_data, cart_depth_4_data, "fig1-appendix.pdf")




