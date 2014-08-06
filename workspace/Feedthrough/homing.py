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

initFeedtrough()

SetParameter(1,"VM",6000)
SetParameter(1,"S2","1,1,0")
SetParameter(1,"LM",1)
getInfo(1)
HomeAxis(1)
SetParameter(1,"S2","3,1,0")
SetParameter(1, "HC", 0)
SetParameter(2, "HC", 0)
