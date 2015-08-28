__author__ = 'matthias'
import numpy as np
import matplotlib.pyplot as plt

from base.base import *
from matplotlib.collections import LineCollection
# Collection of functions to read movement pattern configuration, the file looks like this
class Positions(Base):
    def __init__(self):
        self.name = "configuration"
        super(Positions, self).__init__(name=self.name, logit=True, RunNumber=1)

    def __len__(self):
        return np.size(self.data, axis=0)

    def load(self, filename):
        self.data = np.genfromtxt(filename, comments="#", delimiter=", ", dtype=int)
        self.len = np.size(self.data, axis=0)


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

    def printStep(self, i):
        self.printMsg("Scanning info for step " + str(i + 1) + "/" + str(self.len))
        self.printMsg(" - Horizontal Mirror Movement: steps: " + str(self.getHorRelativeMovement(i)) + " speed: " + str(
            self.getHorSpeed(i)))
        self.printMsg(" - Vertical Mirror Movement:   steps: " + str(self.getVerRelativeMovement(i)) + " speed: " + str(
            self.getVerSpeed(i)))
        self.printMsg(" - Laser Shot Frequency: " + str(self.getShotFreq(i)) + "Hz")
        self.printMsg(" - Attenuator Movement:  " + str(self.getAttenuator(i)))
        self.printMsg(" - Aperture Movement:    " + str(self.getAperture(i)))

    def plotMovement(self):
        x = np.zeros(len(self.data) + 1)
        y = np.zeros(len(self.data) + 1)

        x[1:] = np.cumsum(self.data[:, 1])
        y[1:] = np.cumsum(self.data[:, 2])

        lwidths = self.data[:, 7] / 10000.
        print lwidths
        points = np.array([x, y]).T.reshape(-1, 1, 2)
        segments = np.concatenate([points[:-1], points[1:]], axis=1)
        lc = LineCollection(segments, linewidths=lwidths, colors='blue')

        print x, y
        fig, a = plt.subplots()
        a.add_collection(lc)
        a.set_xlim(0, 200000)
        a.set_ylim(0, 10000)
        plt.show()









