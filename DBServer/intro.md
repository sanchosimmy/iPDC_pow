iPDC suit consists of 3 different modules, iPDC - Phasor Data
Concentrator. It can collect data from Phasor Measurement Units, and other
commercial grade PDC that are IEEE C37.118 standard compliant. PMUSimulator -
Phasor Measurement Unit Simulator and DBServer – Archive received data in a
database i.e., act as a historian. 
The basic working of the program is as follows
\image html basic_pmu_full.png width=500px
Synchrophasors applications are deployed at many places in the world. This project extends the existing WAMS infrastructure that measures phasor data to also obtain instantaneous values in a time synchronous manner over a wide area. Extensions are made by utilizing user definable fields in command frame and data frames of synchrophasor standard C37.118 2011 part 2.The following changes were made to the code.

The high resolution instantaneous values data transmission functionality was
implemented on the iPDC. It was observed that while the PDC was
collecting instantaneous values data frames, its processing of PMU synchrophasor
data frames were not interrupted/disrupted.

The complete process of automated receiving of instantaneous values data using
little extension of existing WAMS infrastructure is shown in the following figures.

\li Event triggered \image html 1.png width=500px
\li PMUs to send instantaneous values frames\image html 2.png width=500px
\li Collecting and storing instantaneous values frames \image html 3.png width=500px

\section INTRODUCTION
Wide area monitoring system (WAMS) is now a mature technology and is being
increasingly adopted in transmission networks worldwide. It provides synchronous measurements
of phasors, analog quantities and digital signals at data rates of typically
once per power frequency cycle. This translates to 50 or 60 frames per second
depending on the system frequency where Phasor Measurement Units (PMUs) are
installed. The phasors can be estimated from directly measured quantities like
three-phase voltages and currents or derived quantities like positive, negative
and zero sequence components. This arrangement works fine when the main purpose
is system monitoring. Applications like state estimations, oscillation mode monitoring, line parameters estimations and instrument transformer calibrations
etc. are successfully deployed using phasor measurements.

The current version of the IEEE Std. C37.118.1 requires the
calculation of synchrophasors within defined accuracy limits in steady state
conditions. During transient conditions, the accuracy limits of steady state do
not apply. The standard only defines length of the time up to which the
transient periods can last after step changes in phase, magnitude and frequency.
It is possible that PMUs of different vendors report different values of
synchrophasors during transient conditions.

Calculation of phasor makes assumptions regarding the wave shapes of the input
signals. All information in the signal except fundamental frequency component is
lost in the process of phasor calculation. In many situations line the analysis
of a fault event and investigating protection system performance, it is
important to get the actual wave shapes of the signals rather than just their
phasor values.

Digital signals like circuit breaker positions can change within a fraction of a
cycle, hence the time resolution of one cycle provided in the existing
infrastructure is not so useful in determining the sequence of events on a
wide-area basis. Local substation-wide sequence of events are generally
available through disturbance records of protection system. However, determining
the system-wide sequence of event development for analysing a system wide event
is a challenging task. 


Usually instantaneous samples of all electrical quantities i.e., voltages and
currents measured at transmission network are already sampled in PMUs using
synchronous sampling as it is a necessary step in calculation of synchronous
phasors. However, transmitting instantaneous values continuously in time is a
costly proposition with respect to communication bandwidth, storage capacity and
computational resources. 

From power system point of view, it serves little purpose to get these
measurements in near real time from sensors deployed over a wide area. The
latency involved in obtaining data from wide area to central location make is
useless for most of equipment protection purposes. The slower wide area control
applications do not require instantaneous point on wave values, as they
generally use phasor values.

On the other hand, during power system faults or a power system events like
opening or closing of a transmission line, transformer or synchronizing a
generator, it will be of great help if synchronously sampled instantaneous values
of these measurements are available for the post-disturbance analysis. These
measurements need not be real time or near real time. A delay of few seconds
between the events and data availability at a central location is tolerable.

