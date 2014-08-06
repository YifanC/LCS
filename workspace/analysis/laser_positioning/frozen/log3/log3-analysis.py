
from pylab import *

read_data1 = loadtxt("loglaser3.txt",delimiter=",")
read_data2 = loadtxt("loglaser4.txt",delimiter=",")

#plot(read_data2[:,1],'ro')

startpoints1 = [0,449,898,1347,1797,2246]
startpoints2 = [0,439,878,1317,1756,2195,2635,3075,3514,4330,4769,5209]

figure(1)
for i in range(5):
    plot(read_data2[startpoints2[i]:startpoints2[i]+448,1],'go')
    plot(read_data2[startpoints[i]:startpoints[i]+448,1],'go')
show()


#figure(2)
#deltaT = read_data2[1:size(read_data2[:,1]),0] - read_data2[0:size(read_data2[:,1])-1,0]
#plot(deltaT,'go')
#show()