import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


#num plots
dt_path = {"Uniform": "./res/uni.txt","Normal": "./res/norm.txt","Expodential": "./res/exp.txt", "Mnist":"./res/mnist.txt" }
f,ax = plt.subplots(1, 1, figsize=(10,7))
for i in dt_path.keys():
    path = dt_path[i]
    df = pd.read_csv (path, names=['num_points', 'dim', 'proc', 'time'])
    temp = df.loc[(df.dim==784) & (df.time<0.8)]
    ax.scatter(temp.num_points, temp.time*1000, label=f"{i}")
    ax.set_xlabel("Number of Points", fontsize=18)
    ax.set_ylabel("Time(ms)", fontsize=18)
    ax.set_title(f"Effect of Number of points (with dimension 784) in different distributions", fontsize=20)
ax.legend(fontsize=18)
f.savefig(f'num_effect.png', dpi=150, bbox_inches='tight')

#dim plot
f,ax = plt.subplots(1, 1, figsize=(10,7))
for i in dt_path.keys():
    path = dt_path[i]
    df = pd.read_csv (path, names=['num_points', 'dim', 'proc', 'time'])
    temp = df.loc[(df.num_points==32000) & (df.time<0.3)]
    ax.scatter(temp.dim, temp.time*1000, label=f"{i}")
    ax.set_xlabel("Dimension of Points", fontsize=18)
    ax.set_ylabel("Time(ms)", fontsize=18)
    ax.set_title(f"Effect of Dimenesion of points (with number of points 32000) in different distributions", fontsize=20)
ax.legend(fontsize=18)
f.savefig(f'dim_effect.png', dpi=150, bbox_inches='tight')