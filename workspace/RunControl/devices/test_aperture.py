__author__ = 'matthias'

from devices.aperture import *

ap = Aperture()
ap.color = False       # just for the use with bpython
ap.comport = "/dev/ttyUSB7"
ap.com_init()
ap.checkName()
ap.enableMotor()    # there is an echo comming back
# check if there is a echo from set commands!
time.sleep(2)
# check if attenuator still works!
#ap.setParameter("setDirection",0)

ap.disableMotor()
