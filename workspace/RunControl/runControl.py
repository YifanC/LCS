
from datetime import datetime
import argparse

from base.controls import *
from services.communication import Producer
from services.data import LaserData

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
rc.com = Producer("runcontrol")
rc.ft_linear = Feedtrough("linear_actuator")
rc.ft_rotary = Feedtrough("rotary_actuator")
rc.laser = Laser()
rc.attenuator = Attenuator()
rc.aperture = Aperture()
rc.mirror_x = Mirror("mirror221", 1)  # not yet defined correctly
rc.mirror_y = Mirror("mirror222", 2)  # not yet defined correctly

# Start broker / encoder
rc.broker_start()
print rc.RunNumber
rc.assembler_start()
time.sleep(2)

# Dry run configuration
rc.encoder_start(dry=True)
rc.laser.comDryRun = True
rc.ft_rotary.comDryRun = True
rc.ft_linear.comDryRun = True

# Initialize com ports
rc.ft_linear.com_init()
rc.ft_rotary.com = rc.ft_linear.com
rc.laser.com_init()
#rc.attenuator.com_init()
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


raw_input("Start Laser Scan?")
# ----------------------------------------------------
# --------------------- Operation --------------------
# ----------------------------------------------------
rc.com.send_data(data)

time.sleep(5)
rc.laser.setRate(1)
rc.laser.openShutter()
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