This project is a midway solution that gives all the advantages of high
resolution measurements, and at the same time does not put much stress on the
communication and computational infrastructure. We also leverage most of the
existing technologies to achieve these objectives with little bit of extension
of their capabilities. The project allows us to get the synchronously
measured instantaneous data with consistent naming and other associated
meta-data along with digital signals measured over a wide area at a central
location in as seamless manner, and present it for post event analysis in case of
a fault or any other power system event. 

\section Extensions_of_PMU_capabilities

The first step is to extend the computation capabilities of PMU. The
capabilities have to be extended to also store the instantaneous
samples that were used for calculating phasors. These samples are
captured in a synchronous manner for all channels and stored in a circular
buffer as First in First out (FIFO) manner for a duration of about a few tens of
seconds. The duration of data that is stored in the buffer can be configurable. In
practice, a duration about 30 seconds is sufficient. The sampling frequency for
instantaneous samples is usually much higher in range of 20 to 80 samples per
power frequency cycle, which is compatible with the requirements of high
resolution point on wave data. 

The second step is to have the capability to receive a signal from Phasor Data
Concentrator (PDC) that commands to send instantaneous point on wave
sampled values. Upon receiving the command a PMU make
a COMTRADE file as per C37.111-2013 binary format. This file
contains instantaneous values for time 30 second prior and 30 second later to
the time stamp of the command frame. Hence, the data stored in the buffer is
used to get the instantaneous values of signal prior to the command and data is
collected for further 30 second time after the command is received. So total
duration of data in a single COMTRADE file is 60 seconds. 

IEEE Std C37.111-2013 defines the formats for files for storing
data generated by various power systems or power system models. This common
format is called Common Format for Transient Data Exchange (COMTRADE).  Each
COMTRADE record can have up to four associated files. viz, header, configuration,
data and information. All four files have the same name with different
extensions to indicate the type of file. All the four files can also be combined
as a single file with an extension CFF. Here we utilize only configuration 
and data files of COMTRADE. The channel names in configuration file of COMTRADE
files may be taken from channel names used in CFG frame of the PMU or there can
be a facility to define unique names for this purpose. 

Generally float32 data type in primary values can be selected in the
data file of COMTRADE to use the full resolution available in the standard, as
there can be a wide range of data when handling instantaneous values of signals.
This also avoids the requirements of determining maximum and minimum value of
each signal to be added in the COMTRADE configuration file. 

The timestamps of data samples need to be synchronized as is the case with PMU
frames.

Finally, the PMU must be able to send this COMTRADE file as a new frame which is
extension to C37_118_2. There are two new frames required, one for
COMTRADE configuration file and the other for COMTRADE data file. Both the
frames must have the same timestamp to indicate an association between them. CHK
bits are also attached at the end of these frames to ensure verification of data
integrity at PDC end. These frames are sent to PDC following the usual framework
of sending DATA frame. Another important feature that must be supported
by PMU is that the instantaneous value COMTRADE data frames should be sent with
a random delay of few seconds. A PMU should be constrained to respond to just
one command for instantaneous point on wave data request at a time. It should
ignore or not respond to a new request until data for the active command is
sent.

The implementation of these extensions must be such that the usual functioning of
PMU i.e, responding to normal commands from PDC and sending data frames at the
regular data frame rate is not disturbed. 

\section Extensions_of_PDC_capabilities

Some changes are also required at the PDC side too. The first change in PDC is to
have the ability to send a new command frame that indicates a command to the PMU
to send instantaneous point on wave data. The second change is to have the ability
to receive two new types of data frames that contain COMTRADE data and
configuration files sent by PMU. 

The PDC can be programmed to send a command for instantaneous values when it
receives an trigger event in data frames of any one of the PMU. From the point of
view of power system events and to get best utilization of the new features it
is required that the command for instantaneous values is sent to all PMUs reporting
to the PDC or few PMUs in the neighbourhood of the PMU that triggered the event.
This ensures that 60 second synchronized instantaneous point on wave data is
available system-wide.   

