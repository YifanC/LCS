
from FeedthroughMotorControls import *
from LaserControl import *
import os
import sys
import subprocess
import time
import signal

COMLaser = 3
COMMotorControl = 10

initFeedtrough(COMMotorControl)
initAxis1()

for i in range(10):
	SetHomeSwitch(1,0)
	time.sleep(1)
	SetLimitSwitches(1,0)
	time.sleep(1)


