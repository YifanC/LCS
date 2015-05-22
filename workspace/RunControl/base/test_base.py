__author__ = 'matthias'

from attenuator import *

motor1 = Motor(18)

motor1.name = "motor1"
motor1.init_com()

motor1.com_write("test213412423112")
test = motor1.com_recv(800)

print "answer: ", test
motor1.close_com()

