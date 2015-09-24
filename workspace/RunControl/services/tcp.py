__author__ = 'matthias'

import socket

from services.data import *
from base.base import *

""" A little class containing routines to start a server and a client to exchange laser data. The server should be
    started in the following order and is intended for testing only. The actual server lives in C on the uboone DAQ.
                                     1. start_server(),
                                     2. server_revcv() which returns the data
                                     3. server_close()
    The client sends the laser data supplied to the host/server, it expects no answer. The starting should follow this
    procedure:  1. start_client() returns True if connection could be established
                2. send_client(DATA) takes the data and sends it over the socket to the server
                3. stop_client()
    """

class TCP(Base):
    def __init__(self, server_ip="localhost", port_server=10000, port_client=10001):
        """ You shoulf supply a serber ip, an port which is used on the server to listen for messages and a port on the
         client on which the data is sent from. """
        self.name = "tcp"
        super(TCP, self).__init__(name=self.name, logit=True)
        self.port_client = port_client
        self.server_address = (server_ip, port_server)
        self.laser_data = LaserData()

    def start_server(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind(self.server_address)
        self.sock.listen(1)
        self.connection, self.client_address = self.sock.accept()

    def stop_server(self):
        self.connection.close()
        self.sock.shutdown(1)
        self.sock.close()

    def send_server(self, data):
        """ sends laser data over tcp socket to TPC DAQ. Returns True if successful and False if not. If server is not
        availabe or if pipe is broken the data is not send and function returns False. """

        self.printMsg("sending data to client")
        packed_data = data.pack()
        try:
            self.connection.sendall(packed_data)
            self.printDebug("sending data to " + str(self.client_address))
            self.printDebug("data sent: " + packed_data)
            return True

        except Exception as e:
            self.printError("Could not send data to client: " + str(e))
            return False

    def recv_server(self):
        """ Expects some data to be received """
        self.printMsg("waiting for data")
        try:
            self.printDebug("new data from" + str(self.client_address))
            data = self.connection.recv(60)
            self.printDebug("client received: " + str(data))
            self.laser_data.fill(self.laser_data.unpack(data))
        except:
            pass
        return self.laser_data

    def start_client(self):
        """ This starts a client connection to the specified address and port. """
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind(("", self.port_client))
        try:
            self.sock.connect(self.server_address)
        except Exception as e:
            self.printError("Could not connect to server: " + str(e))
            return False
        return True

    def stop_client(self):
        """ If your done with sending, call this function. """
        self.sock.shutdown(1)
        self.sock.close()

    def send_client(self, data):
        """ sends laser data over tcp socket to TPC DAQ. Returns True if successful and False if not. If server is not
        availabe or if pipe is broken the data is not send and function returns False. """

        self.printMsg("sending data to server")
        packed_data = data.pack()
        try:
            self.sock.sendall(packed_data)
            self.printDebug("sending data to " + str(self.server_address) + " from port: " + str(self.port_client))
            self.printDebug("data sent: " + packed_data)
            return True

        except Exception as e:
            self.printError("Could not send data to server: " + str(e))
            return False

    def recv_client(self):
        """ Expects some data to be received, if data has no length because nothing was in the buffer it raises an
            exception"""
        self.printMsg("waiting for data")
        self.printDebug("new data from " + str(self.server_address))
        data = self.sock.recv(60)
        if len(data) == 0:
            raise ValueError("Empty data string")

        self.printDebug("client received: " + str(data))
        self.laser_data.fill(self.laser_data.unpack(data))

        return self.laser_data
