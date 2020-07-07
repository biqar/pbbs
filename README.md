# Problem Based Benchmark Suite (PBBS)
A cloned repository of CMU's Problem Based Benchmark Suite (PBBS). I have cloned this for personal use only and may have my own updates, please check the change list bellow.

This repository may not have the updates of the original repo, please visit [CMU's Page](http://www.cs.cmu.edu/~pbbs/index.html) to get the updated one.

## Overview

The problem based benchmark suite (PBBS) is designed to be an open source repository to compare different parallel programming methodologies in terms of performance and code quality. The benchmarks define problems in terms of the function they implement and not the particular algorithm or code they use. We encourage people to implement the benchmarks using any algorithm, with any programming language, with any form of parallelism (or sequentially), and for any machine. The problems are selected so they:
* Are representative of a reasonably wide variety of real-world tasks
* The problem can be defined concisely
* Are simple enough that reasonably efficient solutions can be implemented in 500 lines of code, but not trivial microbencharks
* Have outputs that can be easily tested for correctness and possibly quality

**`This is currently work in early progress, and these pages are preliminary.`** The plan is to supply:
1. The definition of the problems in terms of their function specification (the input they take and the required output given the input).
2. For each problem a weighted set of input distributions on which to test and time implementations.
3. Input generators for each of the input distributions.
4. Output testers for testing the correctness and/or quality of the output
5. A sequential and a parallel base implementation for each problem
6. A set of criteria for measuring the quality of code
7. A repository of submitted implementations.

Currently the benchmarks include the following:
* [Comparison sort (SORT)](https://github.com/biqar/pbbs/tree/master/benchmarks/sequences/comparisonSort)
* [Integer sort (ISORT)](https://github.com/biqar/pbbs/tree/master/benchmarks/sequences/integerSort)
* [Remove Duplicates (RDUPS)](https://github.com/biqar/pbbs/tree/master/benchmarks/sequences/removeDuplicates)
* [Dictionary (RDUPS)](https://github.com/biqar/pbbs/tree/master/benchmarks/sequences/dictionary)
* [Suffix Array (SA)](https://github.com/biqar/pbbs/tree/master/benchmarks/strings/suffixArray)
* [Breadth First Search Tree (BFS)](https://github.com/biqar/pbbs/tree/master/benchmarks/graphs/breadthFirstSearch)
* [Maximal Independent Set (MIS)](https://github.com/biqar/pbbs/tree/master/benchmarks/graphs/maximalIndependentSet)
* [Maximal Matching (MIS)](https://github.com/biqar/pbbs/tree/master/benchmarks/graphs/maximalMatching)
* [Minimum Spanning Forest (MSF)](https://github.com/biqar/pbbs/tree/master/benchmarks/graphs/minSpanningForest)
* [Spanning Forest (SF)](https://github.com/biqar/pbbs/tree/master/benchmarks/graphs/spanningForest)
* [Convex Hull (CH)](https://github.com/biqar/pbbs/tree/master/benchmarks/geometry/convexHull)
* [Delaunay Triangulation (DT)](https://github.com/biqar/pbbs/tree/master/benchmarks/geometry/delaunayTriangulation)
* [Delaunay Refinement (DR)](https://github.com/biqar/pbbs/tree/master/benchmarks/geometry/delaunayRefine)
* [K-Nearest Neighbors (KNN)](https://github.com/biqar/pbbs/tree/master/benchmarks/geometry/nearestNeighbors)
* [Ray-Triangle Intersection (RAY)](https://github.com/biqar/pbbs/tree/master/benchmarks/geometry/rayCast)
* [Nbody Forces (NBODY)](https://github.com/biqar/pbbs/tree/master/benchmarks/geometry/nBody)

These are described on the Benchmarks Page.

## Give a Star! :star:
If you like this project please consider giving it a :star: star :star:. Thanks!
