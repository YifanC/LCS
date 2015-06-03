__author__ = 'matthias'

from devices.aperture import *

ap = Aperture()
ap.color = False       # just for the use with bpython
ap.comport = "/dev/ttyUSB7"
ap.com_init()
print ap.getParameter("acceleration")
