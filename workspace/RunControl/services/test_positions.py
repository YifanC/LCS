__author__ = 'matthias'

from services.positions import *

pos = Positions(123)
pos.load('./scans/config_scan.csv')

pos.plotMovement()


