__author__ = 'matthias'

import serial

com = serial.Serial("/dev/pts/14")

msg = com.readline()
com.write("guguguggg")

print msg