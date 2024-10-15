#!/bin/bash
#SBATCH --job-name=my_application_job      # Job name
#SBATCH --output=output_%j.log             # Output file (with job ID)
#SBATCH --error=error_%j.log               # Error file (with job ID)
#SBATCH --ntasks=1                         # Run on a single CPU
#SBATCH --time=00:10:00                    # Time limit hrs:min:sec
#SBATCH --partition=general                # Partition name

module load gcc

APPLICATION_NAME="My Application"

echo "Running application: $APPLICATION_NAME"

echo "Time: $(date)"

srun ./my_application_executable

