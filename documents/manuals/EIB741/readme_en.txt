DR. JOHANNES HEIDENHAIN GmbH
File: Readme_en.txt for EIB 741/742 driver CD
Date: 30.09.2013



EIB 741/742 Software
================

1. Contents
-----------

/EIB_741_742
   /doc
      /de                Documentation and license conditions in German
      /en                Documentation and license conditions in English

   /labview                 
      /examples          Examples
      /subvi             General VIs
      /typedefs          Type definitions for the VIs
      /VIs               LabView VIs for the EIB 741/742
      eib7_top.mnu       Function pallet of all VIs for the EIB 741/742
   
   /linux                   
      /examples          Examples
         /c_cpp          Examples for C and C++ programming languages
      /include           Header file for the functions of the EIB 741/742
      /lib               Functions library for the EIB 741/742 (Linux x32)
      /lib64             Functions library for the EIB 741/742 (Linux x64)
      /tools             Help programs for the EIB 741/742

   /windows
      /bin               DLL for the EIB 741/742 (Windows x32)
      /bin64             DLL for the EIB 741/742 (Windows x64)
      /examples          Examples
         /c_cpp          Examples for C and C++ programming languages
      /include           Header file for the functions of the EIB 741/742
      /lib               Library for the EIB 741/742 (Windows x32)
      /lib64             Library for the EIB 741/742 (Windows x64)
      /tools             Help programs for the EIB 741/742



2. Requirements
---------------

LabView:
	Version 8.0 or higher
		
Linux:
	Libraries: pthread
	The SO library was created with
	  - Kernel 3.1.0
	  - gcc 4.6.2
	  - libc 2.14.1

Windows:
	Operating system: Windows 2000, Windows XP, Windows Vista, Windows 7
	Libraries: user32.lib, ws2_32.lib, kernel32.lib



3. Initial Setup
----------------

Installing the software
-----------------------

As described in the manual, the driver files (eib7.dll/eib7_64.dll or 
libeib7.so/libeib7_64.so) must be copied from the CD to the system directory. 
After this the contents of the "EIB_741_742" folder on the CD must be copied to 
any directory on the local hard disk. The examples and tools can then be used.

Establishing a network connection to the EIB 741/742
------------------------------------------------
During initial setup, the EIB 741/742 must be connected to the computer via a 
point-to-point network connection and then configured. Before connecting the 
network cable to the computer, the following TCP/IP configuration must be set 
for the network card used:

- Use specific IP address (do not automatically procure)
- IP address:   192.168.1.1
- Subnet mask: 255.255.255.0

Once these settings have been entered, the network cable must be connected to 
the computer and the EIB 741/742, and the EIB 741/742 must be switched on. 
The EIB 741/742 is ready for operation as soon as the STATUS LED is lit.

Testing the connection
---------------------
It is recommended that you first test via a ping command (DOS console or 
Linux shell) whether the EIB 741/742 can be addressed over the network. This 
should be the case if all steps have been followed correctly.

Command:
ping 192.168.1.2

The console reports whether the EIB 741/742 can be addressed.
If it can, then the tools and example programs can be used. In some of these 
programs the IP address of the EIB 741/742 must be entered. The factory default 
setting for the EIB 741/742 is 192.168.1.2.

If the network parameters of the EIB 741/742 have already been changed, then the
EIB 741/742 cannot be addressed via the standard IP address (192.168.1.2). If 
this is the case, then booting with the standard network settings is 
recommended. (See Installation Instructions, “Reset Modes” section)

Configuration of the network parameters of the EIB 741/742
------------------------------------------------------
This is done with the “networksettings” tool (see below).
It is used to configure the EIB 741/742 for your network environment.

Running the examples and tools
------------------------------
The programs included for Windows and Linux are run directly by the 
console (shell). The LabView programs are available as VIs.



4. Examples
-----------

C/C++ examples for Windows and Linux
------------------------------------

AuxAxis:
--------
The position values of the encoder at axis 1 can be saved via two different 
trigger sources. The auxiliary axis is used for this purpose. 
An encoder with incremental signals must be connected to axis 1. The position
values are saved periodically via the internal timer trigger. At the same 
time, the position can be saved with a second, asynchronous trigger signal.

easy:
-----
Axis 1 of the EIB 741/742 is initialized for encoders with incremental signals. 
The position value and status word of this axis are requested and displayed. 
The IP address of the EIB 741/742 must be 192.168.1.2.

eib7info:
---------
The device data of the EIB 741/742 firmware is read. This includes the serial 
number and MAC address as well as the network parameters. All data is output 
to the console. The IP address of the EIB 741/742 is interrogated after the 
program has been started.

