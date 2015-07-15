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

    def test(self, string):
        print string

    def encoder_start(RunNr):
        ProdEncoder = subprocess.Popen(['./write.o'])
        return ProdEncoder

    def encoder_alive(self):
        pass

    def encoder_stop(self):
        #subEncoder.send_signal(signal.SIGINT)
        pass

    def assembler_start(self):
        self.printMsg("Starting Assembler")
        self.proc_assembler = self.process_start(self.path_services + "/" + "test_server.py", py=True)

    def assembler_alive(self):
        return self.process_alive(self.proc_assembler)

    def assembler_stop(self):
        self.printMsg("Stopping Assembler")
        self.process_stop(self.proc_assembler)

    def broker_start(self):

        pass

    def broker_alive(self):
        pass

    def broker_stop(self):
        pass

    def process_start(self, filename, args="", py=False, c=False):
        if py is True:
            prefix = "python"
            command =  [prefix, filename, args]
        elif c is True:
            prefix = "./"
            command =  [prefix + command, args]
        else:
            self.printError("do not know how to execute!")
            return False

        # For debugging process = subprocess.Popen(command)
        process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
        return process

    def process_alive(self, process):
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
        proc1 = self.process_start("test_proc.py", py=True, args="-p 1")
        print "starting c process"
        proc2 = self.process_start("./test_proc.o", c=True)
        #print proc2
        time.sleep(2)
        print "is the python process alive? \n" + str(self.process_alive(proc1))
        print "is the c process alive? \n" + str(self.process_alive(proc2))
        print "stopping the python process \n" + str(self.process_stop(proc1))
        print "stopping the c process \n" + str(self.process_stop(proc2))
        time.sleep(1)
        print "is the python process alive? \n" + str(self.process_alive(proc1))
        print "is the c process alive? \n" + str(self.process_alive(proc2))