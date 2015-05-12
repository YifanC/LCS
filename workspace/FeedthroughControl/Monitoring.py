# simple script to monitor Inputs 1 to 4 on the feedthrough Motor controller. 
# Start as python Monitoring.py COMPort X, where X is the axis you would like to monitor
# Axis 1 = Rotary
# Axis 2 = Linear
#End Switches:
# - Liear Axis: I1 = Upper Limit, I2 = Lower Limit
# - Rotary Axis: I1 = 

from FeedthroughMotorControls import *
import os
import sys
COMLaser = int(sys.argv[1])
MonitorAxis = int(sys.argv[2])

if (MonitorAxis > 2 or MonitorAxis < 1):
  print "Axis is out of range"
  sys.exit()

initFeedtrough(COMLaser)

while (1):
  try:
    os.system('clear')
    MonitorParameter(MonitorAxis,"I1")
    MonitorParameter(MonitorAxis,"I2")
    MonitorParameter(MonitorAxis,"I3")
    MonitorParameter(MonitorAxis,"I4")
    
    time.sleep(0.5)
  except KeyboardInterrupt:
    print "Bye"
    sys.exit()

