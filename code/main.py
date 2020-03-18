import math,time
from iohandle import IOHandle
from pso import PSO
from particle import Particle

def write(dic):
    with open("result/"+str(int(time.time())) + dic["label"],"w") as f:
        f.write("solution:"+dic["Best"].__str__()+"\n")
        cnt = 1
        for i in dic["his"]:
            f.write( str(cnt)+":"+str(i)+"\n" )
            cnt += 1

dataset = "a280"

ioh = IOHandle()
tspdata = ioh.readTsp("../dataset/%s.tsp"%dataset)
tspdataopt = ioh.readTour("../dataset/%s.opt.tour"%dataset)
# tspdata = ioh.readTsp("../dataset/tspdata.tsp")
# tspdataopt = ioh.readTour("../dataset/tspdata.opt.tour")

cityNum = int( tspdata["DIMENSION"] )
generations = 500

# pointSwap = PSO(n = 2*cityNum, length = cityNum, pgraph = tspdata, ppossibility = 1/cityNum, gpossibility = 1/cityNum)
# p = pointSwap.runPointSwap(generations)

rando = PSO(n = 2*cityNum, length = cityNum, pgraph = tspdata, ppossibility = 1/cityNum, gpossibility = 1/cityNum)
r = rando.runRandom(generations)

ga = PSO(n = 2*cityNum, length = cityNum, pgraph = tspdata, ppossibility = 1/cityNum, gpossibility = 0.5)
g = ga.runGA(generations)


opt = Particle(tspdataopt["data"][:-1])
print("Opt Cost:", ga.fitness(opt) )

rando.show( [p, r, g] )

write(p)
write(r)
write(g)