
from pylab import *
dataset = 15
read_data = loadtxt("log"+str(dataset)+"/logMovement"+str(dataset)+".txt",delimiter=",")

plot((read_data[:,0] - read_data[0,0])/1000 ,read_data[:,1],'go')

show()