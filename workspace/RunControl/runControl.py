
from base.controls import *
from services.communication import Producer
from services.data import LaserData

rc = Controls()
data = LaserData()
rc.com = Producer("runcontrol")

rc.broker_start()
rc.assembler_start()
time.sleep(2)
rc.encoder_start()


rc.com.id = 1
rc.com.state = -1
rc.com.start()
rc.com.send_hello()
rc.com.state = 0
rc.com.send_data(data)

time.sleep(10)
rc.assembler_alive()
rc.broker_alive()
rc.encoder_alive()

time.sleep(1)
rc.assembler_stop()
rc.broker_stop()
rc.encoder_stop()

time.sleep(1)

rc.assembler_alive()
rc.broker_alive()
rc.encoder_alive()
