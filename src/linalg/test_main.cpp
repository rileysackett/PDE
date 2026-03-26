// test_main.cpp
#include <iostream>
#include <vector>
#include <cmath>
#include "TridiagonalMatrix.h"
#include "ThomasSolver.h" // Updated to match your filename

using namespace std;

// Helper function to compare vectors with a small tolerance for floating-point accuracy
bool checkVectorsEqual(const vector<double>& a, const vector<double>& b, double tol = 1e-6) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (abs(a[i] - b[i]) > tol) return false;
    }
    return true;
}

int main() {
    cout << "--- Testing Tridiagonal Matrix & Thomas Algorithm ---" << endl;

    // 1. Setup the known 4x4 system
    TridiagonalMatrix A(4);
    A.lower = {-1.0, -1.0, -1.0};
    A.main  = { 2.0,  2.0,  2.0,  2.0};
    A.upper = {-1.0, -1.0, -1.0};

    vector<double> expected_x = {1.0, 1.0, 1.0, 1.0};
    vector<double> d = {1.0, 0.0, 0.0, 1.0};

    // 2. Test Matrix Multiplication
    cout << "Testing Matrix Multiplication (A * x = d)... ";
    vector<double> mult_result = A.multiply(expected_x);
    if (checkVectorsEqual(mult_result, d)) {
        cout << "PASS" << endl;
    } else {
        cout << "FAIL" << endl;
    }

    // 3. Test Thomas Algorithm
    cout << "Testing Thomas Algorithm (solve A * x = d for x)... ";
    vector<double> solved_x = solveThomas(A, d); // Assuming your function is still named solveThomas
    if (checkVectorsEqual(solved_x, expected_x)) {
        cout << "PASS" << endl;
    } else {
        cout << "FAIL" << endl;
        // Print the vectors if it fails so we can see what went wrong
        cout << "  Expected: ";
        for(auto val : expected_x) cout << val << " ";
        cout << "\n  Got:      ";
        for(auto val : solved_x) cout << val << " ";
        cout << endl;
    }

    return 0;
}