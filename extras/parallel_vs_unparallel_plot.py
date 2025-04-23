import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("../output/performance_results.csv")

colors = {"Unparalleled": "blue", "Parallel": "red"}

iterations = df["Iteration"].unique()

plt.figure(figsize=(12, 6))

# For each iteration, draws the two curves (parallel and not)
for iteration in iterations:
    iter_data = df[df["Iteration"] == iteration]
    for method in ["Unparalleled", "Parallel"]:
        method_data = iter_data[iter_data["Method"] == method]
        plt.plot(
            method_data["MatrixSize"],
            method_data["TimeMs"],
            label=f"{method}" if iteration == 0 else None,
            color=colors[method],
            alpha=0.5
        )

plt.xlabel("Matrix Size")
plt.ylabel("Time (ms)")
plt.title("Matrix-Vector Multiplication Time per Run")
plt.legend()
plt.grid(True, which='both', linestyle='--', linewidth=0.5, alpha=0.7)
plt.tight_layout()
plt.show()
