import sys, getopt
import numpy as np


def get_args(argv):
    num = 10
    dim = 10
    option = 1
    try:
        opts, args = getopt.getopt(argv, "hn:d:o:", ["num=", "dim=", "option="])
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
        elif opt in ("-o", "--option"):
            option = int(arg)
    if (num % 2 != 0) :
        print("ERROR: num of data should be even")
        sys.exit()
    return [num, dim, option]


def main(num, dim, opt):
    if(opt==1):
        mat = np.random.rand(num, dim) * 1000
    elif(opt==2):
        mat = mat = np.random.normal(0, 1000, [num,dim])
    elif(opt==3):
        mat = np.random.exponential(scale=1000, size=[num, dim])
    mat = mat.astype('float32')
    info = np.array([num,dim],'float32')
    f = open(f"../data/dt_{opt}_{num}_{dim}.dat", 'wb')
    info.tofile(f)
    mat.tofile(f)
    f.close()
        


if __name__ == "__main__":
    num, dim, opt = get_args(sys.argv[1:])
    main(num, dim, opt)
