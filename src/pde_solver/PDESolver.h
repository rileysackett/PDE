#ifndef PDESOLVER_H
#define PDESOLVER_H

#include <vector>
#include <functional> // This allows us to pass math functions as variables

class PDESolver {
private:
    // 1. Physical & Discretization Parameters
    double alpha; // Diffusivity constant
    double L;     // Spatial domain length
    double T;     // Final simulation time
    int Nx;       // Number of spatial intervals
    int Nt;       // Number of time steps

    // 2. Derived Parameters
    double dx;    // Spatial step size
    double dt;    // Time step size
    double r;     // Stability parameter

    // 3. Mathematical Functions (Initial and Boundary Conditions)
    std::function<double(double)> initial_cond;
    std::function<double(double)> bc_left;
    std::function<double(double)> bc_right;

public:
    // Constructor declaration
    PDESolver(double alpha_in, double L_in, double T_in, int Nx_in, int Nt_in,
              std::function<double(double)> ic, 
              std::function<double(double)> b_left, 
              std::function<double(double)> b_right);
              
    // We will add the solve method here later!
};

#endif