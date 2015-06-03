__author__ = 'matthias'

import serial

com = serial.Serial("/dev/pts/27")

msg = com.readline()
com.write("guguguggg")
msg = com.readline()
com.write("10")

print msg