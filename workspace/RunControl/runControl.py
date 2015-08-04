
from base.controls import *
from services.communication import Producer
from services.data import LaserData

from devices.feedtrough import *
from devices.laser import *
from devices.attenuator import *
from devices.aperture import *
from devices.mirror import *

# ----------------------------------------------------
# ----------------------- Init -----------------------
# ----------------------------------------------------

RunNumber = 99
# Construct needed instances
rc = Controls(RunNumber=RunNumber)
data = LaserData()
rc.com = Producer("runcontrol")
rc.ft_linear = Feedtrough("ft_linear", 1)
rc.ft_rotary = Feedtrough("ft_rotary", 1)
rc.laser = Laser()
rc.attenuator = Attenuator()
rc.aperture = Aperture()
rc.mirror_x = Mirror("mirror221")  # not yet defined correctly
rc.mirror_y = Mirror("mirror222")  # not yet defined correctly

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
#rc.mirror_x.com_init()
#rc.mirror_y.com = rc.mirror_x.com

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

# Close com ports
#rc.ft_linear.com_close()
#rc.ft_rotary.com_close()
#rc.laser.com_close()
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
