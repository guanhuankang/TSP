import matplotlib.pyplot as plt
import math,os
import numpy as np

r = []
pso1 = []
pso2 = []
ga = []
name = []
## val
def multiMethods1fig(filename, xid):
    _r = []
    _pso1 = []
    _pso2 = []
    _ga = []
    name.append(xid)
    with open(filename,"r") as f:
        _r = list(map(float,f.readline().split()))
        _pso1 = list(map(float,f.readline().split()))
        _pso2 = list(map(float,f.readline().split()))
        _ga = list(map(float,f.readline().split()))
    
    r.append(_r)
    pso1.append(_pso1)
    pso2.append(_pso2)
    ga.append(_ga)

    plt.clf()
    plt.title(xid)
    plt.plot( [_ for _ in range(len(r))], r, label = "random" )
    plt.plot( [_ for _ in range(len(pso1))], pso1, label = "pso-v1" )
    plt.plot( [_ for _ in range(len(pso2))], pso2, label = "pso-v2" )
    plt.plot( [_ for _ in range(len(ga))], ga, label = "GA" )

    plt.xlabel("generations")
    plt.ylabel("cost")

    plt.legend()
    plt.savefig("images/"+xid+".png")

# L = os.listdir("analysis")
# for i in L:
#     if "val" not in i:continue;
#     print(i)
#     multiMethods1fig("analysis/"+i, i[0:-4])

## result - i sol
def solve(filename, linenum, func = int):
    sol = []
    with open("result/"+filename, "r") as f:
        for i in range(linenum):
            sol = f.readline()
    return list(map(func, sol.split()))


dic = {}
L = os.listdir("result")
for i in L:
    t = (i.split("_")[1]).split("-")
    if t[1] not in dic:
        dic[t[1]] = {}

    tmp = solve(i, 5)
    dic[t[1]][t[0]] = np.array(tmp+[tmp[0]]).astype(int)

    tmp = solve(i, 6)
    if len(tmp)>0:
        dic[t[1]]["opt"] = np.array(tmp+[tmp[0]]).astype(int)
    else:
        dic[t[1]]["opt"] = np.array(tmp).astype(int)
    dic[t[1]]["map"] = [solve(i, 2, float),solve(i,3,float)]

print(dic.keys())

def show(name):
    plt.clf()
    x = np.array(dic[name]["map"][0])
    y = np.array(dic[name]["map"][1])

    plt.subplot(321)
    plt.scatter(x, y, s = 2, c='r')
    plt.title("Map-"+name)

    plt.subplot(322)
    plt.scatter(x, y, s = 2, c = 'r')
    plt.plot( x[dic[name]["opt"]-1], y[dic[name]["opt"]-1])
    plt.title("Opt-"+name)

    plt.subplot(323)
    plt.scatter(x, y, s = 2, c = 'r')
    plt.plot( x[dic[name]["Random"]-1], y[dic[name]["Random"]-1])
    plt.title("Random-"+name)

    plt.subplot(324)
    plt.scatter(x, y, s = 2, c = 'r')
    plt.plot( x[dic[name]["pso1"]-1], y[dic[name]["pso1"]-1])
    plt.title("pso1-"+name)

    plt.subplot(325)
    plt.scatter(x, y, s = 2, c = 'r')
    plt.plot( x[dic[name]["pso2"]-1], y[dic[name]["pso2"]-1])
    plt.title("pso2-"+name)

    plt.subplot(326)
    plt.scatter(x, y, s = 2, c = 'r')
    plt.plot( x[dic[name]["GA"]-1], y[dic[name]["GA"]-1])
    plt.title("GA-"+name)

    plt.savefig("images/"+name+".png")
    # plt.show()
    
for k in dic:
    show(k)