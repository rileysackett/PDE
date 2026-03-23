#include "ThomasSolver.h"
#include <stdexcept>

using namespace std;

vector<double> solveThomas(const TridiagonalMatrix& A, const vector<double>& d) {
    size_t n = A.size();
    if (d.size() != n) throw runtime_error("System size mismatch in Thomas Algorithm");

    vector<double> x(n, 0.0);
    
    // Handle edge cases for very small matrices
    if (n == 0) return x;
    if (n == 1) {
        x[0] = d[0] / A.main[0];
        return x;
    }

    // Temporary vectors for modified coefficients
    vector<double> c_star(n - 1, 0.0);
    vector<double> d_star(n, 0.0);

    // 1. Forward Elimination
    c_star[0] = A.upper[0] / A.main[0];
    d_star[0] = d[0] / A.main[0];

    for (size_t i = 1; i < n; i++) {
        double m = A.main[i] - A.lower[i - 1] * c_star[i - 1];
        if (i < n - 1) {
            c_star[i] = A.upper[i] / m;
        }
        d_star[i] = (d[i] - A.lower[i - 1] * d_star[i - 1]) / m;
    }

    // 2. Back Substitution
    x[n - 1] = d_star[n - 1];
    for (int i = static_cast<int>(n) - 2; i >= 0; i--) {
        x[i] = d_star[i] - c_star[i] * x[i + 1];
    }

    return x;
}