# random test

from RCCommunication import *
import time

data = LaserData(1)
data.laserid = 1
print str(data)


test = [1, 1000.0,1000.0, 1000, 1000, 365.23, 12, 12, 120009, 100, 200, 300, 400]
test1 = bytearray(test)