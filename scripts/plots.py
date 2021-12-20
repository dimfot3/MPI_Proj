import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

dt_path = {"Uniform": "./data/uni.txt","Normal": "./data/norm.txt","Expodential": "./data/exp.txt", "Mnist":"./data/mnist.txt" }

for i in dt_path.keys():
    f,ax = plt.subplots(1, 1, figsize=(10,7))
    path = dt_path[i]
    df = pd.read_csv (path, names=['num_points', 'dim', 'proc', 'time'])
    best_dim = df['dim'].mode()[0]
    temp = df.loc[df.proc==2].loc[df.dim==best_dim].sort_values(by=['num_points'])
    ax.plot(temp.num_points, temp.time*1000, label="2 Processes")
    temp = df.loc[df.proc==4].loc[df.dim==best_dim].sort_values(by=['num_points'])
    ax.plot(temp.num_points, temp.time*1000, label="4 Processes")
    ax.set_xlabel("Number of Points", fontsize=18)
    ax.set_ylabel("Time(ms)", fontsize=18)
    ax.set_title(f"{i} distribution: Effect of Number of points (with dimension {best_dim})", fontsize=20)
    ax.legend(fontsize=18)
    f.savefig(f'{i}_num_effect.png', dpi=150, bbox_inches='tight')

    f, ax = plt.subplots(1, 1, figsize=(10, 7))
    best_num = df['num_points'].mode()[0]
    temp = df.loc[df.num_points==best_num].loc[df.proc==2].sort_values(by=['dim'])
    ax.plot(temp.dim, temp.time*1000, label="2 Processes")
    temp = df.loc[df.num_points==best_num].loc[df.proc==4].sort_values(by=['dim'])
    ax.plot(temp.dim, temp.time*1000, label="4 Processes")
    ax.set_xlabel("Number of dimension", fontsize=18)
    ax.set_ylabel("Time(ms)", fontsize=18)
    ax.set_title(f"{i} distribution: Effect of Dimension of points (with number of points {best_num})", fontsize=20)
    ax.legend(fontsize=18)
    f.savefig(f'{i}_dim_effect.png', dpi=150, bbox_inches='tight')



