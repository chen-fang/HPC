// Problem 1.6
// [ global sum ]
// Assumption: Core 0 is the master core
// Master core receives data from all of the other cores
// N(receive) = N(addition) = p-1

// [ tree-structure ]
// The total number of iterations is:
// iterN = (int)log2(p-1) + 1
// In each iteration, there are one receive & one addition. So,
// N(receive) = N(addition) = iterN


// Table -----------------------------------------


// Problem 1.7
// It's a combination of both.
// It's task-parallelism because in each iteration, some cores receive and sum
// the values while some others send them.
// It's also data-parallelism because for cores that are assigned with the same kind of work,
// each core carries portion of the total task at the same time.


// Problem 1.8
// [a] task-parallelism
// Preparation for the party may include:
// -- clean the house
// -- buy food
// -- cook
// Faculties can be divided into groups to deal with one the them.


// [b] data-parallelism
// Cleaning the house may include cleaning the
// windows, tables, plates, etc.
// If data-parallelism is applied, then each faculty is equally assigned with partial work out of
// the entire cleaning task. For example, each faculty needs to clean (1) the window, (2) tables
// and (3) plates.

// [c]
// Faculties are divided into several groups. Each group picks one job, while
// members within the same group divide the work equally.


/*
  Problem 1.9

  Parallel implementation of black oil simulator

  Black oil simulation is the basic form of reservoir simulation
  with a list of assumptions that simplifies the physical problems.
  To be specific, compared to compositional model, the phases 
  rather than individual components are tracked to construct the 
  governing equations in black oil model such that thermodynamic 
  equilibrium is ignored and phase behavior can be easily described.
  Even though the black oil model can fail due to its underlying
  assumptions when, for instance, significant component partitioning
  occurs during displacement, it is still the most widely used model
  in the industry since its computational requirement is much lower
  than its compositional counterpart. In recent years,
  however, large scale simulation with million to billion grid
  blocks are gaining attention in the industry, challenging the
  efficiency of the black oil simulation.
  In this project, a parallel black oil simulator will be implemented
  When the system is large, around 70% of the total time is spent
  on solving the equations in iterations. As a result, in addition
  to the discretization and Jacobian evaluation procedures, more
  emphasis will be put on parallelizing solvers of the system.
  With referencing some most recently published work, the most 
  commonly used preconditioners will be implemented. Also, newly
  developed preconditioners that may be more suitable for HPC will
  be coded as well. Tests will be run to compare the results to
  conclude the effect of parallel computations in the context of
  black oil simulation.
*/
