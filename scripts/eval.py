import numpy as np

def dist(pointa, pointb):
    return sum((pointa-pointb)**2)**0.5

f = open("../data/file.txt")
data = f.readlines()[1:]
f.close()
data = np.loadtxt(data)
pivot =12
dists = np.array([])
for i in range(data.shape[0]):
    dists = np.append(dists, dist(data[pivot], data[i]))

print(np.where(dists[0:500]>np.median(dists)))
