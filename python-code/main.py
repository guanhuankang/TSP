import math,time
from iohandle import IOHandle
from pso import PSO
from particle import Particle


dataset = "a280"

def write(dic):
    with open("result/"+str(int(time.time())) + dic["label"] + dataset,"w") as f:
        f.write("solution:"+dic["Best"].__str__()+"\n")
        cnt = 1
        for i in dic["his"]:
            f.write( str(cnt)+":"+str(i)+"\n" )
            cnt += 1

ioh = IOHandle()
tspdata = ioh.readTsp("../dataset/%s.tsp"%dataset)
tspdataopt = ioh.readTour("../dataset/%s.opt.tour"%dataset)
# tspdata = ioh.readTsp("../dataset/tspdata.tsp")
# tspdataopt = ioh.readTour("../dataset/tspdata.opt.tour")

cityNum = int( tspdata["DIMENSION"] )
generations = 2000

pointSwap = PSO(n = 2*cityNum, length = cityNum, pgraph = tspdata, ppossibility = 1/cityNum, gpossibility = 1/cityNum)
p = pointSwap.runPointSwap(generations)

rando = PSO(n = 2*cityNum, length = cityNum, pgraph = tspdata, ppossibility = 1/cityNum, gpossibility = 1/cityNum)
r = rando.runRandom(generations)

ga = PSO(n = 2*cityNum, length = cityNum, pgraph = tspdata, ppossibility = 1/cityNum, gpossibility = 0.5)
g = ga.runGA(generations)


opt = Particle(tspdataopt["data"][:-1])
print("Opt Cost:", ga.fitness(opt) )
print("g,r,p:",ga.fitness(g["Best"]),ga.fitness(r["Best"]),ga.fitness(p["Best"]) )

write(p)
write(r)
write(g)


rando.show( [g,r,p] )
