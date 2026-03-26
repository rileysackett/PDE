#ifndef TRIDIAGONAL_MATRIX_H
#define TRIDIAGONAL_MATRIX_H

#include <vector>
#include <cstddef>
#include <stdexcept>
#include <iostream>
using namespace std;

struct TridiagonalMatrix {
    vector<double> lower; // Sub-diagonal (n-1 elements)
    vector<double> main;  // Main diagonal (n elements)
    vector<double> upper; // Super-diagonal (n-1 elements)

    TridiagonalMatrix() = default;

    explicit TridiagonalMatrix(size_t n)
        : lower((n>0? n-1:0), 0.0), main(n, 0.0), upper((n>0? n-1: 0), 0.0) {}

    size_t size() const { return main.size(); }

    // Getter
    double operator()(size_t i, size_t j) const {
        if(i == j) {
            return main[i];
        } else if(i == j + 1) {
            return lower[j];
        } else if(i == j - 1) {
            return upper[i];
        } else {
            return 0.0;
        }
    }
    
    // Setter
    double& operator()(size_t i, size_t j) {
        if(i == j) {
            return main[i];
        } else if(i == j + 1) {
            return lower[j];
        } else if(i == j - 1) {
            return upper[i];
        } else {
            throw out_of_range("Non-tridiagonal element set");
        }
    }

    vector<double> multiply(const vector<double>& vec) const {
        size_t n = size();
        if(vec.size() != n) throw runtime_error("Size mismatch"); // Size error handling 


        vector<double> result(n, 0.0); // Create result vector and init with 0.0s

        if (n == 1) {
            result[0] = main[0] * vec[0];
            return result;
        }

        result[0] = main[0] * vec[0] + upper[0] * vec[1]; // First element
        for(size_t i = 1; i < n - 1; i++) {
            result[i] = lower[i-1] * vec[i - 1] + main[i] * vec[i] + upper[i] * vec[i + 1];
        }
        result[n - 1] = lower[n - 2] * vec[n - 2] + main[n - 1] * vec[n - 1]; // Last elements

        return result;
    }

    void print() const {
        for(size_t i = 0; i < size(); i++) {
            for (size_t j = 0; j < size(); ++j) {
                cout << (*this)(i,j) << " ";
            }
            cout << "\n";
        }
    }
};


#endif