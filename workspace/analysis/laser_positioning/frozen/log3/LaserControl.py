import sys, time, glob
import serial

def initLaser():
        print '---------------- Laser Initialization ----------------'
        
        timeout=0.1
        print "Get Laser status:"
        serLaser.write("SE\r")
        reply = serLaser.read(20)
        print " My Status is: ", reply

        time.sleep(timeout)

        print "Get Laser shots:"
        serLaser.write("SC\r")
        reply = serLaser.read(9)
        print " I have been shooting: ", reply

        time.sleep(timeout)

        print "Turn Laser on:"
        #serLaser.write("ST 1\r")
        reply = serLaser.read(20)
        print " Laser is on: ", reply

        time.sleep(2)

        print "Set single shot mode"
        serLaser.write("PD 000\r")
        reply = serLaser.read(20)
        print " current mode ", reply
        print '------------------------------------------------------'
def startLaser():
        print "Start Laser:"
        serLaser.write("ST 1\r")
        reply = serLaser.read(20)
        print "Laser on:", reply
def stopLaser():
        print "Start Laser:"
        serLaser.write("ST 0\r")
        reply = serLaser.read(20)
        print "Laser on:", reply

def singleModeLaser():
        print "Enter Single Mode:"
        serLaser.write("PD 000\r")
        reply = serLaser.read(20)
        print "Laser in single mode:", reply
        
def openShutter():
        print "Open shutter:"
        serLaser.write("SH 1\r")
        reply = serLaser.read(20)
        print "Shutter is:", reply
        time.sleep(5)
        
def closeShutter():
        print "Close shutter:"
        serLaser.write("SH 0\r")
        reply = serLaser.read(20)
        print "Shutter is:", reply

def SHOOT():
        time.sleep(0.5)
        print "SHOOT"
        serLaser.write("SS\r")
        reply = serLaser.read(20)
        print "answer:", reply

def SHOOT10Hz():
        serLaser.write("PD 001\r")
        reply = serLaser.readline()
        print "answer:", reply

def SHOOT1Hz(TShoot):
        serLaser.write("PD 010\r")
        reply = serLaser.readline()
        print "answer:", reply
        
        time.sleep(TShoot)
        print "Stop SHOOT at 1Hz"
        serLaser.write("PD 000\r")
        reply = serLaser.readline()
        print "answer:", reply
        

def initLaserCOM(comport):
   try:
      global serLaser
      serLaser = serial.Serial('/dev/ttyUSB' + str(comport), 9600, 8, 'N', 1,timeout=1)
     
   except:
      print "Error opening com port. Quitting."
      sys.exit(0)

   print "Opening " + serLaser.portstr
   print serLaser.isOpen()
   print serLaser.portstr
