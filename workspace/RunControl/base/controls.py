__author__ = 'matthias'

import subprocess as subprocess
import signal
from base import *
import os


class Controls(Base):
    def __init__(self, RunNumber):
        self.name = "controls"
        super(Controls, self).__init__(name=self.name, logit=True, RunNumber=RunNumber)

        self.path_base = os.getenv("LCS_BASE")
        self.path_services = os.getenv("LCS_SERVICES")
        self.path_devices = os.getenv("LCS_DEVICES")
        self.path_macros = os.getenv("LCS_MACROS")

        if self.path_base is None:
            self.printError("Could not find path to base files -> aborting")
            sys.exit(1)

    def encoder_start(self, dry_run=False, ext_trig=False, ref_run=False, send_data=True):
        # TODO: Reset encoder before first start. Make sure that the encoder is in a nice state when we try to turn it on.
        args = ""
        self.printMsg("Starting Encoder")
        if dry_run is True:
            command = self.path_services + "/" + "test_client.o"
        if dry_run is False:
            command = self.path_devices + "/" + "encoder.o"
            if send_data is True:
                # send data to zmq server
                args += '-s'
            if ext_trig is False:
                # Trigger every second
                args += ' -t 1000'
            if ref_run is True:
                # do a reference run first
                args += ' -r'

        self.printMsg(args)
        self.proc_encoder = self.process_start(command, args=args, c=True)

    def encoder_alive(self):
        alive = self.process_alive(self.proc_encoder)
        self.printMsg("Is Encoder alive? " + str(alive))
        return alive

    def encoder_stop(self):
        self.printMsg("Stopping Encoder")
        self.process_stop(self.proc_encoder)

    def assembler_start(self, senddata=True):
        self.printMsg("Starting Assembler")
        if senddata is True:
            self.proc_assembler = self.process_start(self.path_macros + "/" + "assembler.py",
                                                     args="-r " + str(self.RunNumber) + " -c", py=True)
        else:
            self.proc_assembler = self.process_start(self.path_macros + "/" + "assembler.py",
                                                     args="-r " + str(self.RunNumber), py=True)


    def assembler_alive(self):
        alive = self.process_alive(self.proc_assembler)
        self.printMsg("Is Assembler alive? " + str(alive))
        return alive

    def assembler_stop(self):
        self.printMsg("Stopping Assembler")
        self.process_stop(self.proc_assembler)

    def broker_start(self):
        self.printMsg("Starting Broker")
        self.proc_broker = self.process_start(self.path_macros + "/" + "broker.py", py=True)
        pass

    def broker_alive(self):
        alive = self.process_alive(self.proc_broker)
        self.printMsg("Is Broker alive? " + str(alive))
        return alive

    def broker_stop(self):
        self.printMsg("Stopping Broker")
        self.process_stop(self.proc_broker)

    def process_start(self, filename, args="", py=False, c=False):
        # TODO: Handle error messages of c process
        if py is True:
            prefix = "python"
            command = [prefix, filename]
        elif c is True:
            # prefix = "./"
            command = [filename]
        else:
            self.printError("do not know how to execute!")
            return False
        if args is not "":
            for i in range(len(args.split())):
                command.append(args.split()[i])
        # For debugging
        # process = subprocess.Popen(command)
        process = subprocess.Popen(command, shell=False)
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
            # procces.terminate()
            return True
        else:
            return True

    def test(self):
        print "starting python process"
        proc1 = self.process_start(self.path_base + "/" + "test_proc.py", py=True, args="-p 1")
        print "starting c process"
        proc2 = self.process_start(self.path_base + "/" + "test_proc.o", c=True)
        # print proc2
        time.sleep(2)
        print "is the python process alive? \n" + str(self.process_alive(proc1))
        print "is the c process alive? \n" + str(self.process_alive(proc2))
        print "stopping the python process \n" + str(self.process_stop(proc1))
        print "stopping the c process \n" + str(self.process_stop(proc2))
        time.sleep(1)
        print "is the python process alive? \n" + str(self.process_alive(proc1))
        print "is the c process alive? \n" + str(self.process_alive(proc2))

        print "closing a non existent process"
