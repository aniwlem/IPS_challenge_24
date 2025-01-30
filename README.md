
# Coding challenge - Particle simulations
A C++ based, multithreaded solution for the IPS-student challenge for FARM
2024. The algorithm used is based on [cell
      lists](https://en.wikipedia.org/wiki/Cell_lists). Runs in $\mathcalO(NG)$
time, where $N$ is the number of points and $G$ is the number of grid cells.
Works better for dense point clouds.

## Build instructions 
Just run ```make``` to compile everything, creates executable called
'count_pairs' in main directory.
