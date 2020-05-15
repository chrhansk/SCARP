# SCARP

Switching Cost Aware Rounding of Mixed-Integer Optimal Control Problems

## Dependencies

You will need to satisfy the following dependencies in order to compile SCARP

- A C++-11 compiler, such as gcc / clang
- CMake >= 3.1
- Boost >= 1.54

## Build

The following commands build and test SCARP when executed in the
root folder of the project:

- mkdir build
- cd build
- cmake ..
- make
- make test

## Instances

The `dataset/` folder contains some test instances, consisting of
matrices of fractional controls, which can be rounded using SCARP (or
SUR).  The matrices are saved as rows of space-separated values.  Each
row corresponds to one interval of a shooting grid. Consequently, the
values of each row sum up to 1 (being based on a one-hot encoding).
The (optional) output of the rounding algorithms follow the same
convention while being 0/1.

## Solvers

There are currently two solvers available, `scarp_solver` and `sur_solver`.
The options of the solvers can be obtained using: `--help`

```
> scarp_solver --help

Usage: ./src/main/scarp_solver [options] <input> <ouput>
Allowed options:
  --help                   produce help message
  --vanishing_constraints  enable vanishing constraints
  --sur_costs              use SUR costs
  --collect                collect all optimal solutions
  --ampl                   write output in AMPL format
  --scale arg (=1)         scale factor
  --input arg              input file
  --output arg             output file
```

For example, to solve an instance using SCARP with vanishing constraints,
simply use

```
> scarp_solver --vanishing_constraints  /path/to/instance.dat
```
