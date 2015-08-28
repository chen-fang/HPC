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
// It's also data-parallelism because each core carries portion of the total task
// at the same time.


// Problem 1.8
// [a]
// cleaning the house
// buy pizza


// [b]
// Cleaning the house may include cleaning the
// windows, tables, plates, floors etc.
// If data-parallelism is applied, then all faculties do partial work on the same
// type at the same time. For example, each faculty cleans N/p of all the windows.

// [c]
// Faculties are divided into several groups. Each group picks one job, while
// members within the group divide the work.
