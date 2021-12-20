import numpy as np

def dist(pointa, pointb):
    dist = np.power(np.sum(np.power(pointa-pointb,2)),0.5).astype('float32')
    return dist

f = open("../data/dt_1_16000_784.dat",'rb')
data = np.fromfile(f, dtype='float32')
f.close()
num = int(data[0])
dim = int(data[1])
data = data[2:].reshape(num,dim)
pivot = 14719
dists = np.array([], dtype='float32')

for i in range(num):
    dists = np.append(dists, dist(data[pivot], data[i]))

median = np.median(dists).astype('float32')
print(median)
processes = 4
n = int(num/processes)
dists = np.sort(dists)

for i in range(processes):
	print(f"[{dists[i*n]} {dists[(i+1)*n-1]}]")
