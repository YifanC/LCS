
from pylab import *

read_data2 = loadtxt("loglaser8.txt",delimiter=",")
#plot(read_data2[:,1],'ro')

startpoints2 = [0,391,783,1175]

x =r_[0:391]

figure(1)
for i in range(4):
    plot(read_data2[startpoints2[i]:startpoints2[i]+391,0]-read_data2[startpoints2[i],0],read_data2[startpoints2[i]:startpoints2[i]+391,1],'-o')
show()


#figure(2)
#deltaT = read_data2[1:size(read_data2[:,1]),0] - read_data2[0:size(read_data2[:,1])-1,0]
#plot(deltaT,'go')
#show()