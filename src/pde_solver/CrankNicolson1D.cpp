#include "CrankNicolson1D.h"
#include "../linalg/TridiagonalMatrix.h"
#include "../linalg/ThomasSolver.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>

using namespace std;

// Constructor Implementation
CrankNicolson1D::CrankNicolson1D(double alpha_in, double L_in, double T_in, int Nx_in, int Nt_in,
    function<double(double)> ic, 
    function<double(double)> b_left, 
    function<double(double)> b_right)
    // 1. Store the direct inputs using an initializer list
    : alpha(alpha_in), L(L_in), T(T_in), Nx(Nx_in), Nt(Nt_in),
      initial_cond(ic), bc_left(b_left), bc_right(b_right) 
{
    // 2. Calculate the spatial step size (dx)
    dx = L / Nx;

    // 3. Calculate the time step size (dt)
    dt = T / Nt;

    // 4. Calculate the stability parameter (r)
    // Formula: r = alpha * (dt / dx^2)
    r = alpha * dt / (dx * dx);

    // Print confirmation
    cout << "Solver initialized." << endl;
    cout << "dx: " << dx << " | dt: " << dt << " | r: " << r << endl;
}

void CrankNicolson1D::solveAndSave(const string& filename) {
    // The number of interior points we need to solve for
    int n_inner = Nx - 1; 
    
    // ==========================================
    // Initializing Matrices
    // ==========================================
    
    // Create our A (Implicit) and B (Explicit) matrices
    TridiagonalMatrix A(n_inner);
    TridiagonalMatrix B(n_inner);
    
    for (int i = 0; i < n_inner; ++i) {
        // Main diagonal
        A.main[i] = 1.0 + r;
        B.main[i] = 1.0 - r;
        
        // Lower diagonal (skip the very first row)
        if (i > 0) {
            A.lower[i-1] = -r / 2.0;
            B.lower[i-1] =  r / 2.0;
        }
        
        // Upper diagonal (skip the very last row)
        if (i < n_inner - 1) {
            A.upper[i] = -r / 2.0;
            B.upper[i] =  r / 2.0;
        }
    }
    cout << "Matrices A and B constructed." << endl;

    // ==========================================
    // Initialize Rod
    // ==========================================
    
    // Create the vector to hold interior points of rod
    vector<double> U(n_inner);
    
    for (int i = 0; i < n_inner; ++i) {
        // Calculate physical x location: x_i = (i + 1) * dx
        double x = (i + 1) * dx;
        
        // Apply the initial condition function
        U[i] = initial_cond(x);
    }
    cout << "Initial condition applied." << endl;

    // Setup the CSV file to record data
    ofstream outFile(filename);
    outFile << "time,x,u\n";
    
    // Record the t=0 starting state to the file
    for (int i = 0; i < n_inner; ++i) {
        double x = (i + 1) * dx;
        outFile << 0.0 << "," << x << "," << U[i] << "\n";
    }

    // ==========================================
    // Main Solving Loop
    // ==========================================    
    
    cout << "Starting time simulation..." << endl;

    for (int n = 0; n < Nt; ++n) {
        // 1. Figure out exact time for this step
        double t_current = n * dt;
        double t_next = (n + 1) * dt;

        // 2. The Explicit Step: RHS = B * U_current
        vector<double> RHS = B.multiply(U);

        // 3. Add Boundary Corrections
        // Heat might be entering/leaving the edges, so adjust the first and last interior points
        RHS[0] += (r / 2.0) * (bc_left(t_current) + bc_left(t_next));
        RHS[n_inner - 1] += (r / 2.0) * (bc_right(t_current) + bc_right(t_next));

        // 4. The Implicit Step: Solve A * U_next = RHS
        U = solveThomas(A, RHS);


        for (int i = 0; i < n_inner; ++i) {
            // Test 1: Math Stability (Catch division by zero or exploding matrices)
            assert(!std::isnan(U[i]) && "FATAL ERROR: Temperature became NaN!");
            assert(!std::isinf(U[i]) && "FATAL ERROR: Temperature blew up to Infinity!");
            
            // Test 2: Physical Bounds (Assuming our tests stay between -10 and 110 degrees)
            // If it goes outside this range, energy was magically created/destroyed.
            assert(U[i] >= -10.0 && U[i] <= 110.0 && "FATAL ERROR: Maximum Principle Violated!");
        }


        // 5. Record data to CSV
        // To keep file size small, save data every 10% of the simulation
        if ((n + 1) % (Nt / 10) == 0 || n == Nt - 1) {
            for (int i = 0; i < n_inner; ++i) {
                double x = (i + 1) * dx;
                outFile << t_next << "," << x << "," << U[i] << "\n";
            }
        }
    }
    
    outFile.close();
    cout << "Simulation complete. Data saved to " << filename << endl;
}