It is generally possible that for a given power system event, more than one PMU
send an event trigger. Hence, the PDC should respond to just one of the trigger and
ignore rest of the triggers for next 30 seconds. If any trigger is present beyond
that time, a new command for instantaneous values is sent to all PMUs. Hence,
there is always at least 30 second time difference between two commands for
instantaneous values. This avoids overwhelming the communication network and
PMUs with frequent requests of instantaneous values. 

After sending a command for instantaneous values, the PDC must await to receive
the responses from PMUs for COMTRADE files. Once a frame is received, it is
parsed and the trigger time is extracted from the configuration file. This time is
matched with the timestamp of the command that was sent earlier. This step
ensures the association between the command from PDC and the response from the
PMU. The received COMTRADE file is stored at appropriate location with a unique
identity derived from PMUID, STN and timestamp of the command
i.e., SOC and FRACSEC. A wait time of few minutes can be
configured for which the PDC will wait to receive COMTRADE files from PMUs.
Further data parsing is not performed after the wait time is expired.  The CRC
bits are also verified at PDC ends and only valid files are stored.   

It can be programmed that PMUs send these files in TCP protocol so as to ensure
confirmed delivery to PDC. Even though all PMUs would send the COMTRADE data
frames, they would be received in a time spread manner as each PMU is programmed
to have a random delay while sending the data. Hence, there is moderate stress
on communication bandwidth. The normal working of PDC is not disturbed while it
receives the COMTRADE files.

In this way system-wide high resolution instantaneous point on wave data is
collected at PDC end for any event of power system network. The data is received
within few minutes of the event, which is compatible with this type of data as
it is mostly used for post processing and human consumption.

The next section gives details of exact extensions done to the standard C37_118_2. In most of the places, the facility of user definable bits
provided in the standard is used.

\section Extensions_in_IEEE_Std_C37_118_2_2011 
IEEE Standard C37.118.2-2011 is the standard for Synchrophasor Data Transfer for
Power Systems. The standard defines the methods of real time exchange of
synchronized measurement data between PMU, PDC, and other applications using any
suitable communication protocol.

The standard defines

\li Types of messages, its contents and use 
\li Data types and formats 
\li Communication options and requirements 

This standard enables exchange of data among measurement, data
collection, and application equipment. It defines a simple, defined,
open access method for data transmission and storage within a phasor
measurement system.

The PMUs communicate with PDCs or other higher level devices using four types of
frames defined in the standard. Data frame, Configuration frame 1,2 and 3,
Header frame and Command frame. The basic communication flow is as shown here
\image html basic_pmu_full.png width=500px

The following additions to the standard are proposed to enable the disturbance
recorder functionality.  

\subsection Instantaneous_values_frames 
Two new frames have been defined as an extension to the existing standard.
Instantaneous values configuration frame and instantaneous values data frame. 

Bits 6-4 of the SYNC word in the frames is used to distinguish between
various kinds of frames. Some bit combinations specified in the table below
are defined in the standard to indicate frame types. A new frame type of
configuration frame 3 was added in 2011 version of the standard C37_118_2, it was not
present in the 2005 version. In the same way 110 and 111 can be used to indicate
that the frame is an instantaneous values configuration frame and instantaneous
values data frame respectively
\li Bits 6-4 in the SYNC word of the frame          
| Bits 6–4 	|       Frame type      	|
|:--------:	|:---------------------:	|
|    000    	|       Data Frame      	|
|    001    	|      Header Frame     	|
|    010    	| Configuration Frame 1 	|
|    011    	| Configuration Frame 2 	|
|    101   	| Configuration Frame 3 	|
|    100   	|     Command Frame     	|

The COMTRADE files created by PMU in response to the command for instantaneous
values are embedded in the new frames. Except for SYNC word bits 6-4, the other
14 bytes at the beginning of the frame are as per the word definitions common to
all frame types mentioned in Table-2 of C37_118_2. The last two bytes are
also calculated from the rest of the frame and added as CHK bytes. The body of
frames is formed by the contents of COMTRADE configuration and data files
respectively. 


