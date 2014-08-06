
from pylab import *

read_data2 = loadtxt("log2.txt",delimiter=",")
read_data3 = loadtxt("log3.txt",delimiter=",")
read_data4 = loadtxt("log4",delimiter=",")

plot(read_data2[:,1],'go')
plot(read_data3[:,1],'bo')
plot(read_data4[:,1],'ro')
show()