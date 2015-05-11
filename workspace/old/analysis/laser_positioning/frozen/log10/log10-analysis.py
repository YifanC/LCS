
from pylab import *

read_data2 = loadtxt("loglaser10.txt",delimiter=",")

figure(2)
plot(read_data2[:,1],'ro')

startpoints2 = [0,343,686,1028,1372,1715]

x =r_[0:391]

figure(1)
for i in range(4):
    pass#plot(read_data2[startpoints2[i]:startpoints2[i]+342,0]-read_data2[startpoints2[i],0],read_data2[startpoints2[i]:startpoints2[i]+342,1],'-o')
show()


#figure(2)
#deltaT = read_data2[1:size(read_data2[:,1]),0] - read_data2[0:size(read_data2[:,1])-1,0]
#plot(deltaT,'go')
#show()