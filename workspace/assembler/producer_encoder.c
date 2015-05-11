 
#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

unsigned int ID = 2;


struct laser_event {
      float pos_rot;          //Position Rotary Heidenhain Encoder 
      float pos_lin;          //Position Linear Heidenhain Encoder 
      float count_trigger;    //Trigger Counter by Heidenhain Encoder
     };




int SerializeLaserData(struct laser_event *P_laser_event, unsigned char *P_Buffer ){


}


int main (void)
{
    struct laser_event my_laser_event;
      
    my_laser_event.pos_rot = 11.0;          //Position Rotary Heidenhain Encoder 
    my_laser_event.pos_lin = 12.0;          //Position Linear Heidenhain Encoder 
    my_laser_event.count_trigger = 100000.00;      //Number of pulses shot with UV laser

	unsigned char bufferID[ sizeof (ID) ];
	memcpy(&bufferID, &ID, sizeof(ID));

	unsigned char data[ sizeof (my_laser_event) ];
	memcpy(&data, &my_laser_event, sizeof(my_laser_event));


    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *encoder = zmq_socket (context, ZMQ_REQ);
    int rc = zmq_connect (encoder, "ipc:///tmp/feed-laser.ipc");
    assert (rc == 0);

    int request_nbr;
    for (request_nbr = 0; request_nbr != 10; request_nbr++) {
        char buffer [2];
        printf ("Sending Hello %d...\n", request_nbr);
        zmq_send (encoder,&bufferID, sizeof(bufferID) , ZMQ_SNDMORE);
        zmq_send (encoder, &data, sizeof(data), 0);
        
        zmq_recv (encoder, buffer, 2, 0);
        printf ("Received World %d\n", request_nbr);
    }
    zmq_close (encoder);
    zmq_ctx_destroy (context);
    return 0;
}
