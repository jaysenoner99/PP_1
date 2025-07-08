import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# Ensure the output directory exists
os.makedirs("plots", exist_ok=True)

# Load the CSV file
k = 20
filename = "plots/speedups_v1_k=" + str(k) + ".csv"
df = pd.read_csv(filename)

# Extract the number of threads (P values) from the column names
thread_counts = [int(col.split("=")[-1]) for col in df.columns[1:]]

# Plot setup
plt.figure(figsize=(9, 6))  # Less rectangular

# Plot actual speedups for each input size n
for index, row in df.iterrows():
    n = row["n"]
    speedups = row[1:].values
    plt.plot(thread_counts, speedups, marker="o", label=f"n={int(n)}")

# Adjust y-axis to just above the max actual speedup
max_speedup = df.iloc[:, 1:].values.max()
plt.ylim(0, max_speedup * 1.1)

# Styling
plt.title(f"Speedup vs. Threads (k = {k})")  # Title includes k
plt.xlabel("Threads (P)")
plt.ylabel("Speedup")
plt.xticks(thread_counts)
plt.grid(True, linestyle="--", alpha=0.6)
plt.legend(title="Input Size (n)")
plt.tight_layout()

# Save the plot as a PDF
plt.savefig("plots/speedup_plot_fix_k=" + str(k) + ".pdf")
