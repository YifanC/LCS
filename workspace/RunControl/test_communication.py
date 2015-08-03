__author__ = 'matthias'

from base.controls import *

from services.communication import Producer
from services.data import LaserData

import time
import signal
global stop
stop = False


def sigint_handler(signal, frame):
    print "\n Shutting down, bye bye"
    print 'signal: ' + str(signal)
    end()
    sys.exit(0)

def end():
    print rc.encoder_alive()
    rc.encoder_stop()
    #print rc.com.context.term()
    print rc.assembler_alive()
    print rc.assembler_stop()


    print rc.broker_stop()

signal.signal(signal.SIGINT, sigint_handler)

RunNumber = 999999

# Construct needed instances
rc = Controls(RunNumber=RunNumber)
data = LaserData()
rc.com = Producer("runcontrol")

# Start broker / encoder
rc.broker_start()
rc.assembler_start()
time.sleep(2)
rc.encoder_start(dry=True)

# Initialize and setup communication
rc.com.id = 1
rc.com.state = -1
rc.com.start()
rc.com.send_hello()
rc.com.state = 0

while True:
    time.sleep(2)
    rc.com.send_data(data)
