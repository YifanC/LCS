__author__ = 'matthias'

import socket
import sys

from services.data import *
from base.base import *


class TCP(object):
    def __init__(self, server_name="localhost", port=10000):
        """ Initialize data """

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
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect(self.server_address)
        self.laser_data = data
        packed_data = self.laser_data.pack()
        try:
            message = packed_data
            print >> sys.stderr, 'sending "%s"' % message
            self.sock.sendall(message)

            amount_received = 0
            amount_expected = len("OK")
            while amount_received < amount_expected:
                data = self.sock.recv(4)
                amount_received += len(data)
                print >> sys.stderr, 'received "%s"' % data

        finally:
            self.sock.close()

        self.sock.close()

