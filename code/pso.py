import math,random
import matplotlib.pyplot as plt
import tqdm 

from particle import Particle


class PSO:
	def __init__(self, n, length, pgraph, ppossibility, gpossibility):
		self.n = n
		self.length = length
		self.particles =  []
		self.generations = 0
		self.p2pDist = self.calcP2pDist(pgraph)
		self.ppossibility = ppossibility ## personal swap possibility
		self.gpossibility = gpossibility ## global swap possibility

		## insert particles
		for i in range(self.n):
			self.particles.append( Particle(self.length) )

		## set pbest and gbests
		# pbest will be set when instancing
		self.gbest = self.findBest(self.particles)

		## refer function "def run()"" to view the termination condition

	def isValidPath(self, particle):
		seq = particle.getSeq()
		n = particle.length
		if len(seq)!=n:return False
		seq.sort()
		for i in range(n):
			if seq[i]!=i+1:return False
		return True

	def calcP2pDist(self, pgraph):
		cityNum = int(pgraph["DIMENSION"])
		citys = pgraph["data"]

		dist = {}
		for i in range(cityNum):
			for j in range(cityNum):
				dist[ (i+1,j+1) ] = math.sqrt( (citys[i][1]-citys[j][1])**2 + (citys[i][2]-citys[j][2])**2 )

		return dist

	def fitness(self, particle):## const particle
		seq = particle.getSeq()
		length = len(seq)
		seq.append(seq[0])
		cost = 0
		for i in range(length):
			cost = cost + self.p2pDist[ (seq[i], seq[(i+1)]) ]
		return cost

	def findBest(self, particles):## the particle with min fitness will be returned
		n = len(particles) - 1
		fits = [ self.fitness(particles[i]) for i in range(n) ]
		kmin = 0
		for i in range(n):
			if fits[i]<fits[kmin]:
				kmin = i
		return particles[kmin].copy()

	def step(self):
		self.generations = self.generations + 1

		for p in self.particles:
			p.evol(p.pbest, self.ppossibility)
			p.evol(self.gbest, self.gpossibility)
			if self.fitness(p)<self.fitness(p.pbest):
				p.pbest = p.copy()

		self.gbest = self.findBest(self.particles+[self.gbest])

	def randomStep(self):
		self.generations = self.generations + 1

		self.particles = []
		for i in range(self.n):
			self.particles.append( Particle(self.length) )

		self.gbest = self.findBest(self.particles+[self.gbest])
	
	def GAStep(self):
		self.generations = self.generations + 1
		## shuffer alg
		for i in range(self.n):
			j = random.randint(i, self.n-1)
			tmp = self.particles[i]
			self.particles[i] = self.particles[j]
			self.particles[j] = tmp
		## match and crossover
		for i in range(0,self.n,2):
			self.particles.append(self.particles[i].copy())
			self.particles.append(self.particles[i+1].copy())

			if random.random()<=self.gpossibility:
				## double point crossover
				p = random.sample([_ for _ in range(0, self.length)], 2)
				self.particles[i].crossover(self.particles[i+1], p)
			else:
				## single point crossover
				p = random.sample([_ for _ in range(0, self.length)], 1)
				p = [p[0],self.length-1]
				self.particles[i].crossover(self.particles[i+1], p)

			## mutation
			if random.random()<=self.ppossibility:
				## mutation i
				p = random.sample([_ for _ in range(0, self.length)], 2)
				self.particles[i].swap(p[0],p[1])
			if random.random()<=self.ppossibility:
				## mutation i+1
				p = random.sample([_ for _ in range(0, self.length)], 2)
				self.particles[i+1].swap(p[0],p[1])
		
		## update self.gbest
		# self.n = len(self.particles)
		self.gbest = self.findBest(self.particles+[self.gbest])

		## choose
		fits = [ self.fitness(self.particles[i]) for i in range(self.n)  ]
		alpha = 0.0
		for i in fits:
			alpha = alpha + i
		alpha = alpha / self.n
		alpha2 = 2*alpha
		tmp = []

		# choose 1
		# while len(tmp)<self.n:
		# 	for i in range(self.n):
		# 		if random.random() > (fits[i]/alpha2):
		# 			tmp.append(self.particles[i].copy())
		# tmp.sort(key = lambda x:self.fitness(x))
		# self.particles = tmp[0:self.n]

		## choose 2
		self.particles.sort(key = lambda x:self.fitness(x))
		self.particles = self.particles[0:self.n]

	def runPointSwap(self, T = 500, label = "Point-Swap"):
		print(label)
		self.generations = 0
		his = []
		for i in tqdm.tqdm(range(T)):
			self.step()
			his.append( self.fitness(self.gbest) )
		return {"label":label,"Best":self.gbest,"his":his}

	def runRandom(self, T = 500, label = "Random"):
		print(label)
		self.generations = 0
		his = []
		for i in tqdm.tqdm(range(T)):
			self.randomStep()
			his.append( self.fitness(self.gbest) )
		return {"label":label,"Best":self.gbest,"his":his}

	def runGA(self, T = 500, label = "GA"):
		print(label)
		self.generations = 0
		his = []
		for i in tqdm.tqdm(range(T)):
			self.GAStep()
			his.append( self.fitness(self.gbest) )
		return {"label":label,"Best":self.gbest,"his":his}

	def show(self, hisList):
		title = ""
		for l in hisList:
			title = title + "&" + l["label"]
		plt.title(title)
		plt.xlabel("generations")
		plt.ylabel("cost")

		for his in hisList:
			plt.plot([i for i in range(len(his["his"]))], his["his"], label=his["label"])
		
		plt.legend()
		plt.show()
	