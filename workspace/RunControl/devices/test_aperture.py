__author__ = 'matthias'

from devices.aperture import *
DEBUG = False
ap = Aperture()
ap.color = False       # just for the use with bpython
ap.comEcho = False
ap.comport = "/dev/ttyUSB7"
ap.com_init()
ap.checkName()
ap.enableMotor()    # there is an echo comming back
# check if there is a echo from set commands!
# check if attenuator still works!

ap.home()
ap.moveRelative(50,display=True)

i = 1
while int(ap.getParameter("limit2")) is not 1:
	print i
	ap.moveRelative(-100,display=True)
	i = i + 1

print "total " + str(i)
#ap.disableMotor()
