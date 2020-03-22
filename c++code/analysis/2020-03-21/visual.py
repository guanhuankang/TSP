import matplotlib.pyplot as plt
import os

def show(filename):		
	vals = []
	sol = []
	opt = []
	x = []
	y = []
	title = ""
	with open(filename, "r") as f:
		title = f.readline()
		x = f.readline()
		x = list(map(float,x.split()))
		y = f.readline()
		y = list(map(float,y.split()))
		vals = f.readline()
		vals = list( map(float, vals.split()) )
		sol = f.readline()
		sol = list( map(int, sol.split()) )
		opt = f.readline()
		opt = list( map(int, opt.split()) )

	sol.append(sol[0])
	opt.append(opt[0])

	plt.subplot(221)
	plt.scatter(x,y)
	# plt.plot( [ x[i-1] for i in sol], [ y[i-1] for i in sol], c = 'r', label = "solution" )
	# plt.plot( [ x[i-1] for i in opt], [ y[i-1] for i in opt], c = 'g', label = "optimal" )

	plt.subplot(223)
	plt.scatter(x,y)
	plt.plot( [ x[i-1] for i in sol], [ y[i-1] for i in sol], c = 'r', label = "solution" )
	plt.subplot(224)
	plt.scatter(x,y)
	plt.plot( [ x[i-1] for i in opt], [ y[i-1] for i in opt], c = 'g', label = "optimal" )

	plt.subplot(222)
	plt.plot( [_ for _ in range(len(vals))], vals )

	plt.title(title+filename)
	plt.savefig(filename.replace(".txt",".png"))
	print(filename.replace(".txt",".png"))
	plt.clf()
	# plt.show()
	pass

def showvals(filename):
	with open(filename) as f:
		ran = list(map(float, f.readline().split()))
		pso1 = list(map(float, f.readline().split()))
		pso2 = list(map(float, f.readline().split()))
		ga = list(map(float, f.readline().split()))

	plt.clf()
	plt.plot( [_ for _ in range(len(ran))], ran, label = "random" )
	plt.plot( [_ for _ in range(len(pso1))], pso1, label = "pso-v1" )
	plt.plot( [_ for _ in range(len(pso2))], pso2, label = "pso-v2" )
	plt.plot( [_ for _ in range(len(ga))], ga, label = "GA" )

	plt.xlabel("generations")
	plt.ylabel("cost")

	plt.legend()

	plt.show()

for i in os.listdir():
	# if ".val" == i[-4::]:showvals(i)
	if ".txt" == i[-4::]:show(i)