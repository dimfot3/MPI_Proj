import sys, getopt
import torch
import torchvision.datasets as datasets
import numpy as np

def get_args(argv):
    dt_opt = 0
    num = 1000
    try:
        opts, args = getopt.getopt(argv, "ho:n:", ["dt_opt=", "num="])
    except getopt.GetoptError:
        print('create_data.py -o <option of dataset> -n <number of points> \nNOTE: num of data should be even')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print('create_data.py -n <num of data> -p <process>')
            sys.exit()
        elif opt in ("-o", "--dt_opt"):
            dt_opt = int(arg)
        elif opt in ("-n", "--num"):
            num = int(arg)
    if (num % 2 != 0) :
        print("ERROR: num of data should be even")
        sys.exit()
    return [dt_opt, num]


def main(opt, num):
    if opt==1:
        dataset = datasets.MNIST(root='./', train=True, download=True, transform=None)
    elif opt==2:
        dataset = datasets.FashionMNIST(root='./', train=True, download=True, transform=None)
    else:
    	print("Wrong option\n")
    	sys.exit()
    data_len = min(len(dataset), num)
    dim = np.asarray(dataset[0][0]).reshape(1,-1)[0].shape[0]
    f = open(f"../data/dt_{opt}_{num}.txt", "w")
    f.write(f"{data_len} {dim}\n")
    
    arr = dataset.data.numpy().reshape(-1,dim)
    np.savetxt(f, arr[:data_len], fmt='%.2f')
    f.close()
        


if __name__ == "__main__":
    opt, num = get_args(sys.argv[1:])
    main(opt, num)
