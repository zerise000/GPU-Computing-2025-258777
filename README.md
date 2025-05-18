# GPU Computing deliverables

## Homework 1

To compile the serial implementation of the deliverable just execute
```
make serial
```
For the GPU implementation instead
```
make cuda
```
Directories are structured in the following way:
- **src**: contains the source code of the homework,divided in two
subfolders called *serial* and *cuda* which have the code of the
classic and GPU implementation respectively

- **input_files:** contains the input matrices used during the 
deliverable

- **cluster**: inside the folder there are the scripts submitted to the cluster,
both of them must be executed with the following commands 
```sbatch cluster/<script file> input_files```
