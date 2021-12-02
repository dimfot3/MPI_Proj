import sys, getopt
import numpy as np


def get_args(argv):
    num = 10
    proc = 2
    dim = 10
    try:
        opts, args = getopt.getopt(argv, "hn:p:d:", ["num=", "proc=", "dim="])
    except getopt.GetoptError:
        print('create_data.py -n <num of data> -p <process> -d <point dimension> \nNOTE: num of data and process should be even and num divisable by processes')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print('create_data.py -n <num of data> -p <process>')
            sys.exit()
        elif opt in ("-n", "--num"):
            num = int(arg)
        elif opt in ("-p", "--proc"):
            proc = int(arg)
        elif opt in ("-d", "--dim"):
            dim = int(arg)
    if (num % 2 != 0) | (num % proc != 0) | (proc % 2 != 0):
        print("ERROR: num of data and process should be even and num divisable by processes")
        sys.exit()
    return [num, proc, dim]


def main(num, proc, dim):
    n_per_proc = int(num / proc)
    for i in range(proc):
        mat = np.random.rand(n_per_proc, dim) * 1000
        f = open("../data/file"+str(i)+".txt", 'w+')
        f.write(f"{num} {proc} {dim}\n")
        np.savetxt(f, mat, fmt='%.2f')
        f.close()
        


if __name__ == "__main__":
    num, proc, dim = get_args(sys.argv[1:])
    main(num, proc, dim)
