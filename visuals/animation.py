import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import matplotlib.patches as patches

# Load CSV without headers
# Example row: step, x1, y1, x2, y2, ...
df = pd.read_csv("visuals/positions.csv", header=None)

steps = len(df)

fig, ax = plt.subplots()

# Add black circle at (0,3) with radius 8
circle = patches.Circle((0, 3), 8, edgecolor='black', facecolor='none', linewidth=2)
ax.add_patch(circle)

# Skip first column (step index)
coord_cols = df.columns[1:]

# Split into x/y pairs
x_cols = coord_cols[::2]  # even indices after step
y_cols = coord_cols[1::2] # odd indices after step

# Compute global limits
ax.set_xlim(df[x_cols].min().min() - 2, df[x_cols].max().max() + 2)
ax.set_ylim(df[y_cols].min().min() - 2, df[y_cols].max().max() + 2)

# Create particles dynamically
particles = []
colors = ['r', 'g', 'b', 'm', 'c', 'y']  # cycle through colors
for i, (xcol, ycol) in enumerate(zip(x_cols, y_cols)):
    particle = patches.Circle(
        (df[xcol].iloc[0], df[ycol].iloc[0]),
        1,
        color=colors[i % len(colors)],
        alpha=0.6,
        label=f"Particle {i+1}"
    )
    ax.add_patch(particle)
    particles.append((particle, xcol, ycol))

# Update function
def update(frame):
    for particle, xcol, ycol in particles:
        particle.center = (df[xcol].iloc[frame], df[ycol].iloc[frame])
    return [p[0] for p in particles]

ani = FuncAnimation(fig, update, frames=steps, interval=25, blit=True)

plt.show()
