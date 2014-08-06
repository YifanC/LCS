
from pylab import *

read_data2 = loadtxt("loglaser6.txt",delimiter=",")
#plot(read_data2[:,1],'ro')

startpoints2 = [0,1126,2251,3376,4501,5226]

x =r_[0:716]
t0 = read_data2[716,0]

figure(1)
for i in range(5):
    plot(read_data2[startpoints2[i]:startpoints2[i]+716,0]-read_data2[startpoints2[i],0],read_data2[startpoints2[i]:startpoints2[i]+716,1],'-o')
show()


#figure(2)
#deltaT = read_data2[1:size(read_data2[:,1]),0] - read_data2[0:size(read_data2[:,1])-1,0]
#plot(deltaT,'go')
#show()