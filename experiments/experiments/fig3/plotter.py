import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
import seaborn as sns
import os
import pandas as pd

from experiments.globals import get_latest_results, SYNTH_NUM_TREES, DIR_RESULTS_FIGS
from .constants import SAMPLE_SIZE_VALUES, NOISE_VALUES


def run():
    print(f"Plotting performance comparison of algorithms on synth datasets...")

    figure = plt.figure(layout="constrained", figsize=(6, 8))
    axes = figure.subplots(len(NOISE_VALUES), 1)

    all_results = []
    for i in range(SYNTH_NUM_TREES):
        results = get_latest_results("fig3", f"tree{i}")
        all_results.append(results)

    data = pd.concat(all_results)
    dl85_depth_4_data = data[(data["searcher"] == "DL8.5") & (data["params_id"] == 2)]
    dl85_depth_5_data = data[(data["searcher"] == "DL8.5") & (data["params_id"] == 3)]
    dl85_depth_6_data = data[(data["searcher"] == "DL8.5") & (data["params_id"] == 4)]
    gosdt_slow_data = data[(data["searcher"] == "GOSDT") & (data["params_id"] == 0)]
    gosdt_fast_data = data[(data["searcher"] == "GOSDT") & (data["params_id"] == 1)]
    maptree_data = data[(data["searcher"] == "MAPTree") & (data["params_id"] == 0)]
    cart_4_data = data[(data["searcher"] == "CART") & (data["params_id"] == 2)]
    cart_5_data = data[(data["searcher"] == "CART") & (data["params_id"] == 2)]
    cart_6_data = data[(data["searcher"] == "CART") & (data["params_id"] == 2)]

    maptree_data["model"] = "MAPTree"

    dl85_depth_4_data["model"] = "DL8.5 (depth=4)"
    dl85_depth_5_data["model"] = "DL8.5 (depth=5)"
    dl85_depth_6_data["model"] = "DL8.5 (depth=6)"

    gosdt_slow_data["model"] = "GOSDT (reg=0.03125)"
    gosdt_fast_data["model"] = "GOSDT (reg=0.3125)"

    cart_4_data["model"] = "CART (depth=4)"
    cart_5_data["model"] = "CART (depth=5)"
    cart_6_data["model"] = "CART (depth=6)"

    data = pd.concat([
        maptree_data,
        dl85_depth_4_data,
        dl85_depth_5_data,
        # dl85_depth_6_data,
        gosdt_slow_data,
        gosdt_fast_data,
        cart_4_data,
        # cart_5_data,
        # cart_6_data,
    ])

    for i, noise in enumerate(NOISE_VALUES):
        noise_data = data[data["noise_id"] == i]
        noise_data["num_samples"] = noise_data["sample_size_id"].map(lambda id: SAMPLE_SIZE_VALUES[int(id)])
        ax = axes[i]

        sns.lineplot(
            noise_data,
            x="num_samples",
            y="test_acc",
            style="model",
            markers=True,
            hue="searcher",
            estimator="mean",
            errorbar=("ci", 95),
            ax=ax,
        )

        ax.set_title(f"$\epsilon$ = {noise}")
        ax.set_xlabel("Numer of Training Samples")
        ax.set_ylabel("Test Accuracy")
        ax.set_ylim([0.5, 1.0])
        ax.get_legend().remove()

    handles, labels = ax.get_legend_handles_labels()

    new_labels = []
    new_handles = []

    for i in range(6, len(labels)):
        model_line = handles[labels.index(labels[i].split(' ')[0])]
        new_labels.append(labels[i])
        new_handles.append(Line2D([], [], color=model_line.get_color(), marker=handles[i].get_marker(), linestyle=handles[i].get_linestyle()))

    legend = figure.legend(
        labels=new_labels,
        handles=new_handles,
        title='Model',
        loc='lower center',
        ncol=3,
        bbox_to_anchor=(0.5, -0.1),
    )

    fig_file = os.path.join(DIR_RESULTS_FIGS, f"fig3.pdf")
    if not os.path.exists(DIR_RESULTS_FIGS):
        os.makedirs(DIR_RESULTS_FIGS)
    figure.savefig(fig_file, format='pdf', bbox_extra_artists=(legend,), bbox_inches='tight')
