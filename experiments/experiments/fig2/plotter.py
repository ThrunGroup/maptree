import matplotlib.pyplot as plt
import seaborn as sns
import os

from experiments.globals import get_latest_results, CP4IM_DATASET_NAMES, DIR_RESULTS_FIGS


def run():
    print(f"Plotting speed comparison of algorithms on CP4IM datasets...")

    figure = plt.figure(layout="constrained", figsize=(6, 4))
    nrows = 2
    ncols = 2
    axes = figure.subplots(nrows, ncols)

    for i, dataset in enumerate(CP4IM_DATASET_NAMES[:4]):
        ax = axes[i // ncols, i % ncols]
        results = get_latest_results("fig2", dataset)

        # average times across runs
        avg_times = results \
            .groupby(["searcher", "params_id"])['time'] \
            .agg('mean') \
            .reset_index()
        results = results[["searcher", "params_id", "post", "best_post"]] \
            .merge(avg_times, on=["searcher", "params_id"])

        sns.lineplot(
            results,
            x="time",
            y="post",
            hue="searcher",
            estimator="mean",
            errorbar=("ci", 95),
            ax=ax,
        )

        ax.set_title(dataset)
        ax.set_xlabel("Time (s)")
        ax.set_xscale("log")
        ax.set_ylabel("Log Posterior")

        handles, labels = ax.get_legend_handles_labels()
        ax.get_legend().remove()

    legend = figure.legend(
        handles=handles,
        labels=labels,
        title='Model',
        loc='lower center',
        ncol=3,
        bbox_to_anchor=(0.5, -0.13),
    )

    fig_file = os.path.join(DIR_RESULTS_FIGS, f"fig2.pdf")
    if not os.path.exists(DIR_RESULTS_FIGS):
        os.makedirs(DIR_RESULTS_FIGS)

    figure.savefig(fig_file, format='pdf', bbox_extra_artists=(legend,), bbox_inches='tight')

