__author__ = 'matthias'

import subprocess as subprocess
import time
import signal
from base import *
import os


class Controls(base):

    def __init__(self):
        self.name = "controls"
        super(Controls, self).__init__(name=self.name, logit=False)

        self.path_base = os.getenv("LCS_BASE")
        self.path_services = os.getenv("LCS_SERVICES")
        self.path_devices = os.getenv("LCS_BASE")

        if self.path_base is None:
            self.printError("Could not find path to base files -> aborting")
            sys.exit(1)

    def encoder_start(self):
        self.printMsg("Starting Encoder")
        self.proc_encoder = self.process_start(self.path_services + "/" + "test_client.o", c=True)

    def encoder_alive(self):
        alive = self.process_alive(self.proc_encoder)
        self.printMsg("Is Encoder alive? " + str(alive))
        return alive

    def encoder_stop(self):
        self.printMsg("Stopping Encoder")
        self.process_stop(self.proc_encoder)

    def assembler_start(self):
        self.printMsg("Starting Assembler")
        self.proc_assembler = self.process_start(self.path_services + "/" + "test_server.py", py=True)

    def assembler_alive(self):
        alive = self.process_alive(self.proc_assembler)
        self.printMsg("Is Assembler alive? " + str(alive))
        return alive

    def assembler_stop(self):
        self.printMsg("Stopping Assembler")
        self.process_stop(self.proc_assembler)

    def broker_start(self):
        self.printMsg("Starting Broker")
        self.proc_broker = self.process_start(self.path_services + "/" + "test_broker.py", py=True)
        pass

    def broker_alive(self):
        alive = self.process_alive(self.proc_broker)
        self.printMsg("Is Broker alive? " + str(alive))
        return alive

    def broker_stop(self):
        self.printMsg("Stopping Broker")
        self.process_stop(self.proc_broker)

    def process_start(self, filename, args="", py=False, c=False):
        if py is True:
            prefix = "python"
            command =  [prefix, filename, args]
        elif c is True:
            #prefix = "./"
            command =  [filename, args]
        else:
            self.printError("do not know how to execute!")
            return False
        # For debugging
        #process = subprocess.Popen(command)
        process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=False)
        return process

    def process_alive(self, process):
        # TODO: Check if process exists!
        if process.poll() is None:
            return True
        else:
            return False

    def process_stop(self, procces):
        if self.process_alive(procces):
            procces.send_signal(signal.SIGINT)
            procces.terminate()
            return True
        else:
            return True

    def test(self):
        print "starting python process"
        proc1 = self.process_start(self.path_base + "/" + "test_proc.py", py=True, args="-p 1")
        print "starting c process"
        proc2 = self.process_start(self.path_base + "/" + "test_proc.o", c=True)
        #print proc2
        time.sleep(2)
        print "is the python process alive? \n" + str(self.process_alive(proc1))
        print "is the c process alive? \n" + str(self.process_alive(proc2))
        print "stopping the python process \n" + str(self.process_stop(proc1))
        print "stopping the c process \n" + str(self.process_stop(proc2))
        time.sleep(1)
        print "is the python process alive? \n" + str(self.process_alive(proc1))
        print "is the c process alive? \n" + str(self.process_alive(proc2))

        print "closing a non existent process"
