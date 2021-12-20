import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

"""
df = pd.read_csv("hist_thr.txt", names=['num_points', 'dim', 'proc', 'time'])
plt.hist(df['time'], bins=20)
plt.title("Distribution of execution time for 16 processes and same pivot and dataset", fontsize=20)
plt.xlabel("Time(ms)", fontsize=18)
plt.ylabel("Number of occurances", fontsize=18)
plt.show()
"""

df = pd.read_csv("hist_vs.txt", names=['num_points', 'dim', 'proc', 'time'])
val = df.proc.unique()
counter = 0
for i in val:
    plt.bar(counter, np.mean(df.loc[df.proc==i].time), color='blue', width=0.3)
    counter += 1
plt.xticks(range(0, counter),val)
plt.title("Mean execution time in different number of processes", fontsize=20)
plt.xlabel("Number of processes", fontsize=18)
plt.ylabel("Execution time(ms)", fontsize=18)
plt.show()
