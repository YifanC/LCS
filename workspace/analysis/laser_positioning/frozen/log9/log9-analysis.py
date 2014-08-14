
from pylab import *

read_data2 = loadtxt("loglaser9.txt",delimiter=",")

figure(1)


N = len(read_data2)
t = (read_data2[:,0]-read_data2[0,0])/1000

plot(t,read_data2[:,1],'ro')

startpoints2 = [0,343,686,1028,1372,1715]

x =r_[0:391]



figure(2)
for i in range(4):
    plot(t[startpoints2[i]:startpoints2[i]+342]-t[startpoints2[i]],read_data2[startpoints2[i]:startpoints2[i]+342,1],'-o')
show()


figure(3)
plot(t[0:N-1]-t[1:N],'o')
show()
#figure(2)
#deltaT = read_data2[1:size(read_data2[:,1]),0] - read_data2[0:size(read_data2[:,1])-1,0]
#plot(deltaT,'go')
#show()