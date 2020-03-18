class IOHandle:
	def __init__(self):
		pass

	def readTsp(self, filename):
		self.dic = {}
		with open(filename, "r") as f:
			for line in f.readlines():
				if ":" in line:
					L = line.split(":")
					self.dic[L[0].strip()] = L[1].strip()
				elif line.strip() == "NODE_COORD_SECTION":
					self.dic["data"] = []
				elif line.strip() == "EOF":
					break;
				else: ## data
					self.dic["data"].append( list(map(int,line.split())) )
		return self.dic

	def readTour(self, filename):
		self.dic = {}
		with open(filename, "r") as f:
			for line in f.readlines():
				if ":" in line:
					L = line.split(":")
					self.dic[L[0].strip()] = L[1].strip()
				elif line.strip() == "TOUR_SECTION":
					self.dic["data"] = []
				elif line.strip() == "EOF":
					break;
				else: ## data
					self.dic["data"].append( int(line) )
		return self.dic
		pass

	def writeTour(self, tour, filename):
		pass

	def writeTsp(self, tsp, filename):
		pass

# reader = IOHandle()
# dic = reader.readTsp("../dataset/att48.tsp")
# print(dic)
# dic = reader.readTour("../dataset/att48.opt.tour")
# print(dic)