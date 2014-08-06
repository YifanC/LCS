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


Move = int(sys.argv[1])

initFeedtrough()
initAxis1()
#getInfo(1)




pos1 = getPosition(1)
print "Pos 1 " + pos1
moveHorizontal(Move)
while AxisMoving(1):
  print "moving"
  time.sleep(2)

pos2 = getPosition(1)
print "Pos 2 " + pos2

print int(pos1)-int(pos2)

SetParameter(1, "HC", 0)
SetParameter(2, "HC", 0)

