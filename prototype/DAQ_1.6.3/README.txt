Version 1.6.2 is built on version 1.6. 
- Propper Majority Mode support (for more info see manual p40):
 + Corrected Triggering Enable Mask
 + Added init procedure for Majority mode (threshold, n_over_threshold)
 + Added Button: "Majority Mode Threshold" to set the value 

Version 1.6.3 is built on version 1.6.2
- Add Laser positoning support:
 + LHEP-GUI: change button from photodiode to laser position
 + daq_main.c: add laser init if button is on
 + added daq_laser.c which contains the functions for the positioning
 + after every trigger the position is read from the position encoders via ethernet.
   The saving of the position is triggered externally. The values are displayed in the console.
 + daq_writer.c: added saving of the two encoder (linear, rotary) values: Encoder Status, 
   Trigger Stamp, Time Stamp, Position into the raw file. This info is located (in the raw file)
   at the beginning of every event.
