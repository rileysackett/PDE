#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include "CrankNicolson1D.h"
#include <string>
#include "../linalg/exprtk.hpp"

using namespace std;

int main() {
    cout << "=========================================" << endl;
    cout << "   Interactive 1D Heat Equation Solver   " << endl;
    cout << "=========================================" << endl;

    // ---------------------------------------------------------
    // 1. Get Physical Parameters from the User
    // ---------------------------------------------------------
    double alpha, L, T;
    int Nx, Nt;
    
    cout << "Enter thermal diffusivity (alpha) [e.g., 0.01]: "; cin >> alpha;
    cout << "Enter length of rod (L)           [e.g., 1.0]:  "; cin >> L;
    cout << "Enter total simulation time (T)   [e.g., 2.0]:  "; cin >> T;
    cout << "Enter spatial steps (Nx)          [e.g., 50]:   "; cin >> Nx;
    cout << "Enter time steps (Nt)             [e.g., 500]:  "; cin >> Nt;

    // Clear the newline character left in the input buffer
    cin.ignore(10000, '\n');
    cout << endl;

    // ---------------------------------------------------------
    // 2. Get Mathematical Functions as Text
    // ---------------------------------------------------------
    string ic_str, bc_left_str, bc_right_str;
    
    cout << "Enter Initial Condition f(x) [e.g., sin(pi*x)]: "; 
    getline(cin, ic_str);
    
    cout << "Enter Left Boundary g(t)     [e.g., 0]:         "; 
    getline(cin, bc_left_str);
    
    cout << "Enter Right Boundary h(t)    [e.g., 0]:         "; 
    getline(cin, bc_right_str);
    cout << endl;

    // ---------------------------------------------------------
    // 3. Set up the ExprTk Parsers
    // ---------------------------------------------------------
    double x_var, t_var; // The variables that ExprTk will modify
    
    // Set up a table for 'x' (Spatial condition)
    exprtk::symbol_table<double> sym_table_x;
    sym_table_x.add_variable("x", x_var);
    sym_table_x.add_constants(); // Automatically adds 'pi', 'e', etc.
    
    // Set up a table for 't' (Time conditions at boundaries)
    exprtk::symbol_table<double> sym_table_t;
    sym_table_t.add_variable("t", t_var);
    sym_table_t.add_constants();

    // Link the tables to expressions
    exprtk::expression<double> expr_ic, expr_bc_left, expr_bc_right;
    expr_ic.register_symbol_table(sym_table_x);
    expr_bc_left.register_symbol_table(sym_table_t);
    expr_bc_right.register_symbol_table(sym_table_t);

    // Compile the strings into executable math
    exprtk::parser<double> parser;
    if (!parser.compile(ic_str, expr_ic)) { cout << "Error parsing Initial Condition!\n"; return 1; }
    if (!parser.compile(bc_left_str, expr_bc_left)) { cout << "Error parsing Left Boundary!\n"; return 1; }
    if (!parser.compile(bc_right_str, expr_bc_right)) { cout << "Error parsing Right Boundary!\n"; return 1; }

    // ---------------------------------------------------------
    // 4. Wrap the expressions in C++ Lambda Functions
    // ---------------------------------------------------------
    // The solver passes in an 'x' or 't', then ExprTk variable is updated, and return the evaluated answer
    auto ic_func       = [&](double x) { x_var = x; return expr_ic.value(); };
    auto bc_left_func  = [&](double t) { t_var = t; return expr_bc_left.value(); };
    auto bc_right_func = [&](double t) { t_var = t; return expr_bc_right.value(); };

    // ---------------------------------------------------------
    // 5. Run the Solver
    // ---------------------------------------------------------
    cout << "Running Simulation..." << endl;
    CrankNicolson1D solver(alpha, L, T, Nx, Nt, ic_func, bc_left_func, bc_right_func);
    solver.solveAndSave("interactive_results.csv");

    cout << "\nSuccess! You can now plot 'interactive_results.csv' with your Python script." << endl;
    return 0;
}