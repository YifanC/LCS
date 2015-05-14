 
#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

// includes for timing
#include <sys/time.h>
#include <time.h>

struct LaserEvent {
	float SystemTime_sec;		// epoch time in seconds
    float SystemTime_usec;		// This is the rest of the elapsed time
	float RotaryPosDeg;          //Position Rotary Encoder
	float LinearPosDeg;          //Position Linear Encoder
	float TriggerCount;    //Trigger Counter by Heidenhain Encoder
};

struct EncoderInfo {
    unsigned int ID;
    unsigned int Status;
};

int main (void)
{
    struct LaserEvent my_laser_event;
    struct timeval SystemTime;

    struct EncoderInfo EncoderInfo;

    EncoderInfo.ID = 2;
    EncoderInfo.Status = -88;

    my_laser_event.SystemTime_sec = -1;
    my_laser_event.SystemTime_usec = -1;
    my_laser_event.RotaryPosDeg = 11.0;          //Position Rotary Heidenhain Encoder
    my_laser_event.LinearPosDeg = 12.0;          //Position Linear Heidenhain Encoder
    my_laser_event.TriggerCount = 100000.00;      //Number of pulses shot with UV laser

	unsigned char bufferInfo[ sizeof (EncoderInfo) ];
	memcpy(&bufferInfo, &EncoderInfo, sizeof(EncoderInfo));

	unsigned char data[ sizeof (my_laser_event) ];
	memcpy(&data, &my_laser_event, sizeof(my_laser_event));


    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *encoder = zmq_socket (context, ZMQ_REQ);
    int rc = zmq_connect (encoder, "ipc:///tmp/feed-laser.ipc");
    assert (rc == 0);

    int request_nbr;
    for (request_nbr = 0; request_nbr != 100; request_nbr++) {
        gettimeofday (&SystemTime, NULL);

        my_laser_event.SystemTime_sec = (float) (SystemTime.tv_sec - 1431636031);
        my_laser_event.SystemTime_usec = (float) SystemTime.tv_usec;

        printf("Epoch Time[s]: %f : %f [us]\n",my_laser_event.SystemTime_sec,my_laser_event.SystemTime_usec);
        printf("Epoch Time[s]: %lu : %lu [us]\n",SystemTime.tv_sec,SystemTime.tv_usec);


        char buffer [2];
        printf ("Sending Hello %d...\n", request_nbr);

	    memcpy(&data, &my_laser_event, sizeof(my_laser_event));
        zmq_send (encoder,&bufferInfo, sizeof(bufferInfo) , ZMQ_SNDMORE);
        zmq_send (encoder, &data, sizeof(data), 0);
        
        zmq_recv (encoder, buffer, 2, 0);
        printf ("Received World %d\n", request_nbr);
        nanosleep((struct timespec[]){{0, 500000000}}, NULL);
    }
    zmq_close (encoder);
    zmq_ctx_destroy (context);
    return 0;
}
