import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

# Path to the CSV file
csv_path = "plots/clustered_data_sequential.csv"  # Change this to your actual file path

# Load the CSV file
df = pd.read_csv(csv_path)

# Check columns
if not {"x", "y", "cluster"}.issubset(df.columns):
    raise ValueError("CSV must contain 'x', 'y', and 'cluster' columns")

# Create the plot
plt.figure(figsize=(8, 6))
sns.scatterplot(
    data=df, x="x", y="y", hue="cluster", palette="tab10", s=50, edgecolor="black"
)

# Title and labels
plt.title("Clustered Points-Sequential")
plt.xlabel("X")
plt.ylabel("Y")
plt.legend(title="Cluster")
plt.grid(True, linestyle="--", alpha=0.5)
plt.tight_layout()

# Save the plot
output_path = os.path.splitext(csv_path)[0] + "_plot.pdf"
plt.savefig(output_path)
print(f"Plot saved to {output_path}")
