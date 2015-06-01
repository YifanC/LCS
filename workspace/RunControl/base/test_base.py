__author__ = 'matthias'

from base import *

motor1 = ComSerial("/dev/pts/18")

motor1.name = "motor1"
motor1.comEnd = ""
motor1.com_init()

motor1.InfoInstruction = "ll"
motor1.getInfo(True)
motor1.com_write("test213412423112")
test = motor1.com_recv(800)

motor1.com_close()

