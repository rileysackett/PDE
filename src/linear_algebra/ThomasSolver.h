#ifndef THOMAS_ALGORITHM_H
#define THOMAS_ALGORITHM_H

#include <vector>
#include "TridiagonalMatrix.h"

// Solves the tridiagonal system Ax = d using the Thomas Algorithm
std::vector<double> solveThomas(const TridiagonalMatrix& A, const std::vector<double>& d);

#endif // THOMAS_ALGORITHM_H