import random, copy

class Particle:
	def __init__(self, n=1):
		if type(n)==int:
			self.length = n;
			self.setRandomSeq(n);
			self.pbest = copy.deepcopy(self)
		elif type(n)==list:
			self.length = len(n)
			self.seq = n.copy()
			self.pbest = copy.deepcopy(self)
		else:
			raise Exception("Construction fail in Particle")

	def copy(self):
		return copy.deepcopy(self)

	def setRandomSeq(self, n = 0):## fix the first city eq 1
		n = max(n, self.length)
		self.seq = [i for i in range(1,n+1)]
		for i in range(1,n): ## shuffle algorithm
			self.swap(i, random.randint(i,n-1), 1);

	def getSeq(self):
		return self.seq.copy()

	def getHash(self):
		return "-".join( list( map(str,self.seq) ) )

	def setPBest(self, pbest):
		self.pbest = copy.deepcopy(pbest)

	def indexOf(self, city):
		for i in range(self.length):
			if city==self.seq[i]:return i;
		return -1;

	def evol(self, target, possibility):
		if target.length!=self.length: return self
		for i in range(self.length):
			self.swap(i, target.indexOf(self.seq[i]), possibility)
		return self

	def swap(self, a, b, p=1):
		if random.random()<=p:
			self.seq[a], self.seq[b] = self.seq[b], self.seq[a]

	def __str__(self):
		return "-".join( list(map(str,self.seq)) )

	def dist(self, target):## Hanming distance
		if target.length!=self.length:return max(target.length, self.length)
		dis = 0
		for i in range(self.length):
			dis = dis + (0 if self.seq[i]==target.seq[i] else 1)
		return dis
	
	def crossover(self, target, p):
		a, b = self.seq[p[0]:p[1]+1], target.seq[p[0]:p[1]+1]
		A = self.copy()
		B = target.copy()

		Aa = []
		for i in A.seq:
			if i not in b:
				Aa.append(i)
		Aa = Aa + b

		Bb = []
		for i in B.seq:
			if i not in a:
				Bb.append(i)
		Bb = Bb + a

		self.seq = Aa
		target.seq = Bb

# n = 5
# p1 = Particle(n)
# p2 = Particle(n)
# print(p1,p2)
# p3 = Particle([1,2,4,5,6,3])
# print(p3)
# for i in range(100):
# 	print(p1.dist(p2), p1, p2)
# 	p1.evol(p2, 1/n)
# 	pass