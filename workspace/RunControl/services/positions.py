__author__ = 'matthias'
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection
import datetime

from base.base import *
# Collection of functions to read movement pattern configuration, the file format is described below:
# N     Instruction Number:
#       is the step number and has no effect for the input
# HRM   Horizontal Relative Movement:
#       Horizontal movement in microsteps
# VRM   Vertical Relativ Movement:
#       Vertical movement in microsteps
# HS    Horizontal Speed:
#       Horizontal movement speed in microsteps/s
# VS    Vertical speed:
#       Vertical movement speed in microsteps/s
# S     Shot Frequency:
#       Shot frequency: - positive = shooting at a given frequency,
#                       - zero     = single shot,
#                       - negative = shooting after movement at a given frequency
# SC    Number of Shots:
#       Shot count, only relevant if a negative shot frequency is selected. Used to determine the time to hold during
#       the shooting of the laser. To do a move without shooting, select a negative frequency and set this value to 0.
# AT    Attenuator Positions:
#       Absolute attenuator position
# AP    Aperture Pos:
#       Absolute aperture position (not implemented yet)
#
# An example file would look like this, see also comments for each step
#N      HRM,   VRM,   HS,    VS, S, SC,   AT, AP
#0,   80000, 	0, 10000, 10000, 1, 0, 8000, -1     # shooting at 1 Hz while moving
#1,  200000, 	0, 10000, 10000, 1, 0, 8000, -1     # shooting at 1 Hz while moving
#2, -200000, 	0, 10000, 10000, -2, 10, 8000, -1   # move relative and after stop, shoot 10 shot at 2Hz
#3,  -50000, 	0, 10000, 10000, 0, 0, 8000, -1     # move relative and fire a single shot
#4,       0,  1000, 1000, 10000, -1, 20, 8000, -1   # move relative and after stop fire 20 shots at 1Hz
#5,  100000,     0, 1000, 10000, -1, 0, 8000, -1    # move relative without any shots
class Positions(Base):
    def __init__(self, RunNumber):
        self.name = "configuration"
        super(Positions, self).__init__(name=self.name, logit=True, RunNumber=RunNumber)

        self.len = 0
        self.scanning_time = datetime.timedelta(seconds=0)

    def __len__(self):
        return np.size(self.data, axis=0)

    def load(self, filename):
        self.data = np.genfromtxt(filename, comments="#", delimiter=", ", dtype=int)

        self.len = np.size(self.data, axis=0)
        self.calc_scanning_time()


    def getHorRelativeMovement(self, i):
        return self.data[i, 1]

    def getVerRelativeMovement(self, i):
        return self.data[i, 2]

    def getHorSpeed(self, i):
        return self.data[i, 3]

    def getVerSpeed(self, i):
        return self.data[i, 4]

    def getAttenuator(self, i):
        return self.data[i, 7]

    def getAperture(self, i):
        return self.data[i, 8]

    def getShotFreq(self, i):
        return self.data[i, 5]

    def getShotCount(self, i):
        if self.getShotFreq(i) < 0:
            return self.data[i, 6]
        else:
            return False

    def getEntry(self, i):
        return self.data[i, :]

    def time_step(self,i):
        "calculactes time needed for step i"
        if self.getHorSpeed(i) != 0:
            time_horizontal = abs(float(self.getHorRelativeMovement(i)) / self.getHorSpeed(i))
        else:
            time_horizontal = 0
        if self.getVerSpeed(i) != 0:
            time_vertical = abs(float(self.getVerRelativeMovement(i)) / self.getVerSpeed(i))
        else:
            time_vertical = 0

        return datetime.timedelta(seconds=time_horizontal) + datetime.timedelta(seconds=time_vertical)

    def calc_scanning_time(self):
        dt = datetime.timedelta(0)
        for i in range(self.len):
            dt += self.time_step(i)

        self.scanning_time = dt


    def printStep(self, i):
        self.printMsg("Scanning info for step " + str(i + 1) + "/" + str(self.len))
        self.printMsg(" - This step will take: " + str(self.time_step(i)))
        self.printMsg(" - Horizontal Mirror Movement: steps: " + str(self.getHorRelativeMovement(i)) + " speed: " + str(
            self.getHorSpeed(i)))
        self.printMsg(" - Vertical Mirror Movement:   steps: " + str(self.getVerRelativeMovement(i)) + " speed: " + str(
            self.getVerSpeed(i)))
        self.printMsg(" - Laser Shot Frequency: " + str(self.getShotFreq(i)) + "Hz")
        self.printMsg(" - Attenuator Movement:  " + str(self.getAttenuator(i)))
        self.printMsg(" - Aperture Movement:    " + str(self.getAperture(i)))


    def plotMovement(self):
        x = np.zeros(self.len + 1)
        y = np.zeros(self.len + 1)

        time_step_x = np.zeros(self.len + 1)
        time_step_y = time_step_x
        total_time =0

        print self.scanning_time

        x[1:] = np.cumsum(self.data[:, 1])
        y[1:] = np.cumsum(self.data[:, 2])

        lwidths = self.data[:, 7] / 10000.
        points = np.array([x, y]).T.reshape(-1, 1, 2)
        segments = np.concatenate([points[:-1], points[1:]], axis=1)
        lc = LineCollection(segments, linewidths=lwidths, colors='blue')

        print x, y
        fig, a = plt.subplots()
        a.add_collection(lc)
        a.set_xlabel("horizontal steps [microsteps]")
        a.set_ylabel("vertical steps [microsteps]")
        a.set_title("scanning pattern")
        a.set_xlim(-10000, 2500000)
        a.set_ylim(-1100000, 1100000)
        plt.show()









