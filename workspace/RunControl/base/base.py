__author__ = 'matthias'

import serial
from abc import ABCMeta, abstractmethod
import sys
import time
import json

import logging
import os

DEBUG = False


class Base(object):
    def __init__(self, name="", RunNumber=0, logit=True):  # TODO: Implement to put name in all classes
        self.name = name
        self.server = -1
        self.state = 0
        self.StateDict = {0: "Not Initialized",
                          1: "Ready",
                          2: "Error"}
        # switch this to false if using bpython
        self.color = False
        self.config = None
        self.config_com = None
        self.RunNumber = RunNumber  # TODO: Implement passing of run number from the instance

        self.path_logfiles = os.getenv("LCS_LOGFILES")
        if self.path_logfiles is None:
            self.printError("Could not find path to log file -> aborting")
            sys.exit(1)

        # TODO: Make this available everywhere
        self.log = self.config_logging(self.RunNumber, logit)
        self.log.info("started logger")


    def printMsg(self, string, nonewline=False):
        string = str(string)
        msg = time.strftime('%H:%M:%S ', time.localtime()) + self.name + ": " + string
        if nonewline == True:
            print msg,
        else:
            print msg

        self.log.info(string)

    def printError(self, string):
        string = str(string)
        msg = time.strftime('%H:%M:%S ', time.localtime()) + "ERROR: " + self.name + ": " + string
        msg_colored = bcolors.FAIL + time.strftime('%H:%M:%S ',
                                                   time.localtime()) + "ERROR: " + self.name + ": " + string + bcolors.ENDC

        if self.color is True:
            print msg_colored
        else:
            print msg

        self.log.error(string)

    def printDebug(self, string):
        if DEBUG is True:
            string = str(string)
            msg = time.strftime('%H:%M:%S ', time.localtime()) + "DEBUG " + self.name + " " + string
            msg_colored = bcolors.WARNING + time.strftime('%H:%M ',
                                                          time.localtime()) + self.name + ": " + string + bcolors.ENDC
            if self.color is True:
                print msg_colored
            else:
                print msg

        self.log.debug(string)

    def config_setfile(self, filename=-1):
        """ Look for config files in the folders """
        if filename == -1:
            filename = "config_" + str(self.name) + ".json"

        hostname = os.uname()[1]
        if hostname[11:14] == "one":
            path = os.getenv("LCS_DEVICES") + "/config_server1/"
            self.server = 1
        elif hostname[11:14] == "two":
            path = os.getenv("LCS_DEVICES") + "/config_server2/"
            self.server = 2
        else:
            self.printMsg("CAUTION USING NON SPECIFIC CONFIG FILES")
            path = os.getenv("LCS_DEVICES") + "/config/"
            self.server = 0

        configfilename = path + str(filename)

        self.printMsg("Using config file (" + configfilename + ")")

        return configfilename


    def com_load(self, filename):
        """ load the json file containing the mapping of com ports to devices """
        configfilename = self.config_setfile(filename)

        with open(configfilename, 'r') as configfile:
            self.ComPortsDict = json.load(configfile)
            configfile.close()

    def config_load(self, filename=-1):
        self.configfilename = self.config_setfile(filename)

        with open(self.configfilename, 'r') as configfile:
            self.config = json.load(configfile, object_hook=self.Device_Config)
            configfile.close()

    def config_dump(self):
        self.printMsg("Storing configuration")
        with open(self.configfilename, 'w') as configfile:
            json.dump(self.config.__dict__, configfile)
            configfile.close()


    def config_logging(self, RunNr, logit, LogFilename="", ):
        if LogFilename == "":
            LogFilename = time.strftime("%Y-%m-%d-%H%M-Run-", time.localtime()) + str(RunNr) + str(".log")
            LogFilePath = str(self.path_logfiles) + "/" + str(self.name) + "/"

        else:
            LogFilePath = ""

        # TODO: Make the logfiles merge into eachother, so only one logfile is generated. This would work in the
        # TODO: following way: instead of handling the logging separately one would start a logging instance in the
        # TODO: runControl.py and add all the handlers also in there.

        log = logging.getLogger(self.name)

        if logit is True:
            fh = logging.FileHandler(LogFilePath + LogFilename, "wb")
            f = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
            fh.setFormatter(f)
            log.setLevel(logging.DEBUG)
            log.addHandler(fh)
        else:
            log.addHandler(logging.NullHandler())
        # printing out to console
        # console = logging.StreamHandler()
        # console.setLevel(logging.INFO)
        # logging.getLogger(self.name).addHandler(console)

        return log

    def close_logfile(self):
        pass

    class Device_Config():
        """" Config class just to translate the json file into a dict for actual device configurations """

        def __init__(self, f):
            self.__dict__ = f

    class Com_Config():
        """" Config class just to translate the json file into a dict for com port configuration of server """

        def __init__(self, f):
            self.__dict__ = f


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
