__author__ = 'matthias'
from services.communication import *
from services.data import *
from services.tcp import *

import argparse
import signal

# handling arguments
parser = argparse.ArgumentParser(description='Script controlling the start and stop of the data assembler')

parser.add_argument("-c", "-connect", dest='connect', required=False, action="store_true",
                    help='If set assembler will try to connect over a pipe to seb10 abd send data.')
parser.set_defaults(connect=False)
arguments = parser.parse_args()
connect = arguments.connect
print connect

def sigint_handler(signal, frame):
    print "stopping assembler"
    print 'signal: ' + str(signal)
    assembler.stop()

    raise SystemExit(1)

SERVER = "localhost"
PORT_SERVER = 33487
PORT_CLIENT = 33488
signal.signal(signal.SIGINT, sigint_handler)
data = LaserData()
client = TCP("localhost", port_server=PORT_SERVER, port_client=PORT_CLIENT)  # Just for local tests

assembler = Consumer("assembler")
assembler.start()
assembler.color = False
#assembler.open_logfile()
if connect is True:
    while client.start_server() is False:
        time.sleep(1)

print "------------ Wait for Hello ------------"
ready = False
assembler.timeout = 5
while not ready:
    ready = assembler.recv_hellos()
print "--------------- Start DAQ --------------"
while True:
    [source_id, state] = assembler.recv(data)
    data.writeBinary('test.bin')

    # only write if new encoder data arrived
    if source_id == 2:
        if connect is True:
            client.send_server(data)
        data.writeTxt()
    print "trigger " + str(data.count_trigger)
