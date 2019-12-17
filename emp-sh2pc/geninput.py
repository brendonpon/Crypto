import argparse, random, os, math
#import numpy as np
from operator import mul
#include <string>

def create_dirs(program):
    dirname = "data/"+program
    if not os.path.exists(dirname):
        try:
            os.makedirs(dirname)
        except OSError as e:
            if e.errno != errno.EEXIST: raise

def get_rand_list(bits, l):
    return [random.getrandbits(bits) for _ in range(l)]

def gen_mult3_input(n) :
    product = 1
    bits = n/3
    xs = get_rand_list(bits,3)
    ys = get_rand_list(bits,3)
    fx = open("data/mult3/%d.1.dat"%n, 'w')
    fy = open("data/mult3/%d.2.dat"%n, 'w')
    for i in range(3):
  	fx.write("%d\n"%xs[i])
 	fy.write("%d\n"%ys[i])
    fx.close()
    fy.close()
    print("expected value: %d"%reduce(mul, [x+y for x,y in zip(xs,ys)], 1))

def inta2s(ar):
    s = ""
    for i in ar:
	s+=str(i)
    return s
    
def gen_edit_dist(n):
    bits = 2
    xs = get_rand_list(bits, 5)
    ys = get_rand_list(bits, 5)
    fx = open("data/edit_dist/%d.1.dat"%n, 'w')
    fy = open("data/edit_dist/%d.2.dat"%n, 'w')
    sum = 0
    for i in range(5):
	sum += 0 if xs[i]==ys[i] else 1
	fx.write("%d\n"%xs[i])
	fy.write("%d\n"%ys[i])
    fx.close()
    fy.close()
    print("String 1:%s"%inta2s(xs))
    print("String 2:%s"%inta2s(ys))
    print("expected value: %d"%sum)

def gen_lookup(n,l):
    bits = n
    xs = get_rand_list(bits, l)
    xs_2 = get_rand_list(bits, l)
    c = random.randint(0, 1)
    y = 0
    ind = 0
    result = []
    if c == 0:
	print("Gen not in list")
	y = random.getrandbits(bits)
    else:
	print("Gen in list")
	ind = random.randint(0, l-1)
	y = xs[ind]
    fx = open("data/lookup/%d.1.dat"%n, 'w')
    fx_2 = open("data/lookup/%d.1_2.dat"%n, 'w')
    fy = open("data/lookup/%d.2.dat"%n, 'w')
    fy.write("%d\n"%y)
    for i in range(l):
        fx.write("%d\n"%xs[i])
        fx_2.write("%d\n"%xs_2[i])
    for i in range(l):
	if xs[i]==y:
	    result.append(xs_2[i])
    print("List Length: " + str(l))
    print("Lookup Results: \n")
    print(result)


def gen_input(program, n, l):
    bits = (n - int(math.log(l, 2))) / 2
    print bits
    lists = [(i,get_rand_list(bits,l)) for i in [1,2]]
    for party,data in lists:
        with open("data/%s/%d.%s.dat"%(program,n,party),'w') as f:
            for x in data:
                f.write("%d\n"%x)
    print("expected value: %d"%(sum(x*y for x,y in zip(lists[0][1], lists[1][1]))))

def gen_xtabs_input(n, l):
    LEN = l
    IDMAX = min(2 * LEN, 2**n)
    BINS = 5

    with open("data/xtabs/%d.bins.dat"%n,'w') as f:
        xs = [(random.randint(0,IDMAX), random.randint(0,BINS-1)) for _ in range(LEN)]
        for idx, binx in xs:
            x = "%d %d\n"%(idx, binx)
            f.write(x)

    with open("data/xtabs/%d.vals.dat"%n,'w') as f:
        ys = [(random.randint(0,IDMAX), 
               random.getrandbits(n/int(math.log(l,2)))) for _ in range(LEN)]
        for idy, val in ys:
            y = "%d %d\n"%(idy,val)
            f.write(y)

    binsums = [0] * BINS
    for idx, binx in xs:
        for idy, val in ys:
            if idx == idy:
                binsums[binx] += val

    print("expected value: ", binsums)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='generates input for emp-toolkit sample programs')
    parser.add_argument('-n', default=32, type=int, 
        help="integer bit length")
    parser.add_argument('-l', default=10, type=int, 
        help="array length (for innerprod, xtabs, and lookup)")
    programs = ["lookup", "edit_dist","mult3","innerprod","xtabs"]
    parser.add_argument('-e', default="xtabs", choices = programs,
        help="program selection")
    args = parser.parse_args()

    create_dirs(args.e)

    if args.e == "mult3":
        gen_mult3_input(args.n)

    elif args.e == "innerprod":
        gen_input(args.e, args.n, args.l)

    elif args.e == "xtabs":
        gen_xtabs_input(args.n, args.l)
    elif args.e == "edit_dist":
	gen_edit_dist(args.n) 
    elif args.e == "lookup":
	gen_lookup(args.n, args.l)