IntervalCounter:
----------------
The encoder at axis 1 generates trigger pulses depending on the current 
position. The position of axis 2 is saved and displayed via this trigger 
source.

MatchIncrEnDat:
---------------
The absolute and incremental positions of an EnDat01 encoder are read and 
displayed simultaneously.

polling:
--------
The position value and status word for an axis of the EIB 741/742 are read and 
displayed. The EIB 741/742 is operated in Polling mode for this. The user can 
select the axis whose position values are to be read. The program also 
interrogates the type of encoder interface. The user can choose between 
Incremental, EnDat 2.1 and EnDat 2.2. The position values are continually 
read in a loop. The IP address of the EIB 741/742 must be entered after the 
program has been started.

PulsCounter:
------------
The EIB 741/742 is operated in Recording mode and a selectable axis is configured
for incremental encoders. Triggering occurs via the internal timer trigger. 
The trigger pulses are started via a software trigger and are limited to a 
defined number by the PulsCounter. 

softrealtime:
-------------
The position value, status word, timestamp and trigger counter for an axis of 
the EIB 741/742 are read and displayed. The EIB 741/742 is operated in 
Soft Realtime mode for this. Data for four axes is transmitted. The user can 
select which of the four axes is to be displayed. 
Furthermore, the user can choose between external and internal triggering. 
The position values are continually read in a loop. The IP address of the EIB 
741/742 is interrogated after the program has been started. 

SoftrealtimeEnDat:
------------------
The position value, status word, timestamp and trigger counter for an axis of 
the EIB 741/742 are read and displayed. The EIB 741/742 is operated in 
Soft Realtime mode for this. The axis is configured for EnDat 2.2 encoders. 
The user can select which of the four axes is to be displayed. 
Furthermore, the user can choose between external and internal triggering. 
The position values are continually read in a loop. The IP address of the EIB 
741/742 is interrogated after the program has been started.

streaming:
----------
The position value, status word, timestamp and trigger counter for an axis of 
the EIB 741/742 are read and displayed. The EIB 741/742 is operated in 
Streaming mode for this. Data for four axes is transmitted. The user can select
which of the four axes is to be displayed. Furthermore, the user can choose 
between external and internal triggering. The position values are continually 
read in a loop. The IP address of the EIB 741/742 is interrogated after the 
program has been started.

recording:
----------
The position value, status word, timestamp and trigger counter for a 
selectable axis are recorded in the EIB 741/742. The EIB 741/742 is operated in 
Recording mode for this. The data package is configured such that only the 
data for the corresponding axis is recorded. After the recording phase, the 
data is transmitted to the PC and displayed. The IP address of the EIB 741/742 
is interrogated after the program has been started. The user can choose between 
external and internal triggering.

TriggerIO:
-----------
The position value, status word, timestamp and trigger counter for an 
axis of the EIB 741/742 are read and displayed. The EIB 741/742 is operated in 
Soft Realtime mode for this. All axes are configured for incremental 
encoders. The user can select which of the four axes is to be displayed. 
Triggering occurs via an internal timer trigger. 
In addition to this, the trigger signal is output at trigger output 1. The 
IP address of the EIB 741/742 is interrogated after the program has been started.

TriggerRI:
----------
The position value, status word, timestamp and trigger counter for a 
selectable axis of the EIB 741/742 are read and displayed. The EIB 741/742 is 
operated in Soft Realtime mode for this. Triggering is done via the reference 
mark of the encoder. The IP address of the EIB 741/742 is interrogated after 
the program has been started. 


LabView examples
-----------------

/EIB7Info:
----------

AuxAxis:
--------
The position values of the encoder at axis 1 can be saved via two different 
trigger sources. The auxiliary axis is used for this purpose. 
An encoder with incremental signals must be connected to axis 1. The position 
values are saved periodically via the internal timer trigger. At the same 
time, the position can be saved with a second, asynchronous trigger signal, 
Software Trigger 1. Both position values are displayed

/EIB7Info:
----------

GetConnInfo:
------------
The data of all currently open connections to the EIB 741/742 is output. This 
includes the IP address and port number for the TCP connections on both the 
EIB 741/742 and host.

GetNetworkSettings:
-------------------
The settings for the network interface of the EIB 741/742 are read and displayed.

SetNetworkSettings:
-------------------
The settings for the network interface of the EIB 741/742 can be changed. When 
the program is run, the settings are changed to those of the values entered 
in the entry fields. These take effect after the next reset.

