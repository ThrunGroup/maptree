# MAPTree

This repository contains all of the code to reproduce the experiments from 
'_MAPTree: Beating "Optimal" Decision Trees with Bayesian Decision Trees_' 
by Colin Sullivan*, Mo Tiwari*, and Sebastian Thrun.

Our main algorithm is written in C++ and called from Python via Python bindings.

Below, we have instructions on how to reproduce all of our results.
If you have a question about our code, please submit a Github issue.

## Set Up The Environment

Our code only supports Python 3.10.
Note that our dependencies require installing the old `sklearn` package 
(_not_ `scikit-learn`); you may need run the following command to allow this:

```
export SKLEARN_ALLOW_DEPRECATED_SKLEARN_PACKAGE_INSTALL=True
```

### M1 Macs

If you're on an M1 Mac, you'll need to install `gosdt` from source; see the instructions 
[here](https://github.com/ubc-systopia/gosdt-guesses/blob/main/doc/build.md). Afterwards, install
the other dependencies and the `maptree` Python package with

```
python -m pip install -r requirements_m1mac.txt
python -m pip install .
```

### Other platforms

If you're not on an M1 Mac, you can install all dependencies and then build the `maptree` Python package directly:

```
python -m pip install -r requirements.txt
python -m pip install .
```

## Install Data

Download all the necessary data with

```
python setup_data.py
```

## Run Experiments on a Personal Computer

We support running all experiments on a single machine (e.g., a personal laptop) with:

```
python run_experiment.py
```

Note that this may take a long time (1-2 weeks), depending on your hardware.

## Run Experiments on a Cluster

We have also included a script to run the experiments on a cluster via SLURM, which can be invoked with:

```
sbatch script.slurm
```

Note that this may take some time (1-2 days) to complete, depending on your hardware, and that the 
parameters of the `script.slurm` file may need to be modified according to your cluster setup.

## Plot Results

You can plot the results of all experiments with:

```
python plot_results.py
```

The plots can be found in `experiments/results/figures`.

Hyperparameters for all the experiments can be found in the following files:
```
experiments/globals.py
experiments/experiments/fig1/constants.py
experiments/experiments/fig2/constants.py
experiments/experiments/fig3/constants.py
```