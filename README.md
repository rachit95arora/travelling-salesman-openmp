# Travelling Salesman OpenMP

## Introduction
The Travelling Salesman Problem is a well known NP-Hard problem with a computational time complexity of **O(n!)**. A more scalable solution of TSP is much more favourable even at the cost of optimality. Therefore an AI based solution that is fast and close to optimal seems much more attractive. This is an implementation of several **genetic crossover algorithms** to solve the problem. Clearly time is of critical importance in such a computationally dense problem and it makes a lot of sense to explore any avenues for parallelism. Hence the implementation makes use of the **OpenMP library** to provide maximal speedups.

## Requirements
The code is a C++ implementation and therefore requires g++ with OpenMP support.

## How to use
* The project includes a makefile that can be used to create an executable named `tsp`.
* The number of threads can be set in the environment.  
  ```export OMP_NUM_THREADS=x```
* The executable accepts and requires an input file, output file and afforded time in seconds.  
  ```./tsp input_file_path output_file_path time_in_sec```

## The algorithms
* The project implements chromosome crossover algorithms like Partially Mapped Crossover (PMX), Greedy Crossover (GX), Cycle Crossover (CX) and Edge Recombination Crossover (ERX).
* This is followed by a roulette wheel selection of parents on the basis of their fitness for further crossovers.
* The chromosomes are periodically mutated to prevent gene stagnation.  
  Mutation is especially higher towards the end to evade any local maximas.
* **An additional heuristic** : Maintained a list of 10 best chromosomes at  
  any given point to reseed good solutions into the population time to time  
  preventing their extinction.
