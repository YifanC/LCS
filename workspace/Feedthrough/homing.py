# simple script to monitor Inputs 1 to 4 on the feedthrough Motor controller. 
# Start as python Monitoring.py X, where X is the axis you would like to monitor
# Axis 1 = Rotary
# Axis 2 = Linear
#End Switches:
# - Liear Axis: I1 = Upper Limit, I2 = Lower Limit
# - Rotary Axis: I1 = 
from FeedthroughMotorControls import *
import os
import sys
LaserCOM = 10

initFeedtrough(LaserCOM)
initAxis1()
getInfo(1)
HomeAxis(1)
