
from pylab import *

read_data2 = loadtxt("logMovement1.txt",delimiter=",")

plot(read_data2[:,1],'go')

show()