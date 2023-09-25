#!/bin/bash
#SBATCH --job-name=bdt-map
#SBATCH --account=thrun
#SBATCH --partition=thrun
#SBATCH --time=1-0
#SBATCH --mem-per-cpu=16GB
#SBATCH --cpus-per-task=1
#SBATCH --mail-type=ALL
#SBATCH --array=0-51

python -u run_experiment.py -j ${SLURM_ARRAY_TASK_ID}