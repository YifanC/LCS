
from datetime import datetime
import argparse

from base.controls import *
from services.communication import Producer
from services.data import LaserData
from services.positions import *

from devices.feedtrough import *
from devices.laser import *
from devices.attenuator import *
from devices.aperture import *
from devices.mirror import *

# Ask for the runnumber
parser = argparse.ArgumentParser(description='Script to control the UV laser system')

parser.add_argument("-r", "--runnumber", action='store', dest='RunNumber', required=True,
                    help='current run number issued by DAQ', type=int)

arguments = parser.parse_args()


# ----------------------------------------------------
# ----------------------- Init -----------------------
# ----------------------------------------------------

# Construct needed instances
rc = Controls(RunNumber=arguments.RunNumber)
data = LaserData(RunNumber=arguments.RunNumber)
pos = Positions()
rc.com = Producer("runcontrol")
rc.ft_linear = Feedtrough("linear_actuator")
rc.ft_rotary = Feedtrough("rotary_actuator")
rc.laser = Laser()
rc.attenuator = Attenuator()
#rc.aperture = Aperture()
#rc.mirror_x = Mirror("mirror221", 1)  # not yet defined correctly
#rc.mirror_y = Mirror("mirror222", 2)  # not yet defined correctly

# Start broker / encoder
rc.broker_start()
print rc.RunNumber
rc.assembler_start(senddata=False)
time.sleep(2)

# Load Positions from file
pos.load("./services/config.csv")

# Dry run configuration
rc.encoder_start(dry=True)
rc.laser.comDryRun = True
rc.attenuator.comDryRun = True
rc.ft_rotary.comDryRun = False
rc.ft_linear.comDryRun = True

# Initialize com ports
rc.ft_rotary.com_init()
rc.ft_linear.com = rc.ft_rotary.com
rc.laser.com_init()
rc.attenuator.com_init()
#rc.aperture.com_init()
#rc.mirror_x.com_init()
#rc.mirror_y.com = rc.mirror_x.com

rc.laser.timeout = 20  # minutes
starttime = datetime.today()

initialized = False
rc.laser.start()
while (datetime.today() - starttime).seconds < 5: # int(60 * rc.laser.timeout):
    rc.laser.printMsg("waiting for laser to warm up (20 minutes)")
    if initialized is False:
        # Initialize and setup communication
        rc.com.id = 1
        rc.com.state = -1
        rc.com.start()
        rc.com.send_hello()
        rc.com.state = 0

        # Homing Feedtrough
        rc.ft_linear.homeAxis()
        rc.ft_rotary.homeAxis()

        # homing Attenuator
        initialized = True

    time.sleep(1)

rc.laser.setRate(0)
rc.laser.closeShutter()

raw_input("Start Laser Scan?")
# ----------------------------------------------------
# --------------------- Operation --------------------
# ----------------------------------------------------
rc.com.send_data(data)

for scanstep in range(len(pos)):
    pos.printStep(scanstep)
    # Set scanning speeds of axis
    rc.ft_rotary.setParameter("VM", pos.getHorSpeed(scanstep))
    rc.ft_linear.setParameter("VM", pos.getVerSpeed(scanstep))

    # Set Attenuator Position
    rc.attenuator.moveAbsolute(pos.getAttenuator(scanstep))

    # Configure the Laser to shoot at a given frequency while moving
    if pos.getShotFreq(scanstep) > 0:
        rc.printMsg("Moving and shooting @ " + str(pos.getShotFreq(scanstep)) + "Hz")
        rc.laser.setRate(pos.getShotFreq(scanstep))
        rc.laser.openShutter()
    else:
        rc.laser.setRate(0)

    # Do the actual movement and monitor it
    rc.ft_rotary.moveRelative(pos.getHorRelativeMovement(scanstep),monitor=True)
    rc.ft_linear.moveRelative(pos.getVerRelativeMovement(scanstep),monitor=True)
    rc.laser.closeShutter()

    # Sigle shot if frequency is 0
    if pos.getShotFreq(scanstep) == 0:
        rc.printMsg("Firing a single shot")
        rc.laser.openShutter()
        rc.laser.singleShot()
        rc.laser.closeShutter()

    # Shoot at a position a number of shots at a given rate,
    elif pos.getShotFreq(scanstep) < 0:
        if pos.getShotCount(scanstep) > 0:
            rc.printMsg("Shooting at given position @ " + str(-pos.getShotFreq(scanstep)) + "Hz for " +
                        str(pos.getShotCount(scanstep)/float(abs(pos.getShotFreq(scanstep)))) + " seconds")
            rc.laser.setRate(abs(pos.getShotFreq(scanstep)))
            rc.laser.openShutter()
            time.sleep(pos.getShotCount(scanstep)/float(abs(pos.getShotFreq(scanstep))))
            rc.laser.closeShutter()
        else:
            rc.printMsg("No laser shots in this step")

rc.assembler_alive()
rc.broker_alive()
rc.encoder_alive()

# ----------------------------------------------------
# -------------------- Finalize ----------------------
# ----------------------------------------------------


rc.laser.closeShutter()
rc.laser.setRate(0)
rc.laser.stop()
# Close com ports
#rc.ft_linear.com_close()
#rc.ft_rotary.com_close()
rc.laser.com_close()
#rc.attenuator.com_close()
#rc.aperture.com_close()

time.sleep(1)
rc.assembler_stop()
rc.broker_stop()
rc.encoder_stop()

time.sleep(1)

rc.assembler_alive()
rc.broker_alive()
rc.encoder_alive()
