__author__ = 'matthias'
from services.communication import *
from services.data import *
from services.tcp import *

import argparse
import signal

# handling arguments
parser = argparse.ArgumentParser(description='Script controlling the start and stop of the data assembler')

parser.add_argument("-r", "-runnumber", dest='RunNumber', required=True, action="store",
                    help='Neccessary run number where to store the data for the txt file.')

parser.add_argument("-c", "-connect", dest='connect', required=False, action="store_true", default=False,
                    help='If set assembler will try to connect over a pipe to seb10 abd send data.')

# TODO: Implement a logic so states from the different clients are written to file

parser.set_defaults(connect=False)
arguments = parser.parse_args()
connect = arguments.connect


def sigint_handler(signal, frame):
    assembler.printMsg("stopping assembler")
    assembler.stop()

    raise SystemExit(1)


SERVER = "localhost"
PORT_SERVER = 33487
PORT_CLIENT = 33488

# SERVER = ''
#PORT_SERVER = 33488
#PORT_CLIENT = 33488

signal.signal(signal.SIGINT, sigint_handler)
data = LaserData(RunNumber=arguments.RunNumber)
client = TCP(SERVER, port_server=PORT_SERVER, port_client=PORT_CLIENT)  # Just for local tests

assembler = Consumer("assembler")
assembler.start()
assembler.color = False
# assembler.open_logfile()
assembler.printDebug("starting")

if connect is True:
    while client.start_server() is False:
        assembler.printDebug("trying to connect to client")
        time.sleep(1)

assembler.printDebug("starting server is over...")

assembler.printMsg("------------ Wait for Hello ------------")
ready = False
assembler.timeout = 5
while not ready:
    ready = assembler.recv_hellos()
assembler.printMsg("--------------- Start DAQ --------------")
while True:
    [source_id, state] = assembler.recv(data)
    data.writeBinary('test.bin')

    # only write if new encoder data arrived
    if source_id == 2:
        if connect is True:
            assembler.printDebug(data)
            client.send_server(data)
        data.writeTxt()
    assembler.printMsg("recieved data trigger: " + str(data.count_trigger))