\subsubsection Instantaneous_values_configuration_frame 
The COMTRADE configuration file is embedded in this frame. This
frame is similar to the CFG-2 frame. Bits 6-4 in the SYNC word is
110 to indicate that its a DR  configuration frame.
\subsubsection Instantaneous_values_data_frame 
Bits 6-4 in the SYNC word is 111 to indicate that its a instantaneous values
data frame. The content of COMTRADE files include time stamps in SOC and FRACSEC
for all instantaneous samples of all channels given, instantaneous samples of
all digital channels and various internally generated binary signals to indicate
status of PMU and time synchronizing, similar to that given in data frames in
C37_118_2. Due to the large size of the  data file, the .DAT file is
split into parts with size less than 65535 bytes and each part is embedded and
sent in a frame that is similar to configuration frame 3. The CONT_ID of the
last file is to be set as 65535 to indicate that its the last part of the file.
Sufficient delay is to be given before the last file is sent to ensure that all
other parts have been received at the receiving end before the last part is
received, as there is no indication of the total numbers of parts of instantaneous
values data frame to be expected from PMU.

The size of the instantaneous values configuration file is not large but
the instantaneous values data file may be substantially higher, in the order of
megabytes. The instantaneous values data file has to be split into smaller files
before it is sent and has to reassembled at the receiving end. Since a large
number of files are sent and the guaranteed arrival of all the parts is
essential for the integrity of the whole file, this can be reliably transmitted
over TCP. On the receiving end, the file has to be reassembled using the
CONT_ID.

\subsection Command_Frames 
A new command frame to request all connected PMUs to send instantaneous values
frames have been defined. Bits 15-0 if the command frame are used to identify
different command frames.  

\li Commands sent to the PMU/PDC
|         Bits 15–0        	| Command word bits Definition               	|
|:------------------------:	|--------------------------------------------	|
|     0000 0000 0000 0001  	|  Turn off transmission of data frames.     	|
|     0000 0000 0000 0010  	|  Turn on transmission of data frames.      	|
|     0000 0000 0000 0011  	|  Send HDR frame.                           	|
|     0000 0000 0000 0100  	|  Send CFG-1 frame.                         	|
|     0000 0000 0000 0101  	|  Send CFG-2 frame.                         	|
|     0000 0000 0000 0110  	|  Send CFG-3 frame (optional command).      	|
|     0000 0000 0000 1000  	|  Extended frame.                           	|
|     0000 0000 xxxx xxxx  	|  All undesignated codes reserved.          	|
|     0000 yyyy xxxx xxxx  	|  yyyy != 0 available for user designation. 	|
|     zzzz xxxx xxxx xxxx  	|  zzzz !=0 reserved.                        	|

As mentioned in the table, 0000 yyyy xxxx xxxx where yyyy != 0
 is available for user designation. 0000 0001 0000 0000 has been used as the
command to be sent to all PMUs by the PDC to request the
instantaneous values frame.

\subsection STAT_word_in_the data_frame
A user defined trigger, apart from other usual triggers, can be set from PMU
side whenever a PMU observes any power system event for which wide area
disturbance record would be useful. When such an event occurs, Bit 11 of the
STAT word in the data frame i.e., PMU Trigger pick-up is set to one to indicate
that a trigger condition has been detected for PMUs that have trigger
capability. The bit shall be set for a mandatory set period of at least one data
frame or one second, whichever is longer. It may remain set as long as the
trigger condition is detected or may be cleared after this mandatory set period
to allow for detection of other triggers. 

\li STAT word of DATA frame 
|   Bits 03–00  	| Trigger reason                	| Bits 03–00 	| Trigger reason          	|
|:-------------:	|-------------------------------	|:----------:	|-------------------------	|
|      111      	|  Digital                      	|     110    	|  Reserved               	|
|      101      	|  df/dt High                   	|     100    	|  Frequency high or low  	|
|      011      	|  Phase angle diff             	|     010     	|  Magnitude high         	|
|      001       	|  Magnitude low                	|     000     	|  Manual                 	|
|     1111–1000 	| Available for user definition 	|            	|                         	|

