#!/bin/bash

#SBATCH --job-name=SpMV-cuda
#SBATCH --output=SPMV-cuda-out-%j.out
#SBATCH --error=SPMV-cuda-err-%j.err
#SBATCH --partition=edu-short
#SBATCH --nodes=1
#SBATCH --gres=gpu:1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1

module load CUDA/12.3.2
for file in $1*
do
	./SpMV_cuda $file
done
