import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import matplotlib.patches as patches

# Load your data
df = pd.read_csv("visuals/positions.csv", names=["x","y","x1","y1"])
steps = len(df)

fig, ax = plt.subplots()

# Add black circle at (0,3) with radius 8
circle = patches.Circle((0, 3), 8, edgecolor='black', facecolor='none', linewidth=2)
ax.add_patch(circle)

ax.set_xlim(df[["x","x1"]].min().min() - 2, df[["x","x1"]].max().max() + 2)
ax.set_ylim(df[["y","y1"]].min().min() - 2, df[["y","y1"]].max().max() + 2)

# Create particle circles with radius = 1 unit
particle1 = patches.Circle((df["x"].iloc[0], df["y"].iloc[0]), 1, color='r', alpha=0.6, label="Particle 1")
particle2 = patches.Circle((df["x1"].iloc[0], df["y1"].iloc[0]), 1, color='g', alpha=0.6, label="Particle 2")
ax.add_patch(particle1)
ax.add_patch(particle2)

ax.legend()

def update(frame):
    particle1.center = (df["x"].iloc[frame], df["y"].iloc[frame])
    particle2.center = (df["x1"].iloc[frame], df["y1"].iloc[frame])
    return particle1, particle2

ani = FuncAnimation(fig, update, frames=steps, interval=10, blit=True)

plt.show()
