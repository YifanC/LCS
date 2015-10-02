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

# Ask for the arguments
parser = argparse.ArgumentParser(description='Script to control the UV laser system')

parser.add_argument("-r", "--runnumber", action='store', dest='RunNumber', required=True,
                    help='current run number issued by DAQ', type=int)

parser.add_argument("-c", "--configfile", action='store', dest='configfile', required=True,
                    help='Please supply path to movement configuration', type=str)

parser.add_argument("-n", "--nowarmup", action='store_false', dest='warmup', default=True, required=False,
                    help='do not wait 20 minutes for the laser to warm up')

parser.add_argument("-dry", "--dry_run", action='store_true', dest='dry_run', default=False, required=False,
                    help='perform a dry run. No com ports opened. Using a fake position producer')

parser.add_argument("-l", "--local_mode", action='store_false', dest='send_data', default=True, required=False,
                    help='Do not send data to uboone DAQ on seb10.')

parser.add_argument("-i", "--int_trig", action='store_true', dest='int_trig', default=False, required=False,
                    help='Use internal trigger (every second) for position encoding')

parser.add_argument("-noref", "--no_ref_run", action='store_true', dest='ref_run', default=True, required=False,
                    help='Do not perform a reference run for the encoder.')

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
    rc.laser.stop()
    # Close com ports
    rc.ft_linear.com_close()
    rc.ft_rotary.com_close()
    rc.laser.com_close()
    rc.attenuator.com_close()
    #rc.aperture.com_close()

    rc.mirror111.com_close()
    rc.mirror121.com_close()

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
    if arguments.dry_run is False:
        # These need special replies which are not fullfiled in a dry run
        rc.ft_linear.initAxis()
        rc.ft_rotary.initAxis()
        #rc.ft_linear.homeAxis()
        rc.ft_rotary.homeAxis()

    # start the encoder just before we do the reference run, the wait a short time to let it set things up.
    # Also the zmq server will be ready at this point
    if arguments.ref_run is True:
	# move rotary ft a bit to get the encoder to read the reference marks (50000 microsteps is enough)
        rc.ft_rotary.printMsg("Performing movement to detect reference marks")
        rc.ft_rotary.moveRelative(200000, monitor=True)
        rc.ft_rotary.homeAxis()
    rc.encoder_start(dry_run=False, ext_trig=True, ref_run=True)
    time.sleep(1)

    # move rotary ft a bit to get the encoder to read the reference marks (50000 microsteps is enough)
    rc.ft_rotary.printMsg("Performing movement to detect reference marks")
    rc.ft_rotary.moveRelative(220000, monitor=True)
    # homing Attenuator
    rc.ft_rotary.homeAxis()

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
    # rc.aperture.com_init()

    # mirror laser box
    rc.mirror111.com_init()
    rc.mirror112.com = rc.mirror111.com

    # mirro feedthrough
    rc.mirror121.com_init()
    rc.mirror122.com = rc.mirror121.com


def startup():
    rc.laser.timeout = 0.01 * 60  # seconds
    start_time = datetime.datetime.today()

    initialized = False
    rc.laser.start()

    if warmup:
        elapsed_time = (datetime.datetime.today() - start_time).seconds
        while elapsed_time < rc.laser.timeout:
            rc.laser.printMsg(
                str("waiting for laser to warm up: " + str(rc.laser.timeout - elapsed_time) + " seconds left \r"),
                nonewline=True)
            if initialized is False:
                # Initialize and setup communication
                initMotors()
                initialized = True

            time.sleep(1)
            elapsed_time = (datetime.datetime.today() - start_time).seconds
    else:
        initMotors()

    rc.laser.setRate(0)
    rc.laser.closeShutter()


def update_mirror_data():
    # get mirror positions

    data.pos_tomg_1_axis1 = rc.mirror111.getPosition()
    data.pos_tomg_1_axis2 = rc.mirror112.getPosition()
    data.pos_tomg_2_axis1 = rc.mirror121.getPosition()
    data.pos_tomg_2_axis2 = rc.mirror122.getPosition()

    data.count_laser = rc.laser.getShots()
    rc.com.send_data(data)

def config_dryRun():
    rc.mirror111.comDryRun = True
    rc.mirror112.comDryRun = True
    rc.mirror121.comDryRun = True
    rc.mirror122.comDryRun = True
    rc.laser.comDryRun = True
    rc.attenuator.comDryRun = True
    rc.ft_rotary.comDryRun = True
    rc.ft_linear.comDryRun = True

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
        rc.com.send_data(data)

        # Configure the Laser to shoot at a given frequency while moving
        if pos.getShotFreq(scanstep) > 0:
            rc.printMsg("Moving and shooting @ " + str(pos.getShotFreq(scanstep)) + "Hz")
            rc.laser.setRate(pos.getShotFreq(scanstep))
            time.sleep(1)
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
rc.mirror111 = Mirror("mirror111", 1)
rc.mirror112 = Mirror("mirror112", 2)
rc.mirror121 = Mirror("mirror121", 1)
rc.mirror122 = Mirror("mirror122", 2)


# define data
data.laserid = rc.ft_linear.server

# Load Positions from file
pos.load(arguments.configfile)

rc.printMsg("----------------------------------------")
rc.printMsg(" Configuration: ")
rc.printMsg("  - Run Number:       " + str(RunNumber))
rc.printMsg("  - Using config:     " + str(arguments.configfile))
rc.printMsg("  - Scanning time     " + str(pos.scanning_time))
rc.printMsg("  - Dry Run:          " + str(arguments.dry_run))
rc.printMsg("  - Send Data:        " + str(arguments.send_data))
rc.printMsg("  - Reference Run:    " + str(arguments.ref_run))
rc.printMsg("  - Internal Trigger: " + str(arguments.int_trig))
rc.printMsg("----------------------------------------")
pos.load("./services/config_scan.csv")

try:
    # Start broker and assembler (encoder comes up later)
    rc.broker_start()
    rc.assembler_start(senddata=arguments.send_data)

    # Dry run configuration
    if arguments.dry_run is True:
        config_dryRun()
    # init
    init()

    # Start up devices
    startup()
    rc.assembler_alive()
    rc.broker_alive()
    rc.encoder_alive()

    update_mirror_data()

    # Ask for the start
    raw_input("Start Laser Scan?")

    run()
    rc.assembler_alive()
    rc.broker_alive()


    rc.encoder_alive()

except Exception as e:
    print "ERROR"
    print e
    SystemExit(-1)

finally:
    finalize()