Also Bits 0–3 of the STAT word in the data frame, trigger reason, a 4-bit code
indicating the initial cause of a trigger is to be set. 

Bits 1111–1000 are available for user definition. Any values
within this range can be used to indicate that a power system event for which
wide area disturbance record would be useful has occurred. Bits
0–3 can be set as 1001 to indicate that a power system event for which wide area
disturbance record would be useful has occurred.

All the  extensions described in the sections could also be achieved by using
the extended frame data in command frame according to Table 14 of C37_118_2.
However, it is to be noted that we have not used the extended frame facility.
This is to maintain the convention that it is only PDC that sends commands to
PMUs. The PMUs are generally acting as a devices that responds to commands by
giving data frames and configuration frames as required by PDC. 

\section Summary_and_Conclusion
For high resolution data functionality, it is proposed that PMUs have extended
capability to maintain a buffer of the instantaneous samples that were used
for calculating phasors. Extended PMUs are able to accept a command frame, sent
by a PDC, to execute a command to send instantaneous values data. Upon receiving
the command, PMUs make a COMTRADE files. This file includes instantaneous values for
time 30 seconds prior and 30 seconds later to the time stamp of the command frame.
Extended PDCs accept the instantaneous values, configuration and data frames and
store them.

The importance of such a system was felt in events like a partial black out
experienced in the city of Mumbai in India in month of October 2020. One of the
main triggers events was the overvoltage observed at a 400 kV substations. The
disturbance records of only a few of the relays were available, as event
recording due to overvoltage was not configured in all relays. If high
resolution synchronous data were available from all PMUs it would have been of
great help in post event analysis. 

The system proposed in this paper results in a streamlined
synchronous wide area disturbance record of an event in quick time. It is
necessary that the proposed mechanism is followed by all PMUs and PDCs in the
WAMS system, hence an update in the existing communication standard is sought. 

General Information
-------------------

	iPDC consists of 3 different modules, 

          iPDC - Phasor Data Concentrator compliant with IEEEC37.118 synchrophasor standard, 
          DBServer - which is used to store the data in MySQL database, and 
          PMUSimulator - which simulates the Phasor Measurement Unit compliant with IEEEC37.118 synchrophasor standard.

	This version works in Unix and Unix-like operating systems. It uses GCC compiler, GTK+ graphical user interface library & also POSIX thread library, and MySQL as database server. 


Dependencies
-------------

	gcc
	mysql-server-5.0
	mysql-client-5.0.
	mysql-common
	libmysqlclient16*
	GTK+2.5
	GTK 3
	glade 3.6.7 (optional for development)
	NTP-client  (For synchronization of system clock)