/Polling:
---------

PollEncoderData:
----------------
The trigger counter, position value, status word and timestamp for an axis of 
the EIB 741/742 are output. The axis is configured for incremental encoders. 
One position value is interrogated each time the program is run. The 
EIB 741/742 is operated in Polling mode.

PollIncrEnDatPostition:
-----------------------
An axis of the EIB 741/742 is configured for EnDat 01. The EnDat and 
incremental position are read simultaneously. 

PollPosition:
-------------
The position values and status word for axes 1 to 4 are called cyclically. 
The axes can be configured individually for incremental encoders or for 
EnDat 2.1 or EnDat 2.2. The EIB 741/742 is operated in Polling mode.

/SoftRealtime:
--------------

SoftRealtime:
-------------
The position value, status word, timestamp and reference positions for 
axes 1 and 2 of the EIB 741/742 are read and displayed. The EIB 741/742 is 
operated in Soft Realtime mode for this. The user can choose between external 
and internal triggering. If internal triggering is selected, then the interval 
for the timer trigger can be set. The axes are configured for incremental 
encoders, and the position values are continuously read in a loop. 
Additionally, recording of the reference mark is activated at the start.

SoftRealtime_EnDat:
-------------------
Axis 1 is configured for an incremental encoder, and axis 2 is configured for 
an EnDat 2.2 encoder. In addition, two reference marks are stored for the 
incremental encoder, and their values are transmitted. 
Various pieces of additional EnDat information are polled cyclically from the 
EnDat encoder. The EIB 741/742 is operated in Soft Realtime mode. The user can 
choose between external and internal triggering. If internal triggering is 
selected, then the interval for the timer trigger can be set.

/Streaming:
-----------

Streaming:
----------
The position value and status word for axes 1 and 2 of the EIB 741/742 are read 
and displayed. The EIB 741/742 is operated in Streaming mode for this. The user 
can choose between external and internal triggering. 
If internal triggering is selected, then the interval for the timer trigger 
can be set. The axes are configured for incremental encoders, and the 
position values are continuously read in a loop. At the same time, the FIFO 
for buffering of data in the EIB is monitored, and its status is displayed.

/Recording:
-----------

Recording:
----------
The position value, status word and timestamp for axes 1 and 2 are recorded 
in the EIB 741/742. The EIB 741/742 is operated in Recording mode for this. 
The user can choose between external and internal triggering. If internal 
triggering is selected, then the interval for the timer trigger can be set. 
At the same time, the memory of the EIB 741/742 is monitored, and the number of 
samples already saved is shown. The user can end the recording phase. The 
data is then transmitted to the PC, and the curve of the position values is 
shown graphically in a diagram.

/Trigger:
---------

ExtTriggerOutput:
-----------------
The position value, the status word and the timestamp for axes 1 and 2 of the 
EIB 741/742 are read and displayed. The EIB 741/742 is operated in 
Soft Realtime mode for this. All axes are configured for incremental encoders. 
Triggering occurs via the internal timer trigger. In addition to this, the 
trigger signal of the internal timer is output at trigger output 1.

IntervalCounter:
----------------
The encoder at axis 1 generates trigger pulses depending on the current 
position. The position of axis 2 is saved and displayed via this trigger 
source. The trigger pulse interval can be set.

PulsCounter:
------------
The EIB 741/742 is operated in Recording mode and axes 1 and 2 are configured 
for incremental encoders. Triggering occurs via an internal timer trigger. The 
first trigger pulse is released by the software trigger and the number of 
trigger pulses is limited to a defined number by the PulsCounter. After 
measurement, the recorded position values are shown in a diagram.

RI_Trigger:
-----------
The position value, status word, timestamp and trigger counter for an axis of 
the EIB 741/742 are read and displayed. The EIB 741/742 is operated in 
Soft Realtime mode for this. Triggering is done via the reference mark of the 
encoder. 



5. Tools
--------

networksettings:
----------------
The settings for the network interface of the EIB 741/742 can be changed. This 
includes the IP address, network mask, standard gateway, the settings for 
the DHCP client and the host name.



6. Contact
----------

HEIDENHAIN
------------------------------------------
DR. JOHANNES HEIDENHAIN GmbH
Dr.-Johannes-Heidenhain-Straße 5
83301 Traunreut, Germany

Tel.:   +49 8669 31-0
E-mail: info@heidenhain.de
------------------------------------------
Measurement systems (EIB741/742 Support)
Tel.:   +49 8669 31–3104
E-mail: service.ms-support@heidenhain.de
------------------------------------------
www.heidenhain.de

