import subprocess as subprocess
import time
import signal



def StartEncoder(RunNr):

def StopEncoder(RunNr):

def StartAssembler(RunNr):

def StopAssembler()

RunNumber
LogFilenmae = time.strftime("%Y-%m-%d-%H%M", time.localtime()) + str(RunNumber) + str(".log")
OutputLogfile = open("log/stdout.txt", "wb")

ProdEncoder = subprocess.Popen(['../write'], stdout=OutputLogfile)

time.sleep(5)

ProdEncoder.send_signal(signal.SIGINT)
OutputLogfile.close()


