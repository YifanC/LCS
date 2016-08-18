import zmq
import struct
from base.base import *
from services.data import LaserData

# TODO: Split classes into files
# TODO: Implement shut down of zmq messaging

class Communication(Base):
    ID = {0: "Assembler",
          1: "RunControl",
          2: "Encoder"}
    ID_RUNCONTROL = 1
    ID_ENCODER = 2

    def __init__(self, name):
        self.name = name
        self.id = -99
        self.state = 0

        self.context =zmq.Context()

    def start(self, channel):
        """ bind the socket to some communication channel, default will use an ipc socket """
        self.socket.connect(channel)

    def stop(self):
        self.printMsg("Shutting down communication")
        self.socket.close()
        self.context.term()

    def printMessage(self, id, state):
        self.printMsg("zmq message header: id: " + str(id) + " (" + Communication.ID[id] + ")" + " state: " + str(state))

class broker(Communication):
    def __init__(self):
        # Prepare our context and sockets
        name = "broker"

        super(broker, self).__init__(name=name)

        self.context = zmq.Context()
        self.frontend = self.context.socket(zmq.ROUTER)
        self.backend = self.context.socket(zmq.DEALER)
        self.frontend.bind("ipc:///tmp/laser-in.ipc")
        self.backend.bind("ipc:///tmp/laser-out.ipc")

        # Initialize poll set
        self.poller = zmq.Poller()
        self.poller.register(self.frontend, zmq.POLLIN)
        self.poller.register(self.backend, zmq.POLLIN)



        # exchange messages between sockets
        while True:
            socks = dict(self.poller.poll())

            if socks.get(self.frontend) == zmq.POLLIN:
                message = self.frontend.recv_multipart()
                self.backend.send_multipart(message)

            if socks.get(self.backend) == zmq.POLLIN:
                message = self.backend.recv_multipart()
                self.frontend.send_multipart(message)


# ------------------------------ CONSUMER ------------------------------------

class Consumer(Communication):
    def __init__(self, name):
        super(Communication, self).__init__(name=name)
        super(Consumer, self).__init__(name=name)
        self.id = -99
        self.name = name
        #self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REP)

        self.timeout = 600 # time we wait for hello message
        self.encoder_alive = False
        self.runcontrol_alive = False



    def start(self, channel=""):
        """ bind the socket to some communication channel, default will use an ipc socket """
        # start up serve
        if channel is "":
            self.socket.connect("ipc:///tmp/laser-out.ipc")
        else:
            self.socket.connect(channel)

    # TODO: Change poller to timer. This is always resetting when a new message is recieved from anywhere
    def recv_hello(self):
        hello_data = LaserData()

        poller = zmq.Poller()
        poller.register(self.socket, zmq.POLLIN)

        if poller.poll(10000000):
            self.printMsg("Waiting for Hello")
            [reply_id, reply_state] = self.recv(hello_data)

            if DEBUG:
                self.printMessage(reply_id, reply_state)

            if reply_state == -1:
                return reply_id

            else:
                self.printMsg("Msg was not hello")
                return -1
        else:
            self.printError("ran out of time for hello messages")
            sys.exit(-1)

    def recv_hellos(self):
        """ We require that one encoder and one run control is alive """
        self.printMsg("waiting for hello")
        reply_id = self.recv_hello()

        if reply_id == Communication.ID_ENCODER:
            self.encoder_alive = True
        elif reply_id == Communication.ID_RUNCONTROL:
            self.runcontrol_alive = True
        else:
            self.printError("Did not collect hello from everyone yet. (" + str(reply_id) + ")")

        if (self.encoder_alive is True) and (self.runcontrol_alive is True):
            return True

    def recv(self, data):
        info_string, data_string = self.socket.recv_multipart()
        self.send_ack()
        [ID, STATE] = self.unpack_info(info_string)

        if ID == self.ID_ENCODER:  # data from run control
            self.printMsg("Received data from encoder")
            self.unpack_encoder(data_string, data)

            # Something here is very strange: Serialization seems to be different from c and python. Floats and ints
            # work, but everythin else ends up crookend: So in some time we will have a problem here!
            data.trigger_time_sec += 1471533965
        elif ID == self.ID_RUNCONTROL:  # data from encoder
            self.printMsg("Received data from run control")
            self.unpack_runcontrol(data_string, data)
        else:
            self.printError("Exiting: Message from unidentified client received (ID was " + str(ID) + ")")
            sys.exit(-1)

        return [ID, STATE]

    def send_ack(self):
        self.socket.send(b"OK")

    def unpack_encoder(self, data_string, container):
        container.trigger_time_sec, container.trigger_time_usec, container.pos_rot, container.pos_lin, \
        container.count_trigger = struct.unpack('f' * 2 + 'f' * 3, data_string)


    def unpack_runcontrol(self, data_string, container):
        (container.laserid, container.pos_att, container.pos_iris, container.count_run, container.count_run,
         container.pos_tomg_1_axis1, container.pos_tomg_1_axis2, container.pos_tomg_2_axis1, container.pos_tomg_2_axis2) \
            = struct.unpack('i' + 'f' * 8, data_string)
    def unpack_info(self, info_string):
        [id, state] = struct.unpack('ii', info_string)  #[0]
        return [id, state]

# ------------------------------ PRODUCER ------------------------------------

class Producer(Communication):

    def __init__(self, name):
        super(Communication, self).__init__(name=name)
        super(Producer, self).__init__(name=name)
        self.name = name
        #self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)
        self.color = True

        self.id = -99

    def start(self):
        self.socket.connect("ipc:///tmp/laser-in.ipc")

    def send_hello(self):
        """ Send hello message to consumer. The idea is:
            1. Set self.state = -1
            2. send complete message frame to consumer"""
        self.printMsg("Sending Hello")
        hello_data = LaserData()
        recieved = self.send_data(hello_data)

        return recieved

    def pack_encoder(self, data_msg):
        """ serializes the variable according to the communication definitions """
        info_string = struct.pack('i' * 2, self.id,  self.state)

        data_string = struct.pack('f' * 2 + 'f' * 3, data_msg.trigger_time_sec, data_msg.trigger_time_usec,
                                 data_msg.pos_rot, data_msg.pos_lin, data_msg.count_trigger)

        return [info_string, data_string]

    def pack_runcontrol(self, data_msg):
        """ serializes the variable according to the communication definitions """
        info_string = struct.pack('i' * 2, self.id,  self.state)

        data_string = struct.pack('i' + 'f' * 8, data_msg.laserid, data_msg.pos_att, data_msg.pos_iris,
                                 data_msg.count_run, data_msg.count_run, data_msg.pos_tomg_1_axis1,
                                 data_msg.pos_tomg_1_axis2, data_msg.pos_tomg_2_axis1, data_msg.pos_tomg_2_axis2)

        return [info_string, data_string]

    def send_data(self, data):
        self.printMsg("Sending Data")
        if self.name == "encoder":
            self.socket.send_multipart(self.pack_encoder(data))

        elif self.name == "runcontrol":
            self.socket.send_multipart(self.pack_runcontrol(data))
        else:
            self.printError("did not recognize name of sender")
        message = self.socket.recv()

        self.printMsg(message)
        if message == "Ok":
            return True
        else:
            return False


    def recv_ack(self):
        pass
        self.printMsg("Control Message: ID=" + str(self.ID) + " Status=" + str(self.Status))