INSTALLATION:
------------

     Dependencies installation: Installation of glade internally installed the GTK+ libraries and there is no need to explicit installation of GTK+. NTP is recommended for better results by iPDC and PMU Simulator.

          su/sudo apt-get update (recommended)
          su/sudo apt-get install mysql-server-5.0 (or latest available version)
          su/sudo apt-get install mysql-client-5.0 (or latest available version)
          su/sudo apt-get install mysql-common
          su/sudo apt-get install libmysqlclient16 (or latest available version)
          su/sudo apt-get install libmysqlclient16-dev (or latest available version)
          su/sudo apt-get install glade
          su/sudo apt-get install ntp (http://howto.eyeoncomputers.com/ubuntu/install-ntp/)

     For detailed explanation of iPDC, DBServer, and PMUSimulator working refer the User Manual. Available on website. Make sure you have done with the dependency installation before go for installation of iPDC software.

     A) iPDC Installation -> 

  		Multiple instance can be run on same machine by ONLY one time installation.
          First go to the source folder on terminal then,

	          1. Extract iPDC.x.y.tar.gz.
	          2. cd iPDC.x.y/iPDC.x
	          3. make install 
		  4. Run iPDC via terminal or you could found its shortcut on menu named iPDC.


     B) Install DBServer ->

          DBServer can be installed on the same machine or a different machine.
          If DBServer is to be installed on a different machine then copy iPDC.x.y/DBServer to that machine(cp iPDC.x.y/DBServer /PATH....). Install dependencies like demysql-server-5.0, mysql-common, libmysqlclient15off, mysql-client-5.0. on that machine and Then follow the steps given below.

	          1. cd /PATH/iPDC.x.y/DBServer.x.y
	          2. mysql -u root -p <"Db.sql"
	          3. make
		  4. ./DBServer or DBServer (accesseble only from folder)

     C) Install PMUSimulator ->

          If you need to run simulator on different from iPDC machine then Copy the PMU Simulator on that machine.
          Single installation of PMU Simulator will work in case you need to simulate multiple PMUSimulator on the same machine.

	          1. cd /PATH/iPDC.x.y/PMUSimulator.x.y
	          2. make install 
		  3. Run - PMU via terminal or you could found its shortcut on menu name PMU Simulator.


USAGE
------

     i) When iPDC acts as a Client ->

	     1. Run DBServer on terminal of the machine where DBServer is installed. This will be listening on port 9000 for data & CFG frames from iPDC.

	     2. Run PMU Simulator in the machine where PMUSimulator is installed. You can run the PMU Simulator from anywhere in the system. Enter the PMU Port numbers for TCP and UDP. Port should be other than 9000, as 9000 is used by DBServer. 

	     3. Run iPDC and enter the details like TCP Port, UDP Port.
			These are the ports on which PDC would listen to command frames from other PDC's, or applications.
			Also enter PDC ID, that will be use for authenticate to other ends PDC's.
			Enter the IP Address of the machine where DBServer is running. (if local, may use like 127.0.0.1)
			Now add the PMU/Source devices by entring their minimal details.
			As soon as the PMU details are entered, a command frame is sent to machine where the PMU/Simulator is running.
			And communication between iPDC and PMUSimulator (PMU) will take place as follows.


          4. Communications
    
	          1. iPDC sends a command frame to PMU, requesting its configuration.

	          2. At PMU on receiving of a request for Configuration frame, PMU send back the latest configuration frame iPDC.
           
	          3. iPDC then send a command frame to requesting PMU to start data frames transmission.

	          4. PMU on reception of the command frame for send data would reply with data frames.

		          iPDC  -----------> 	PMU (Command to send CFG)
		          iPDC  <----------- 	PMU (CFG Sent)
		          iPDC  -----------> 	PMU (Command to send Data)
		          iPDC  <----------- 	PMU (Data Sent)
		          iPDC  <----------- 	PMU (Data Sent)
		          iPDC  <----------- 	PMU (Data Sent)
		          iPDC  -----------> 	PMU (Command to stop Data)

			 5. In the above mentioned PMU-iPDC communication, iPDC would act as a client and PMU as server.

     ii) When iPDC acts as a Server ->

		When iPDC acts as a Server, it sends the combined frames (data and Configuration) to other iPDC on requesting.
		The details of these other end iPDC's need to be entered first. In this case the iPDC would work like as PMU Server.
		iPDC receives a command frames from other iPDC it would first authenticate the request.
		If the request is from authentic iPDC then combined frame would sent to respective iPDC. Remaining same as iPDC-PMU Communication.
                                                                                                                         /
		More details of the s/w can be found in the Technical Report on iPDC on the website.


UNINSTALL
----------

     Uninstall iPDC ->
		cd iPDC.x.y/iPDC.x/
		su/sudo make uninstall   (will be needing roots permission)

     Uninstall DBServer ->
		cd iPDC.x.y/DBServer.x/
		make clean

     Uninstall PMUSimulator ->
		cd iPDC.x.y/PMUSimulator.x/
		su/sudo make uninstall   (will be needing roots permission)
