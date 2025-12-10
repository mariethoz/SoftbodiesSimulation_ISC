import pandas as pd
import matplotlib.pyplot as plt

# Define groups: (name, number of particles)
groups = [
    ("Square1 s=0.9, d=0.1", 4),
    ("Square1 s=0.5, d=0.5", 4),
    ("Square1 s=0.2, d=0.8", 4),
    ("Square1 s=0.9, d=0.8", 4),
    ("Square1 s=0.5, d=0.1", 4),
    ("Square1 s=0.2, d=0.5", 4),
    ("Square1 s=0.9, d=0.5", 4),
    ("Square1 s=0.5, d=0.8", 4),
    ("Square1 s=0.2, d=0.1", 4),
]
X_min = 400
X_max = 1500

# Load CSV without headers
df = pd.read_csv("visuals/positions.csv", header=None)

delta_time = 0.01
coord_cols = df.columns[1:]
x_cols = coord_cols[::2]
y_cols = coord_cols[1::2]

# Choose starting frame (skip noisy beginning)
df_cut = df.iloc[X_min:X_max].reset_index(drop=True)
time = df_cut.index * delta_time + X_min * delta_time

colors = ['r', 'g', 'b', 'm', 'c', 'y']

# Build mapping of groups to particles
grouped_particles = {}
idx = 0
for name, count in groups:
    grouped_particles[name] = []
    for _ in range(count):
        if idx < len(x_cols):
            grouped_particles[name].append((x_cols[idx], y_cols[idx]))
        idx += 1

# Generate one figure per group
for group_name, particles in grouped_particles.items():
    fig, axes = plt.subplots(len(particles), 2, figsize=(10, 2*len(particles)))
    fig.suptitle(group_name, fontsize=16)

    # If only one particle, axes is 1D → normalize to 2D
    if len(particles) == 1:
        axes = [axes]

    for row, (xcol, ycol) in enumerate(particles):
        color = colors[row % len(colors)]
        axes[row][0].plot(time, df_cut[xcol], color=color)
        axes[row][0].set_ylabel(f"P{row+1} X")
        axes[row][0].grid(True)

        axes[row][1].plot(time, df_cut[ycol], color=color)
        axes[row][1].set_ylabel(f"P{row+1} Y")
        axes[row][1].grid(True)

    axes[-1][0].set_xlabel("Time [s]")
    axes[-1][1].set_xlabel("Time [s]")

    plt.tight_layout(rect=[0, 0, 1, 0.95])
    plt.savefig(group_name+".png")
    #plt.show()

# Collect the first particle of each group
first_particles = []
for group_name, particles in grouped_particles.items():
    if particles:  # ensure group has particles
        first_particles.append((group_name, particles[0]))

first_particles.sort()

# Create combined figure
fig, axes = plt.subplots(len(first_particles), 4, figsize=(20, 2*len(first_particles)))
fig.suptitle("First particle of each group", fontsize=16)

# Normalize axes if only one particle
if len(first_particles) == 1:
    axes = [axes]

for row, (group_name, (xcol, ycol)) in enumerate(first_particles):
    color = colors[row % len(colors)]
    x = df_cut[xcol]
    y = df_cut[ycol]
    axes[row][0].plot(time, x, color=color)
    axes[row][0].set_ylabel(f"{group_name}")
    axes[row][0].grid(True)
    
    axes[row][1].plot(time, y, color=color)
    axes[row][1].grid(True)

    # --- X variation relative to final value ---
    x_final = df_cut[xcol].iloc[-1]
    x_var = abs(x - x_final)
    axes[row][2].plot(time, x_var, color=color)
    axes[row][2].set_yscale("symlog", linthresh=1e-2)
    axes[row][2].grid(True)

    # --- Y variation relative to final value ---
    y_final = df_cut[ycol].iloc[-1]
    y_var = abs(y - y_final)
    axes[row][3].plot(time, y_var, color=color)
    axes[row][3].set_yscale("symlog", linthresh=1e-2)
    axes[row][3].grid(True)

    # If you want to emphasize small oscillations, you could use log scale:
    # axes[row][1].set_yscale("symlog", linthresh=1e-2)



axes[0][0].set_title("X")
axes[0][1].set_title("Y")
axes[0][2].set_title("ΔX")
axes[0][3].set_title("ΔY")
axes[-1][0].set_xlabel("Time [s]")
axes[-1][1].set_xlabel("Time [s]")

plt.tight_layout(rect=[0, 0, 1, 0.95])
plt.savefig("first_particles.png")
#plt.show()
