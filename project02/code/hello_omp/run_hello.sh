#!/bin/bash
#SBATCH --job-name=hello_omp      # Job name    (default: sbatch)
#SBATCH --output=hello_omp-%j.out # Output file (default: slurm-%j.out)
#SBATCH --error=hello_omp-%j.err  # Error file  (default: slurm-%j.out)
#SBATCH --ntasks=1                # Number of tasks
#SBATCH --cpus-per-task=8         # Number of CPUs per task
#SBATCH --mem-per-cpu=1024        # Memory per CPU
#SBATCH --time=00:05:00           # Wall clock time limit

# Load some modules & list loaded modules
module load gcc
module list

# Compile
make clean
make

# Run the program with 8 OpenMP threads
export OMP_NUM_THREADS=8
./hello_omp
