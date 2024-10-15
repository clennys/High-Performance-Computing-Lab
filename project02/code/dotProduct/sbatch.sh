#!/bin/bash
#SBATCH --job-name=dot-product			# Job name
#SBATCH --output=output_%j.log          # Output file (with job ID)
#SBATCH --error=error_%j.log            # Error file (with job ID)
#SBATCH --time=00:10:00                 # Time limit hrs:min:sec
#SBATCH --ntasks=1						# Number of tasks
#SBATCH --cpus-per-task=20				# Number of CPUs per task

module load gcc

make clean
make

srun ./dotProduct.exe

