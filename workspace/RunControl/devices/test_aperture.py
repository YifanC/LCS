__author__ = 'matthias'

from devices.aperture import *
DEBUG = False
ap = Aperture()
ap.color = False       # just for the use with bpython
ap.comEcho = False

# set com port
#ap.comport = "/dev/ttyUSB7"
ap.com_init()
ap.checkName()
ap.enableMotor()    # there is an echo comming back
# check if there is a echo from set commands!
# check if attenuator still works!

# test to do: (in this order)
ap.init()

ap.disableMotor()
