import sys, getopt
import numpy as np


def get_args(argv):
    num = 10
    proc = 2
    dim = 10
    try:
        opts, args = getopt.getopt(argv, "hn:d:", ["num=", "dim="])
    except getopt.GetoptError:
        print('create_data.py -n <num of data> -d <point dimension> \nNOTE: num of data should be even')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print('create_data.py -n <num of data> -p <process>')
            sys.exit()
        elif opt in ("-n", "--num"):
            num = int(arg)
        elif opt in ("-d", "--dim"):
            dim = int(arg)
    if (num % 2 != 0) :
        print("ERROR: num of data should be even")
        sys.exit()
    return [num, dim]


def main(num, dim):
    mat = np.random.rand(num, dim) * 1000
    f = open(f"../data/dt_0_{num}_{dim}.txt", 'w+')
    f.write(f"{num} {dim}\n")
    np.savetxt(f, mat, fmt='%.2f')
    f.close()
        


if __name__ == "__main__":
    num, dim = get_args(sys.argv[1:])
    main(num, dim)
