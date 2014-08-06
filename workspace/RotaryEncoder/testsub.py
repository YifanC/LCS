
import os
import sys
import subprocess
import time
import signal

subprocess.Popen("cd ~/workspace/RotaryEncoder/",shell=True)
proc = subprocess.Popen("./2encoder > log.txt",shell=True,stdin=subprocess.PIPE,stderr=subprocess.PIPE)
time.sleep(2)
proc.stdin.write('n\n')
proc.stdin.flush()
time.sleep(2)
proc.stdin.write("n\n")
proc.stdin.flush()

time.sleep(15)

proc.send_signal(signal.SIGINT)
time.sleep(1)
proc.terminate()

