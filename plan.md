1. Setup (inside constructor)

    Inputs: alpha (diffusal constant), length $L$, time $T$, num spatial steps $Nx$, num time steps $Nt$

    Calculate step sizes: ($\Delta x = L / Nx$), ($\Delta t = T / Nt$)

    Calculate stability parameter: $r = \frac{\alpha \cdot \Delta t}{\Delta x^2}$

2. Matrix Construction

    Initialize TridiagonalMatrix objects for $A$ and $B$

    Fill diagonals of matrices using $r$

3. Initial Condition

    Need to know heat at $t=0$

    Create the Grid: Create a std::vector<double> called U to represent the interior points of our 1D rod.

    Apply $f(x)$: Loop over every point in U and use the initial condition function to set the starting temperature for each specific point

4. Main Loop

    Code enters a for loop that runs $Nt$ times

    Takes current temperatures (U) and multiplies them by Matrix B using .multiply(). This creates a new vector, which we call the RHS

    Boundary Corrections: Calculates the left and right boundary temperatures for the current time and adds those specific values to the very first and very last elements of our RHS vector

    Now have: $A \times U_{next} = RHS$. Input matrix A and the RHS vector into solveThomas function which returns $U_{next}$, which are the new temperatures

    Write new temperatures to .csv file. Finally, make $U_{next}$ the new current temperature (U), loop restarts