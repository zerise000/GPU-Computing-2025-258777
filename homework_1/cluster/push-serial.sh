#!/bin/bash

#SBATCH --job-name=SpMV-serial
#SBATCH --output=SPMV-serial-out-%j.out
#SBATCH --error=SPMV-serial-err-%j.err
#SBATCH --partition=edu-short
#SBATCH --nodes=1
#SBATCH --gres=gpu:0
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1

for file in $1*
do
	./SpMV_serial $file
done
