__author__ = 'matthias'


from data import *

data = LaserData()

# change status
data.status = 100000000

# pack data
packed = data.pack()
print packed

# get unpacked data and fill the data
unpacked = data.unpack(packed)
print unpacked
data.fill(unpacked)

# check it changed
print data

data.writeTxt()
data.writeBinary("test_laserdata.bin")
