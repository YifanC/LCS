#include <fstream> 
using namespace std;

struct laser_event {
      int laserid;            //which laser system: 1 or 2
      float status;	      //Container for ervice messages
      float pos_rot;          //Position Rotary Heidenhain Encoder 
      float pos_lin;          //Position Linear Heidenhain Encoder 
      float pos_att;          //Position Attenuator Watt Pilot 
      float pos_iris;         //Position Iris Standa 
      float time_sec;         //Epoche System Time of Laser Server
      float time_usec;        //Fraction to be added to the epoche time in us
      float count_trigger;    //Trigger Counter by Heidenhain Encoder
      float count_run;        //Run Counter of step in calibration run 
      float count_laser;      //Number of pulses shot with UV laser
      float pos_tomg_1_axis1; //Motorized Mirror Zaber T-OMG at box, axis 1
      float pos_tomg_1_axis2; //Motorized Mirror Zaber T-OMG at box, axis 2
      float pos_tomg_2_axis1; //Motorized Mirror Zaber T-OMG at flange, axis 1
      float pos_tomg_2_axis2; //Motorized Mirror Zaber T-OMG at flange, axis 2
     };


int main()
{
    laser_event my_laser_event;

      my_laser_event.laserid =1;            //which laser system: 1 or 2
      my_laser_event.pos_rot = 1234.0;          //Position Rotary Heidenhain Encoder 
      my_laser_event.pos_lin = 1234.1;          //Position Linear Heidenhain Encoder 
      my_laser_event.pos_att = 1234.2;          //Position Attenuator Watt Pilot 
      my_laser_event.pos_iris = 1234.3;         //Position Iris Standa 
      my_laser_event.time_sec = 123456789;             //System Time of Laser Server 
      my_laser_event.time_usec = -1;            
      my_laser_event.count_trigger = 12;    //Trigger Counter by Heidenhain Encoder
      my_laser_event.count_run = 12;        //Run Counter of step in calibration run 
      my_laser_event.count_laser = 1234;      //Number of pulses shot with UV laser
      my_laser_event.pos_tomg_1_axis1 = 1234.5; //Motorized Mirror Zaber T-OMG at box, axis 1
      my_laser_event.pos_tomg_1_axis2 = 1234.6; //Motorized Mirror Zaber T-OMG at box, axis 2
      my_laser_event.pos_tomg_2_axis1 = 1234.7; //Motorized Mirror Zaber T-OMG at flange, axis 1
      my_laser_event.pos_tomg_2_axis2 = 1234.8; //Motorized Mirror Zaber T-OMG at flange, axis 2

    ofstream myFile ("laser_struc.bin", ios::out | ios::binary);
    myFile.write ((char *) &my_laser_event, sizeof(my_laser_event));
    myFile.close();
}
