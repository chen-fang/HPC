#!/bin/bash

#SBATCH --job-name=montepiMPI
#SBATCH --error=job.%J.err 
#SBATCH --output=job.%J.out
#SBATCH --ntasks=1
#SBATCH --time=10:00
#SBATCH --mem-per-cpu=2048


module load gcc
module load openmpi


cd ~/MontePiEst/
mpirun ./mpi_montepi 5000000000
