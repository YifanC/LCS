from datetime import datetime
import argparse
import signal

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

parser.add_argument("-n", "--nowarmup", action='store_false', dest='warmup', default='true', required=False,
                    help='do not wait 20 minutes for the laser to warm up')

arguments = parser.parse_args()

RunNumber = arguments.RunNumber
warmup = arguments.warmup

def sigint_handler(signal, frame):
    rc.printMsg("Stopping laser run on user request (sigint)")
    stop()
    raise SystemExit(0)


def stop():
    rc.laser.closeShutter()
    rc.ft_linear.stopMovement()
    rc.ft_rotary.stopMovement()
    rc.attenuator.stopMovement()


def finalize():
    # ----------------------------------------------------
    # -------------------- Finalize ----------------------
    # ----------------------------------------------------
    rc.laser.closeShutter()
    rc.laser.setRate(0)
    
    # Just for now!
    #rc.laser.stop()

    # Close com ports
    # rc.ft_linear.com_close()
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

def initMotors():

    # Homing Feedtrough
    rc.ft_linear.initAxis()
    rc.ft_rotary.initAxis()
    #rc.ft_linear.homeAxis()
    rc.ft_rotary.homeAxis()

    # start the encoder just before we do the reference run, the wait a short time to let it set things up.
    # Also the zmq server will be ready at this point
    rc.encoder_start(dry_run=False, ext_trig=False, ref_run=True)
    time.sleep(1)

    # move rotary ft a bit to get the encoder to read the reference marks (50000 microsteps is enough)
    #rc.ft_rotary.printMsg("Performing movement to detect reference marks")
    #rc.ft_rotary.moveRelative(250000, monitor=True)
    # homing Attenuator


def init():
    # init communication
    rc.com.id = 1
    rc.com.state = -1
    rc.com.start()
    rc.com.send_hello()  # it will hang here if no communication is set up
    rc.com.state = 0

    # Initialize com ports
    rc.ft_rotary.com_init()
    rc.ft_linear.com = rc.ft_rotary.com
    rc.laser.com_init()
    rc.attenuator.com_init()
    #rc.aperture.com_init()
    rc.mirror_x.com_init()
    rc.mirror_y.com = rc.mirror_x.com


def startup():
    rc.laser.timeout = 0.01*60  # seconds
    start_time = datetime.today()

    initialized = False
    rc.laser.start()

    if warmup:
        elapsed_time = (datetime.today() - start_time).seconds
        while elapsed_time < rc.laser.timeout:
            rc.laser.printMsg(str("waiting for laser to warm up: " + str(rc.laser.timeout- elapsed_time) + " seconds left \r"), nonewline=True)
            if initialized is False:
                # Initialize and setup communication
                initMotors()
                initialized = True

            time.sleep(1)
            elapsed_time = (datetime.today() - start_time).seconds
    else:
         initMotors()

    rc.laser.setRate(0)
    rc.laser.closeShutter()

def run():
    # ----------------------------------------------------
    # --------------------- Operation --------------------
    # ----------------------------------------------------
    rc.com.send_data(data)
    for scanstep in range(len(pos)):
        pos.printStep(scanstep)
        data.count_run = scanstep
        # Set scanning speeds of axis
        rc.ft_rotary.setParameter("VM", pos.getHorSpeed(scanstep))
        rc.ft_linear.setParameter("VM", pos.getVerSpeed(scanstep))

        # Set Attenuator Position
        rc.attenuator.moveAbsolute(pos.getAttenuator(scanstep), monitor=True, display=True)
        data.pos_att = rc.attenuator.getPosition()
        # just for debugging:
        data.pos_att = pos.getAttenuator(scanstep)

        # Configure the Laser to shoot at a given frequency while moving
        if pos.getShotFreq(scanstep) > 0:
            rc.printMsg("Moving and shooting @ " + str(pos.getShotFreq(scanstep)) + "Hz")
            rc.laser.setRate(pos.getShotFreq(scanstep))
            rc.laser.openShutter()
        else:
            rc.laser.setRate(0)

        # Do the actual movement and monitor it if movement step is non-zero
        if pos.getHorRelativeMovement(scanstep) != 0:
            rc.ft_rotary.moveRelative(pos.getHorRelativeMovement(scanstep), monitor=True)
        if pos.getVerRelativeMovement(scanstep) != 0:
            rc.ft_linear.moveRelative(pos.getVerRelativeMovement(scanstep), monitor=True)

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
                            str(pos.getShotCount(scanstep) / float(abs(pos.getShotFreq(scanstep)))) + " seconds")
                rc.laser.setRate(abs(pos.getShotFreq(scanstep)))
                rc.laser.openShutter()
                time.sleep(pos.getShotCount(scanstep) / float(abs(pos.getShotFreq(scanstep))))
                rc.laser.closeShutter()
                rc.laser.setRate(0)
            else:
                rc.printMsg("No laser shots in this step")

        # send the data to the assembler
        rc.com.send_data(data)


# ----------------------------------------------------
# ----------------------- Init -----------------------
# ----------------------------------------------------
# Construct needed instances

signal.signal(signal.SIGINT, sigint_handler)

rc = Controls(RunNumber=RunNumber)
data = LaserData(RunNumber=RunNumber)
pos = Positions(RunNumber=RunNumber)
rc.com = Producer("runcontrol")
rc.ft_linear = Feedtrough("linear_actuator", RunNumber=RunNumber)
rc.ft_rotary = Feedtrough("rotary_actuator", RunNumber=RunNumber)
rc.laser = Laser(RunNumber=RunNumber)
rc.attenuator = Attenuator(RunNumber=RunNumber)

# rc.aperture = Aperture()
rc.mirror_x = Mirror("mirror221", 1)  # not yet defined correctly
rc.mirror_y = Mirror("mirror222", 2)  # not yet defined correctly

# define data
data.laserid = rc.ft_linear.server

# Start broker / encoder
rc.broker_start()
rc.assembler_start(senddata=False)
time.sleep(2)

# Load Positions from file
pos.load("./services/config.csv")

# Dry run configuration
#rc.laser.comDryRun = True
#rc.attenuator.comDryRun = True
#rc.ft_rotary.comDryRun = True
#rc.ft_linear.comDryRun = True

# init
init()

# Start up devices
startup()
rc.com.send_data(data)

# Ask for the start

rc.assembler_alive()
rc.broker_alive()
rc.encoder_alive()


