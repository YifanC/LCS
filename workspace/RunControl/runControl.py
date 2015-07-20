
from base.controls import *
from services.communication import Producer
from services.data import LaserData
from devices.feedtrough import *
from devices.laser import *
from devices.attenuator import *
from devices.aperture import *

# ----------------------------------------------------
# ----------------------- Init -----------------------
# ----------------------------------------------------

RunNumber = 99
# Construct needed instances
rc = Controls(RunNumber=RunNumber)
data = LaserData()
rc.com = Producer("runcontrol")
rc.ft_linear = Feedtrough("linear")
rc.ft_rotary = Feedtrough("rotary")
rc.laser = Laser()
rc.attenuator = Attenuator()
rc.aperture = Aperture()

# Start broker / encoder
rc.broker_start()
rc.assembler_start()
time.sleep(2)
rc.encoder_start()

# Initialize and setup communication
rc.com.id = 1
rc.com.state = -1
rc.com.start()
rc.com.send_hello()
rc.com.state = 0

# Initialize com ports
#rc.ft_linear.com_init()
#rc.ft_rotary.com = rc.ft_linear.com
#rc.laser.com_init()
#rc.attenuator.com_init()
#rc.aperture.com_init()

# ----------------------------------------------------
# --------------------- Operation --------------------
# ----------------------------------------------------
rc.com.send_data(data)

time.sleep(10)
rc.assembler_alive()
rc.broker_alive()
rc.encoder_alive()

# ----------------------------------------------------
# -------------------- Finalize ----------------------
# ----------------------------------------------------

time.sleep(1)
rc.assembler_stop()
rc.broker_stop()
rc.encoder_stop()

time.sleep(1)

rc.assembler_alive()
rc.broker_alive()
rc.encoder_alive()
