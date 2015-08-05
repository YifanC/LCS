__author__ = 'matthias'

import socket
import sys

from services.data import *
from base.base import *


class TCP(Base):
    def __init__(self, server_name="localhost", port=10000):
        """ Initialize data """
        self.name = "tcp"
        super(TCP, self).__init__(name=self.name,  logit=True)

        self.server_address = (server_name, port)
        self.laser_data = LaserData()

    def start_server(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind(self.server_address)
        self.sock.listen(1)

    def stop_server(self):
        self.sock.close()

    def recv_server(self):
        print >> sys.stderr, 'waiting for a connection'
        connection, client_address = self.sock.accept()
        try:
            print >> sys.stderr, 'client connected:', client_address
            while True:
                data = connection.recv(60)
                if DEBUG:
                    print >> sys.stderr, 'received "%s"' % data
                if data:
                    self.laser_data.fill(self.laser_data.unpack(data))
                    connection.sendall("OK")
                else:
                    break

        finally:
            connection.close()

        return self.laser_data


    def send_client(self, data):
        """ sends laser data over tcp socket to TPC DAQ. Returns True if successful and False if not. If server is not
        availabe or if pipe is broken the data is not send and function returns False. """
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.sock.connect(self.server_address)
        except Exception as e:
            self.printError("Could not connect to server: " + str(e))
            return False
        self.printMsg("sending data")
        packed_data = data.pack()
        try:
            self.sock.sendall(packed_data)

            amount_received = 0
            amount_expected = len("OK")
            while amount_received < amount_expected:
                data = self.sock.recv(4)
                amount_received += len(data)
                self.printMsg("recieved: " + str(data))
        except Exception as e:
            self.printError("Could not send data to server: " + str(e))


        finally:
            self.sock.close()

        self.sock.close()

    def check_server(self):
        """ checks if server is alive """
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.sock.connect(self.server_address)
        except Exception as e:
            self.printMsg("Could not connect to server: " + str(e))
            return False

        self.printMsg("server is alive")
        self.sock.close()

        return True
