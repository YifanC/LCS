import subprocess as subprocess
import time
import signal


def openLogfile(RunNr, service):
    LogFilenmae = time.strftime("%Y-%m-%d-%H%M-Run-", time.localtime()) + str(RunNr) + str(".log")
    LogFilePath = "./log/" + str(service)

    OutputLogfile = open(LogFilePath + LogFilenmae, "wb")

    return OutputLogfile

def startEncoder(RunNr):
    LogfileEncoder = openLogfile(RunNumber, "encoder")
    ProdEncoder = subprocess.Popen(['./write.o'], stdout=LogfileEncoder)
    return ProdEncoder

def stopEncoder():
    subEncoder.send_signal(signal.SIGINT)


RunNumber = 000
subEncoder = startEncoder(0)
time.sleep(5)

stopEncoder()


