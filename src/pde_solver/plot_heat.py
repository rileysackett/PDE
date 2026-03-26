import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def plot_simulation(csv_file, title):
    # 1. Load the data
    print(f"Loading {csv_file}...")
    df = pd.read_csv(csv_file)
    
    # 2. Get a list of all the unique time steps recorded
    times = df['time'].unique()
    
    # 3. Pick 6 evenly spaced time steps to draw
    indices_to_plot = np.linspace(0, len(times) - 1, 6, dtype=int)
    
    plt.figure(figsize=(10, 6))
    
    # 4. Loop through our chosen times and plot a line for each one
    for idx in indices_to_plot:
        t = times[idx]
        # Filter the data for just this specific moment in time
        data_at_t = df[df['time'] == t]
        
        # Plot X position vs. Temperature
        # Use a colormap (coolwarm) so early times are blue and late times are red
        color_intensity = idx / max(indices_to_plot) 
        plt.plot(data_at_t['x'], data_at_t['u'], 
                 label=f't = {t:.2f}s', 
                 color=plt.cm.coolwarm(color_intensity),
                 linewidth=2)

    # 5. Make the graph look pretty and professional
    plt.title(title, fontsize=16)
    plt.xlabel('Position along Rod (x)', fontsize=12)
    plt.ylabel('Temperature (u)', fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend()
    plt.tight_layout()
    
    # 6. Show the graph on screen
    plt.show()

# Run the plotting function for both of our test files
if __name__ == "__main__":
    plot_simulation("interactive_results.csv", "Crank-Nicolson Results")