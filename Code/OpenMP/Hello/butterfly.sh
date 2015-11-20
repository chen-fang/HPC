#!/bin/bash

#SBATCH --job-name=HelloOpenMP
#SBATCH --error=job.%J.err 
#SBATCH --output=job.%J.out
#SBATCH --ntasks=4
#SBATCH --time=10:00
#SBATCH --mem-per-cpu=2048


module load gcc
module load openmpi


cd ~/Hello/
./run
