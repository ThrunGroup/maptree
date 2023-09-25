# Hello AAAI reviewers! Thank you for taking the time to look over our work.

You can replicate all of our results from this directory as follows:

# Set Up Environment
```
pip install -r requirements.txt
pip install .
```

# Install Data
```
python setup_data.py
```

# Run Experiments (on personal computer) [$\approx$ 1 week]
```
python run_experiment.py
```

# Experiments (on cluster) [$\approx$ 1 day]
```
sbatch script.slurm
```

# Plot Results
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