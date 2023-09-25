from argparse import ArgumentParser

from experiments.experiments.fig1.runner import run as run_fig1
from experiments.experiments.fig2.runner import run as run_fig2
from experiments.experiments.fig3.runner import run as run_fig3

from experiments.globals import CP4IM_DATASET_NAMES, SYNTH_NUM_TREES


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('--job_index', '-j', default=None, type=int)
    args = parser.parse_args()

    jobs = list(range(52)) if args.job_index is None else [args.job_index]

    for job in jobs:
        if job < 2 * len(CP4IM_DATASET_NAMES):
            run = run_fig1 if (job % 2 == 0) else run_fig2
            run(CP4IM_DATASET_NAMES[job // 2])
        else:
            job -= 2 * len(CP4IM_DATASET_NAMES)
            tree_id = job % SYNTH_NUM_TREES
            run_fig3(tree_id)
