__author__ = 'matthias'

from services.positions import *

pos = Positions()
pos.load('./config.csv')


pos.plotMovement()

for i in range(len(pos)):
    pos.printStep(i)

