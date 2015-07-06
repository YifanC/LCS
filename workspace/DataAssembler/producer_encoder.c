 
#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <signal.h>

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

static int stop = 0;
void CtrlHandler(int sig)
{
   if(sig==SIGINT)
     stop = 1;
}


char BufferReply [2];


int main (void)
{
    struct LaserEvent my_laser_event;
    struct timeval SystemTime;

    struct EncoderInfo EncoderInfo;

   signal(SIGINT, CtrlHandler);
   signal(SIGTERM, CtrlHandler);

    EncoderInfo.ID = 2;
    EncoderInfo.Status = 0;

    my_laser_event.SystemTime_sec = -1;
    my_laser_event.SystemTime_usec = -1;
    my_laser_event.RotaryPosDeg = -1.;          //Position Rotary Heidenhain Encoder
    my_laser_event.LinearPosDeg = -1.0;          //Position Linear Heidenhain Encoder
    my_laser_event.TriggerCount = -1.0;      //Number of pulses shot with UV laser

	unsigned char BufferInfo[ sizeof (EncoderInfo) ];
	memcpy(&BufferInfo, &EncoderInfo, sizeof(EncoderInfo));

	unsigned char BufferData[ sizeof (my_laser_event) ];
	memcpy(&BufferData, &my_laser_event, sizeof(my_laser_event));

    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *encoder = zmq_socket (context, ZMQ_REQ);
    int rc = zmq_connect (encoder, "ipc:///tmp/laser-out.ipc");

    assert (rc == 0);

    printf("Sending Hello to assembler\n");

    zmq_send (encoder,&BufferInfo, sizeof(BufferInfo) , ZMQ_SNDMORE);
    zmq_send (encoder, &BufferData, sizeof(BufferData), 0);
	zmq_recv (encoder, BufferReply, 2, 0);

    nanosleep((struct timespec[]){{2, 0}}, NULL);


    printf ("Received:%s\n", BufferReply);


    int request_nbr = 0;

    while (!stop) {
        char buffer [10];

        gettimeofday (&SystemTime, NULL);

        my_laser_event.SystemTime_sec = (float) (SystemTime.tv_sec - 1431636031);
        my_laser_event.SystemTime_usec = (float) SystemTime.tv_usec;

        printf("Epoch Time[s]: %lu : %lu [us]\n",SystemTime.tv_sec,SystemTime.tv_usec);
        printf ("Sending Data Package: %d\n", request_nbr);

	    my_laser_event.TriggerCount = (float) request_nbr;

	    memcpy(&BufferInfo, &EncoderInfo, sizeof(EncoderInfo));
	    memcpy(&BufferData, &my_laser_event, sizeof(my_laser_event));

        zmq_send (encoder,&BufferInfo, sizeof(BufferInfo) , ZMQ_SNDMORE);
        zmq_send (encoder, &BufferData, sizeof(BufferData), 0);

        zmq_recv (encoder, buffer, 2, 0);

        if (strcmp(buffer,"OK") == 0) {
            printf ("Received: %s\n", buffer);
        }
        else if (strcmp(buffer,"XX") == 0) {
            printf ("Received: %s, will end now.", buffer);
            stop = 1;
        }
        else {
            printf ("Received: %s did not understand reply. Aborting...", buffer);
            stop = 1;
        }
        request_nbr += 1;
        nanosleep((struct timespec[]){{0, 500000000}}, NULL);
    }

    printf ("Shutting down.");

    zmq_close (encoder);
    zmq_ctx_destroy (context);
}
