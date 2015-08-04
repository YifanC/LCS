__author__ = 'matthias'

from base.base import *
from base.controls import *
from services.communication import Producer
from services.data import LaserData

from devices.feedtrough import *
from devices.laser import *
from devices.attenuator import *
from devices.aperture import *
from devices.mirror import *

class RunControl(Base):
    def __init__(self, RunNumber):
        name = "run_control"
        super(RunControl, self).__init__(name=name)
        RunNumber = 99
        # Construct needed instances
        self.con = Controls(RunNumber=RunNumber)
        self.data = LaserData()
        self.com = Producer("runcontrol")
        self.ft_linear = Feedtrough("ft_linear", 1)
        self.ft_rotary = Feedtrough("ft_rotary", 1)
        self.laser = Laser()
        self.attenuator = Attenuator()
        self.aperture = Aperture()
        self.mirror_x = Mirror("mirror221")  # not yet defined correctly
        self.mirror_y = Mirror("mirror222")  # not yet defined correctly


    def load_positions(self, Filename):
        pass

    def next_step(self):
        pass