# PicoGrid with LoRa 2.4 GHz

Abstract
Renewable energy such as wind and solar are inherently unpredictable which may cause brownouts or blackouts in an electric grid. This document attempts to create a “Pico-Grid” which is a type of smart grid that is on a small scale, between different floors within the same building in the hopes it may alleviate the aforementioned problem. 
In this project instead of using a real smart metre or inverter, it will only create a simulation of such devices but only with dummy data. Its focus will mainly be on the communication side using a variety of communication protocols and software rather than physical energy exchange.
Keywords: smart grids, Pico-Grid, Arduino Nano, WiMODino, LoRa 2.4 GHz, ChirpStack, InfluxDB, Modbus.

Chapter 1 Introduction
1.1 Introduction
In this final year project, the goal is to create a “Pico-Grid”. In contrast to a microgrid or a community grid, a Pico-Grid would be created using a simulation of integrated smart batteries and grid-tie storage inverters that would exchange energy between different floors in a building. The floors would be able to export and import energy between them. This would require an IoT solution to connect the components in a bidirectional communication system that is reasonably secure, and reliable and high throughputs are desired. With this setup there would be no need to worry about enforcing disturbance-neutrality as excess energy will not spill into the electrical grid, thus there would also be no need to worry about Ireland’s Distribution System Operator (DSO). 
The project can be divided into three main stages:
•	LoRa 2.4 GHz Network - Establish communication between the EMB-Fem2GW-2G4-O gateway to the WiMODino. If procuring the EMB-Fem2GW-2G4-O gateway is not possible, then using the SX1280Z3DSFGW1 would be a suitable alternative. The WiMODino has already been programmed to communicate with a LoRa 2.4 GHz gateway and would require small modifications to the configurations to change the gateway to connect to. There would be changes needed if using the SX1280Z3DSFGW1. The EMB-Fem2GW-2G4-O is an out-of-the-box device with a preinstalled OS and configuration page that would hasten the connection process. The network server to enable LoRaWAN should be ChirpStack as the network server can be implemented into the gateway itself instead of using a cloud-based service such as The Things Network.
•	Pico-Grid - Mimic the predicted behaviour and functionalities of inverters, smart batteries and smart meters into the WiMODino. This would require reading into the documentation on the various devices and working with an expert on this particular subject. The WiMODino would then need to be programmed to transmit data from devices. The gateway would also need to transmit data, especially to the “inverter” to issue commands such as exporting energy.
•	Energy Trading Platform - Create a simple simulated energy trading platform. What would be required is a database to store the data of a prosumer, a dashboard to visualise the data and algorithms to simulate a working energy trading platform based on the data provided from the end nodes.
1.2 Ethical/Commercial/Safety/Environmental
There should be no negative ethical concerns in the pursuit of the project. The purpose of this project is to attempt to advance Ireland’s renewable energy initiative by stabilising the electric grid from unpredictable factors which may cause blackouts or brownouts. Therefore, the ethics in this project is for a positive cause.
This final year project has prompted me to help with a company called Smart M Power. Smart M Power is a Research and Development company that has undertaken multiple projects concerning smart grids. The final year project would have support from the business in the form of materials and equipment, advice and expertise. Any specific involvement in the form such as expertise, code etc. shall be acknowledged in the document. Technologies, methodologies or code utilised in this project may be used for commercialisation purposes for Smart M Power.  
While the project involves electric grids with smart batteries and grid-tie inverters within different floors in a building, this project only seeks to create a simulation of them. Therefore, there would be no direct handling of high-voltage electrical equipment. So, there is no need, for example, to have a qualified professional with the necessary credential to install a grid-tie inverter. 
There should be no adverse environmental impact in the project, except for perhaps the electricity used to power the equipment. Non-renewable resources are still the largest energy contributor to the electrical grid, in comparison, Ireland’s largest renewable energy contributor is wind power which only accounts for 86%of total renewable electricity and 36% of total electricity demand. 
The project safety plan has already been filled in. The potential hazards that are identified in the document are:
•	Computer Usage
•	Lone Working
•	Working Off Campus / Field Work
The category of supervision ticked in the document is Category D. Which means that the risks are low and carry no special supervision requirements. 
1.3 Literature Review
The world in recent times, has started to shift towards green energy generation resources such as solar, wind, tidal etc. This can be seen in international politics such as The Paris Agreement which is a legally binding international treaty to tackle climate change. 196 parties such as Ireland hinged into an agreement in 2016 which has encouraged them to pursue carbon neutrality targets. The Irish government has further legally bound itself by signing the “Climate Action and Low Carbon Development (Amendment) Act 2021” where Ireland has to undertake a path towards net-Zero emissions by 2050 and have a 51% reduction in emissions before 2030.
One of the biggest problems faced by renewable energies is that they are produced at certain times of the day and are typically not meeting peak demand hours. For example, solar PV produces energy during the day but will not be able to meet energy demand during the night when consumers return to their homes. Solar energy and wind energy are unpredictable which can cause instability in the grid via blackouts or brownouts. 
One way to handle this is the use of a battery storage system. It allows the grid the ability to store the excess energy for the energy to be used at a later time. Though the cost and capacities of batteries have improved over time, large-scale deployment battery storage systems are still too expensive to be feasible. The other method is smart grids.
A smart grid is an electrical grid that provides multiple services that are normally not provided by traditional grids. Some of these services are:
•	Advanced metering infrastructure i.e., smart meters.
•	Demand response which is a change in power consumption of the electric utility to match the demand for power with the supply. 
•	Use of renewable energy resources such as charging electric vehicles, batteries etc.
•	Export of surplus energy produced by non-electric utilities such as businesses and households or in other words, prosumers.
•	The utilisation of wired or wireless communication technology to connect and monitor the various technologies mentioned.
1.4 Work Plan
As mentioned earlier, there are three main stages in this project, creating a LoRa 2.4 GHz Network, a simulated PicoGrid and the creation of an Energy Trading Platform. There is a good chance that the Energy Trading Platform would not have sufficient time left remaining to be worked, so it would it apt to consider that stage an optional objective. 
The first stage, the creation of a LoRa 2.4 GHz Network has the following activities that need to be done:
•	Research LoRa 2.4 GHz and LoRaWAN
•	Change the firmware on the end nodes
•	Programme the end node to transmit uplink data
•	Install ChirpStack on the gateway and configure the gateway to set up the end nodes to the gate
•	Programme gateway to transmit downlink data
•	Work on documentation on setting up the gateway and end node
•	Perform coverage planning using software such as CloudRF
•	Perform testing scenarios (reliability, coverage)
The second stage which is creating a simulation of the Pico-Grid requires:
•	Research Smart Meters and Smart Inverters
•	Create a Modbus bridge between the LoRa end node and an Arduino device acting as the Smart Metre
•	Create the Modbus slave to mimic the Smart Metre’s input register
•	Make the LoRa end node transmit the Smart Metre data to the gateway
•	Figure out how to implement a mimicry of a Smart Inverter and exchange data between it and the LoRa end node. May have to be Modbus or Sunspec Modbus
•	Make the LoRa end node transmit the inverter data to the gateway and the gateway sends downlink information back to the device mimicking the inverter
The third stage is creating an energy trading platform which would need:
•	Research energy trading platform/peer-to-peer energy trading
•	Setup a server/database on the laptop to store data
•	Retrieve data received from the ChirpStack network
•	Programme trading algorithms 
•	Develop the GUI software of the energy trading platform 
•	Come up with a unit test for the software and the trading algorithm
Chapter 2: Design Development
2.1 Overall Network Design

 
Figure 1: Overall Network Design
This project will be using three main communication standards, which are Modbus RS485, LoRaWAN 2.4 GHz and TCP/IP. There are two Arduino Nanos that would be acting as a smart metre or a smart inverter and they would be connected with their own WiMODino iM282A via Modbus RS485 to exchange data. The WiMODinos will communicate with the SX1280Z3DSFGW1 ChirpStack gateway using LoRa 2.4 GHz. Then finally, the data will be sent to the laptop’s InfluxDB database through TCP/IP or more specifically HTTP. 
The data will be able to be observed through InfluxDB’s dashboards using the localhost. There is also a Python program that is connected to the InfluxDB to get data and it is also connected to the ChirpStack network to send downlink communication back to the WiMODinos. The Python program should have been able to make decisions based on the retrieved data and then downlink data back to the end nodes. For example, if the smart inverter has detected that the connected battery has almost 10%, the Python code then sends a downlink a battery shutdown message to the inverter to avoid damage to the battery.
2.2 Hardware Equipment
2.2.1 SX1280Z3DSFGW1
The SX1280Z3DSFGW1 was originally intended for use with The Things Network (TTN) as stated in the “User Guide to the LoRa 2.4GHz 3 Channels Single SF Reference Design” which is its quick start guide. For the purposes of the project, I would be using the ChirpStack network. 


 
Figure 2: SX1280Z3DSFGW1
2.2.2 WiMODino iM282A
The WiMODino is an Arduino board that is made for fast prototyping and evaluation of the iM282A LoRa radio module. The iM282A is a cheap radio module that uses the unlicensed 2.4 GHz frequency band with the SX1280 transceiver from the Semtech Corporation.
The WiMODino has accidentally been ordered with the LR Base Plus firmware which is a proprietary software that allows LoRa to implement features not found in LoRaWAN such as a LoRa gateway and a network server not being necessary and the use of a peer-to-peer or star network topology. However, this technology is outside of the project’s scope. Therefore, the software needed to be replaced with another software called GlobalLink24 which is compatible with the LoRaWAN. 


 
Figure 3: WiMODino IM282A
2.2.3 Arduino Nano
The Arduino Nano is perhaps the second most popular microcomputer from Arduino. It is a relatively small and breadboard-friendly board that is based on the ATmega328P microcontroller. It has a lot of the same features as the Arduino UNO but it is a cheaper alternative. 

 
Figure 4: Arduino Nano
2.2.4 LTC1485
The LTC 1485 is a differential bus transceiver that is designed to facilitate communication between two different signals by converting the electrical signals into using either the RS422 or the RS485 point-to-point communication standard. In this project, I am using the RS485 to allow communication between the Arduino Nano and the WiMODino iM282A.

 
Figure 5: LTC1485
2.2.5 Physical Setup
The image below is a photo of the physical setup between WiMODino which is connected to the Arduino Nano acting as a smart metre. The other Arduino Nano will be emulating as the smart inverter.
 
Figure 6: WiMODino & Arduino Nano
2.3 Software
2.3.1 LoRa
The LoRa wireless technology is a proprietary technology developed by Cycleo and acquired by Semtech in 2012. Low Power Wide Area Network (LPWAN) technology allows for long-range communication with low power consumption. LoRa achieves its long-range connectivity by using a modulation technique called Chirp Spread Spectrum (CSS), which spreads the signal over a wide bandwidth and makes it more robust against interference and noise.
LoRa is designed to transmit small payloads like sensor data, making it suitable for Internet of Things (IoT) applications. It is a half-duplex communication technology, which means that it can send and receive data, but not at the same time. LoRa uses a star network topology or a star-to-star network topology, which enables it to communicate with multiple end nodes through a single gateway.
The minimum received signal strength indicator (RSSI) of LoRa is -120 dBm, which is the minimum signal strength required for successful communication. The LoRa technology operates at the physical layer of the Open System Interconnection (OSI) model layer.
It is important to note that LoRa is often confused with LoRaWAN, which is a protocol that operates at the data link and networking layers of the OSI model. LoRaWAN is an open standard that allows for interoperability between different LoRa devices and networks, while LoRa is a proprietary technology with a closed source.
Finally, it is worth noting that for this project, battery optimization may not be a significant concern as both the gateway and end nodes will be powered via mains.
2.3.2 LoRa Modulation
CSS is known for low power consumption and robustness against channel degradation challenges such as interference and multi-path fading. It is a spread spectrum technique that uses wideband linear frequency-modulated chirp pulses to encode data.  Spread spectrum techniques are methods by which a signal is deliberately spread in the frequency domain. For example, a signal is transmitted in a short burst, “hopping” between frequencies in a pseudo-random sequence. 
LoRa nodes have a radio module with a modulator that encodes information onto a carrier signal. This modulated signal is transmitted and received by a gateway or end node. The LoRa nodes also have a demodulator which decodes the modulated signal and extracts the information.
 
Figure 7: End node to gateway modulation signals, source at [8] 
A chirp, often called a sweep signal, is a tone in which the frequency increases (up-chirp) or decreases (down-chirp) with time.
 
Figure 8: Sweep signal, source at [9]
The chirps are cyclically shifted, and it is the frequency jumps that determine how the data is encoded onto the chirps, aka LoRa modulation.
The spreading factor (SF) defines two values:
•	The number of raw bits that can be encoded by that symbol
•	Each symbol can hold 2 SF 
Spreading Factor impact:
•	The symbol duration doubles compared to the previous SF
•	It reduces the bit rate by approximately half compared to the previous SF
•	The Time on Air (ToA) increases which means the distance increases. ToA is the message transmission time
If an end device is further away from a gateway the signal gets weaker and therefore needs a higher spreading factor.
A symbol represents one or more bits of data, for example, Symbol = 1011111 (decimal = 95). In this example, the number of raw bits that can be encoded by the symbol is or has a Spreading Factor (SF) of 7. The symbol has 2SF values. If SF =7, the values range from 0-127 Therefore the chirp is divided into 2SF steps.  The Spreading Factor values are 7 – 12. The higher the value, the longer the range but lower data rates and longer ToA. 
Note: Difference between chirp & chip:
•	A symbol holds 2SF chips (To make things easier, symbols and chips can be used interchangeably).
•	Chirps are simply a ramp from low to high signals (down-chirp to up-chirp).
 
Figure 9: Sweep signal divided into 2SF chips, source at [9]
For each spreading factor, there is a SNR limit, if this limit is reached the receiver will never be able to modulate the signal.
 
Figure 10: SNR limit, source at [9]
LoRa has a Forward Error Correction (FEC) function where error correction bits are added to the transmitted data. These redundant bits help to restore the data when the data gets corrupted by interference. If more error correction bits are added, the easier the data can be corrected. However, adding more can decrease battery life. 
Coding Rate (CR) refers to the proportion of the transmitted bits that carries information, the rest being FEC bits. The LoRa coding rate values are CR = 4/5, 4/6, 4/7 or 4/8.
The LoRa packet comprises three elements: Preamble, header (optional) and payload.
 
Figure 11: LoRa packet structure, source at [10]
The preamble is used to synchronise the receiver with the incoming data flow. By default, configured with a 12-symbol long sequence. This is shown as the unmodulated data signal the and preamble ends with the 2 down-chirps.
Explicit header mode includes a short header that contains information about the number of bytes, coding rate and whether a 16-bits CRC is used in the packet. This is the default mode.
Implicit header mode is where the payload, coding rate and CRC presence are fixed or known in advance. This will remove the header from the packet. The known parameters must be configured on both sides of the radio link. This mode will reduce transmission time. 
The payload is a variable length field that contains the actual data coded at the error rate (Coding Rate) either as specified in the header in explicit mode or the register settings in implicit mode.
2.3.3 LoRaWAN
While LoRa is on the Physical Layer of the OSI model, LoRaWAN is on the Session, Network and Data Link Layer. LoRaWAN is implemented on top of the LoRa modulation bands. LoRaWAN is an open standard that is defined and maintained by the LoRa Alliance, a non-profit organization that promotes the adoption of the LoRaWAN technology.
 
Figure 12: LoRaWAN Architecture, source at [11]
The LoRaWAN protocol provides a range of features that enable efficient and reliable communication between LoRa devices and LoRaWAN networks. These features include:
•	Configuring radio parameters: LoRaWAN allows network operators to configure the radio parameters of LoRa devices remotely. This enables operators to optimize the performance of the network and ensure that devices are using the most efficient settings for their specific applications.
•	Device activation: LoRaWAN supports two types of device activation, namely over-the-air activation (OTAA) and activation by personalization (ABP). OTAA is a secure and automated method of device activation that involves exchanging keys between the device and the network. ABP, on the other hand, is a manual method of activation that involves pre-configuring the keys on the device.
•	Message integrity checking: LoRaWAN includes mechanisms for ensuring the integrity of messages transmitted over the network. This is achieved through the use of message authentication codes (MACs) that are generated by the sender and verified by the receiver.
•	Session management: LoRaWAN manages the session between the device and the network, including authentication, encryption, and key management.
•	Application payload encryption: LoRaWAN provides encryption of application payloads, ensuring that sensitive data is protected from unauthorized access.
2.3.4 LoRaWAN Summary of Classes and Gateway
Class A Devices:
•	Send at any time
•	Receive directly after uplink
•	Device always initiates communication
•	Most battery efficient
Class B Devices:
•	Receive at specific time intervals
•	Consumes more energy than Class A
•	Beaconing
Class C Devices
•	Receive at any time 
•	Consumes most energy
•	Continuous listening
2.3.4.1 Class A End Nodes
Class A suitable for: 
•	Devices that send data on time-based interval
•	Send event-driven data (e.g. temperature goes above 20°C)
Each uplink transmission is followed by 2 short downlinks receive windows to allow packets from the server to be received. Downlink messages from the server will have to wait until the next scheduled uplink.
If a message arrives, the end device doesn’t know for whom the packet is meant. The packet gets demodulated and the device address and Message Integrity Code (MIC) need to be checked. If the packet is meant for the device that receives it only opens the first receive window and not the second. If this is not for the device, it reads the first and second windows.
 
Figure 13 Class A Transmission, source at [12] 
2.3.4.2 Class B End Nodes
This allows for more receive slots than for Class A. In addition to their class A receive windows, these devices have extra Rx at scheduled times. To do this, it receives a time-synchronized beacon from the gateway. This allows the server to know when the node is listening. The device must first receive a network beacon and align its internal timing with the network server. The end node can be used by the network to initiate downlink communication. A network-initiated downlink message is called a ping. The gateway selected to initiate this downlink is selected by the network server based on signal strengths and the quality of uplink messages. The device must periodically send a beacon message to cancel any drift that may occur with its internal clock.
 
Figure 14 Class B Transmission, source at [12]
2.3.4.3 Class C End Nodes
These devices have continuous receive windows that are only closed when the device is transmitting. They consume the most power. They implement the same 2 receive windows as Class A, but they don’t close Rx2 until they need to send again. This way they can receive the downlink in the Rx2 window at any time. There is no specific message sequence for the node to tell the server whether it is Class A or C. It is up to the application server to be aware that it manages Class C devices based on the characteristics during the join process.
 
Figure 15 Class C Transmission, source at [12]
2.3.5 Network Server Key Features
The Network Server (NS) is responsible for managing the connectivity of devices and gateways. Some key features of a LoRaWAN Network Server include:
•	Connection with gateways: The NS communicates with LoRaWAN gateways to receive data from devices and send data to them.
•	Commissioning and supervision interface: The NS provides an interface for commissioning new devices and supervising the network.
•	Deduplication of packets
•	Device registry: The NS maintains a registry of devices connected to the network, including their security sessions, operation modes, timing windows, frequencies, and data rate for downlink.
•	Device identification: The NS uses the network session integrity key to identify devices
•	Control data rate: The NS uses Adaptive Data Rate (ADR) to control the data rate of devices to optimise network performance and battery life.
•	Set radio parameters and timing windows: The NS configures radio parameters and timing windows for devices, optimising network performance.
•	Prevent replay attacks
•	Downlink message queue: The NS maintains a queue of downlink messages to be sent to end node devices.
•	Select gateway for downlink: The NS selects the best gateway for sending downlink messages to devices based on Link Quality Indication, which includes factors such as Received Signal Strength Indicator and Signal-to-Noise Ratio.
•	Prevent conflicting messages: The NS prevents conflicting messages from being transmitted by devices.
2.3.6 Application Server Key Features
The Application Server (AS) is in charge of processing the application data sent by devices. Some key features of a LoRaWAN Application Server include:
•	Connected to the Network Server: The AS is connected to the NS which enables it to receive and process application data sent by devices.
•	Encryption and decryption of payload: The AS is responsible for encrypting and decrypting the payload sent by devices.
•	Recommended to run in the user's domain: The AS is typically run in the user's domain, allowing them to have full control over their application data.
•	Web interface: The AS may provide a web interface that enables users to access and manage their application data.
•	Integrate with IoT platforms: The AS can integrate with various IoT platforms which creates the capability for users to leverage the data sent by their LoRaWAN devices in various applications and services.
2.3.7 Join Server Key Features
The next component in a LoRaWAN network is the Join Server (JS) which is for securely activating devices and setting up their communication sessions with the Network Server (NS) and Application Server (AS). Some key features of a LoRaWAN Join Server include:
•	Activation of devices: The JS is responsible for securely activating new LoRaWAN devices and setting up their communication sessions with the NS and AS.
•	Session Security Keys: The JS generates and provides the necessary Session Security Keys to the device for secure communication with the NS and AS. 
•	Specifies which Network Server the User wants to work in: The JS enables the user to specify which NS they want their device to work with.
2.3.8 Adaptive Data Rate (ADR)
Adaptive Data Rate (ADR) enables the Network Server to dynamically control the data rate of devices. Initially, ADR instructs devices to use the highest data rate possible for maximum capacity and the most efficient power consumption. This is achieved by sending ADR commands to devices to keep or increase their data rate.
If the network does not respond to several messages, the device will lower its data rate to increase the probability that packets are received by the network. Additionally, ADR reduces the transmission power to have less channel utilization of gateways.
It is important to note that ADR only works for devices that are not moving, so the network learns over time which data rates work best when gateways and devices are in fixed locations. As a result, ADR provides a mechanism for optimal and efficient use of the available network resources while ensuring reliable communication between devices and the network.
2.3.9 Gateway message processing
Gateway message processing allows gateways to receive and forward messages between end devices and the network server. One of the key features of gateways is their ability to receive messages on different frequency channels and with different data rates at the same time. This means that gateways can simultaneously receive messages from multiple end devices, allowing for efficient use of available network resources.
In addition, gateways provide message acknowledgement for important packets. If a message is not received, the end device will retry sending the message until it receives confirmation of successful transmission. This ensures reliable communication between the end devices and the network server.
2.3.10 LoRaWAN Security 
LoRaWAN provides security features to ensure the confidentiality, integrity, and authenticity of the transmitted data between the end devices, gateways, and network servers. These security features are implemented at the network and application layers.
At the network layer, LoRaWAN ensures the identification of the device and integrity check of the transmitted data. Each end device has a unique identifier that is used to identify and authenticate the device. The transmitted data is checked for its integrity to ensure that it has not been tampered with during transmission. This is achieved using a 32-bit Message Integrity Check (MIC) that is calculated by the end device and verified by the network server.
LoRaWAN uses the Network Session Integrity Key to ensure the security of the network layer. This key is unique to each end device and is used to encrypt and decrypt the network layer messages. The 128-bit Advanced Encryption Standard (AES) algorithm is used to encrypt the MAC commands, which are sent over the air interface. These commands can include changing the data rate, enabling or disabling channels, and changing the downlink frequencies, among others.
At the application layer, LoRaWAN ensures the encryption and decryption of the application payload using the Application Session Key. This key is also unique to each end device and is used to encrypt and decrypt the application payload. The 128-bit AES algorithm is used to encrypt the application payload, providing end-to-end security.
2.3.11 Device Activation
LoRaWAN supports two types of device activation methods - Activation by Personalization (ABP) and Over-the-Air Activation (OTAA).
Activation by Personalization (ABP) is a method where the device is pre-configured with its unique device address and security session keys. The device is programmed with these parameters before deployment, and the network server is already aware of the device's identity. This approach is simpler and faster than OTAA since there is no exchange of messages between the device and the network server to establish security keys. However, ABP has some security limitations as there is no exchange of keys between the device and the network server.
Over-the-Air Activation (OTAA) is a more secure activation method, where the device and network exchange a series of messages to establish a secure session. When a device activates by joining the network, it sends a Join Request message to the network server. The network server then responds with a Join Accept message containing a unique device address, network session key, and application session key. These keys are used for subsequent communication between the device and the network server. This method is more secure as the keys are exchanged securely but it takes more time than ABP. OTAA is recommended for most applications where security is a concern.
2.3.12 ChirpStack
To create a private LoRaWAN network with no third-party cloud networks, ChirpStack was used. This is an open-source LoRaWAN Network Server stack provides that the whole LoRaWAN stack including the network and application server. It allows for easy device activation and management, data handling, and integration with various applications and IoT platforms. 
ChirpStack provides a web-based interface for managing the gateways. It allows for configuring the gateway parameters, monitoring the gateway status, and troubleshooting any issues. It also provides various APIs and protocols for integrating with external applications and IoT platforms. It allows for easy data exchange and seamless integration with different systems.
The graph below showcases the ChirpStack architecture of the components:
 
Figure 16: ChirpStack Architecture V4, source at [13] 
•	The ChirpStack Concentratord is a concentrator daemon that is placed on top of the hardware abstraction layers. It uses ZeroMQ which is an open-source universal messaging library that provides sockets that can send atomic messages in various transports such as TCP, multicast, in-process and inter-process. It could also run without a message broker. The ZeroMQ-based API allows packet wording applications like the UDP forwarder and MQTT forwarder to be decoupled from the gateway hardware.
•	The ChirpStack MQTT Forwarder is a component that converts LoRa Packet Forwarder protocols into JSON and Protobuf.
•	The ChirpStack Gateway Bridge is a service whose purpose is to convert LoRa packet forwarder protocols into a common data format
2.3.13 Modbus
Modbus was created in the 1970s by a company called Modicon which is now called Schneider Electric as a communication protocol standard for programmable logic controllers [8]. This protocol uses a master and slave technique which allows a master to poll at least one device for data that are operating as a slave. The master can read and even write data on the slave data storage. There are two main Modbus protocols which are Modbus RTU and Modbus TCP. In this project, Modbus RTU is used.
Modbus RTU is perhaps the most commonly used as it is a proportionally simple protocol that transmits data using UART. The data are being transmitted in 8-bit bytes on baud rates that can be around 1200 bits per second (bps) or 38400 bps. In a Modbus RTU network, each slave must be assigned a device address or a unit number for the master to identify on which it should send the message. The slave must send a response on a timely matter or else the master would have to declare it to have a no response error. Modbus RTU which is using the RS-485 network could only have 32 devices.
The Modbus data are defined to be 16-bit registers. However, if a 32-bit floating point is required then those values need to be treated as a pair of registers. There are 4 types of registers;
•	Coil/Discrete output: This is a 1-bit register that can be read or written. Typically used as true or false statements.
•	Discrete input: This is also a 1-bit register but can only be read. 
•	Input register: It is a 16-bit register that can only be used to read values.
•	Holding register: Just like the input register, it is 16-bit but it can read and write data.
2.3.14 InfluxDB
InfluxDB is an open-source database management system that was developed by InfluxData, Inc in 2013. It is written in Google’s programming language called Go or known as Golang. InfluxDB is a type of database called a time series database which is made efficiently to store time-based values. In other words, its database is used to store and analyse sensor data with timestamps over a period of time. To make sure that time is synchronised between systems, InfluxDB is included to have the Network Time Protocol.	
InfluxDB’s columns have two types, tag and field. The tag column can be used as an identifier or metadata while fields contain the values to be analysed. Even its terminology for retrieved data is called a measurement.
In comparison to standard relational databases such as MySQL, time series databases are faster in storing and processing time-based measurement data. As typical data database systems have more complex indexes which result in slower speed. InfluxDB’s more focused or rather simple index allows it high write speeds.
Chapter 3: Implementation
3.1 Flowchart
Figure 17 is a flowchart of the overall system. At the start, the Arduino Nano which can either be acting as the Smart metre or inverter as the Modbus server, will be updating Modbus registers in which the values reside. It will be doing this action every 100ms by updating. The WiMODino which is the Modbus client will be reading the voltage, current, kWhr and phase and then transmitting via LoRa 2.4 GHz every 30 seconds to the SX1280Z3DSFGW1 gateway.  The ChirpStack network which resides on the gateway uses the InfluxDB integration to create a HTTP connection to connect with a laptop. A Python code on the laptop is polling the InfluxDB database every 30 seconds for the latest data and is sending downlink messages to be queued to the ChirpStack network.

 
Figure 17: Flowchart of the overall system
3.2 Circuit Diagram
Figure 14 shows the circuit diagram connection between the Arduino Nano, the WiMODino iM282A, the LTC1485 differential bus transceiver, capacitors, LED lights and resistors. The two Arduino Nanos are connected as an RS485 serial bus with the help of the ltc1485 to create an RS485 system. They both have their LTC1485 to signify that they are the es. Meanwhile, the WiMODino has its LTC1485 to act as the master. The yellow LEDs next to each of the Arduino Nanos shows the user that a message has been read. The blue LED connected next to the WiMODino shows that a message has been sent to a slave. The RS485 connection is half-duplex and there should be done on the software side to avoid potential collisions. It should be noted that the physical circuity was set up by Frank Duignan.
 
Figure 18: Circuit diagram of WiMODino and Arduino nano (Smart metre)
LTC1485 pin functions and connections:
Pin	Function	Arduino Nano	WiMODino
R0 (Pin 1)	Receiver output	Connected to Arduino Nano’s D10	Connected to WiMODino’s D1
RE (Pin 2)	Receiver output enable	Connected with DE with Arduino Nano’s D6 and LED	Connected with DE with WiMODino’s D6 and LED
DE (Pin 3)	Driver output enable	Connected with RE with Arduino Nano’s D6 and LED	Connected with RE with WiMODino’s D6 and LED
DI (Pin 4)	Driver input	Connected to Arduino Nano’s D11	Connected to WiMODino’s D0
GND (Pin 5)	Ground Connection	GND pin	GND pin
A (Pin 6)	Driver Output/Receiver Input	This should be connected to another LTC1485	This should be connected to another LTC1485
B (Pin 7)	Driver Output/Receiver Input	This should be connected to another LTC1485	This should be connected to another LTC1485
Vcc (Pin 8)	Positive Supply, 5V	5V pin	5V pin

3.3 CloudRF: Radio Planning Tool
CloudRF has a web application that can be used to help with RF planning. It shows the cover range of radio technologies like LoRa with Google Maps. This was done by inputting the parameters of the gateway like its transmission height, frequency, transmission power, terrain type etc. The terrain data, antenna patterns and clutter layers are all already included. However, it doesn’t take account into the spreading factor. 
It also has a feature called the Path Profile Analysis (PPA) that shows a point-to-point link between the gateway and the end node on a 2D profile graph highlighting obstructions. An important detail to when looking at the PPA is the Path Loss. If the path loss exceeds LoRa’s receiver sensitivity then communication between the devices is impossible.
3.3.1 Limerick Georgian Building
The building chosen for testing is a Georgian building in Limerick that was chosen as the gateway location for use by Smart M Power. The parameters listed below are what was used for the CloudRF tool. 
Site/Tx
•	Height AGL: 5 Metres
Signal
•	Frequency: 2402 MHz
•	RF Power: 28 dBm
•	Bandwidth: 0.2 MHz
Feeder
•	Coaxial type: TMS LMR-400
•	Coaxial length 1m
•	Connectors 1
Antenna
•	Pattern: Ubiquity AM-2G16-90-HPOL.MSI
•	Polarisation: Vertical
•	Antenna Gain: 16 dBi
•	Azimuth: 0
•	Down-tilt: 0 
Mobile/Rx
•	Receiver Height AGL: 5 Metre
•	Antenna Gain: 16 dBi
•	Sensitivity: -97 dBm
•	Noise floor: -120 dBm
Model
•	Propagation model: ITM/Longley-Rice (< 20GHz)
•	Reliability: 90% (Higher percentages give more conservative results)
•	Context: Average/Mixed
•	Diffraction: Knife edge
Clutter
•	Profile: Minimal.clt
•	Landcover: Enabled + Buildings
•	Custom clutter: Disabled
Output
•	Resolution: 20m / 65ft
•	Colour schema: RAINBOW.dBm
•	Radius: 3 Km
As expected, from the results shown in Figure 19 it theoretically be able to provide coverage in a couple of blocks at best.
 
Figure 19: Coverage Map of a Georgian building to be used as a gateway
The end node location is also chosen by Smart M Power as it may potentially set up a LoRaWAN location there. It is merely right across the Georgian building and therefore its RSSI is still fairly strong.
 
Figure 20: Path Profile Analysis from the Georgian building to Hanratty’s Accommodation Centre
3.3.2 TUDublin Central Quad
A place where some actual coverage testing was done was on the fifth floor of TUDublin’s Central Quad building. Below are its parameters:
Site/Tx
•	Height AGL: 25 Metres (a guess made by Frank Duginan)
Signal
•	Frequency: 2402 MHz
•	RF Power: 28 dBm
•	Bandwidth: 0.2 MHz
Feeder
•	Coaxial type: TMS LMR-400
•	Coaxial length 1m
•	Connectors 1
Antenna
•	Pattern: Ubiquity AM-2G16-90-HPOL.MSI
•	Polarisation: Vertical
•	Antenna Gain: 16 dBi
•	Azimuth: 0
•	Down-tilt: 0 
Mobile/Rx
•	Receiver Height AGL: 5 Metre
•	Antenna Gain: 16 dBi
•	Sensitivity: -97 dBm
•	Noise floor: -120 dBm
Model
•	Propagation model: ITM/Longley-Rice (< 20GHz)
•	Reliability: 90% (Higher percentages give more conservative results)
•	Context: Average/Mixed
•	Diffraction: Knife edge
Clutter
•	Profile: Minimal.clt
•	Landcover: Enabled + Buildings
•	Custom clutter: Disabled
Output
•	Resolution: 20m / 65ft
•	Colour schema: RAINBOW.dBm
•	Radius: 3 Km
In comparison to the building in Limerick, the Central Quad has a pretty favourable simulated result. This is likely due to the building’s higher height which would allow it to go over many obstacles.  
 
Figure 21: Coverage map of TUDublin's Central Quad
The end node location first chosen to test the coverage range is the nearby Grangegorman Playground. With a clear line of sight, there should be no problem with getting a reliable connection to the gateway.
 
Figure 22: Path Profile Analysis nearby the Grangegorman Playground
4.4 Brief Program Explanation
4.4.1 Arduino Nano codes
The name of the program for the Arduino Nano used to imitate the smart metre is called “ModbusExampleSlave” whilst the one for the smart inverter is the “ModbusExampleSlave2”. Both programs are using the SoftwareSerial and SimpleModbusSlaveSoftwareSerial libraries to enable the connection between the ltc1485 and the WiMODino through Modbus. It is in this code that the pins are defined, the data are initialised with data and are placed in Modbus Holding register addresses. In the main loop it checks if there are any errors and updates the values in the Modbus registers every 100ms. “ModbusExampleSlave” has been assigned the slave ID of 1 whilst “ModbusExampleSlave2” has been given slave ID 2. These slave IDs are needed by the master to communicate with them separately.
4.4.2 WiMODino iM282A code
The WiMODino’s code is called “WimodModbusClientV2”. One of the libraries in use is WiMODGlobalLink24 which enables LoRaWAN communication. It also uses the ArduinoModbus library which depends on the ArduinoRS485 library to establish Modbus RTU communication. Similarly, the pins have to be defined but the pins are defined by default using the ArduinoRS485 library or more specifically the “RS485.h” source file. It initialises all of the needed data variables from both Arduino Nanos. 
The WiMODino is configured to use Class C as the WiMODino does not need to conserve power as it will be connected to the laptop for electricity. It is also configured to use the APPKEY and the APPEUI which would provide access to the ChirpStack network. Some important things to note is that the APPEUI is left empty due to the ChirpStack network does not require this but the WiMODGlobalLink24 still requires this to be included. 
On the main loop, every data register from both Arduino uses its slave ID to specify them individually. Each Modbus read has been given a 100ms delay for the RS485 time more than enough to read the data. Once all data has been read, they would be combined into four variables that would be transmitted every 30 seconds. All downlink communication is read as hexadecimal values.
4.4.3 ChirpStack to InfluxDB configuration
All received data from the end nodes are in hexadecimal format which will present a problem when trying to send the data to InfluxDB as it would not be in a human-readable format. Within the ChirpStack web interface, there is a way to manipulate all uplink and downlink data using JavaScript. This is located in the “Device Profiles” section, choose the “WiMODino” device profile and then click “Codec”. In summary, the code written would turn the hexadecimal payload into a string format. Next, it would divide the payloads into separate variables to enable InfluxDB to appropriately measure the variables and show them in dashboards.
4.4.4 Laptop Python
The laptop runs a Python program called “chirpstackDown.py” which would connect to InfluxDB and retrieves recent data in the last 30 seconds to be printed out on a console. Its functionality is that it would connect to the ChirpStack network using an API token and then send out a downlink message using the WiMODino’s unique device EUI. 
Chapter 4 Results
4.1 Testing
Coverage testing
Before committing to any coverage testing, some RF Planning must be done beforehand. A suitable for such is CloudRF which is an online RF planning software. Then we can use real coverage testing on a physical location in Limerick or elsewhere. Afterwards, the results from the coverage testing and the expected results from CloudRF can be compared and contrasted for analysis.
Test Scenarios:
•	Have the transmitters send data indoors and outdoors.
•	Leave the SLU transmitting for 5 minutes.
•	Perform one of the tests in severe weather conditions such as heavy fog (Optional).
4.1.1 Coverage Testing
There was coverage testing performed at the Grangegorman campus, specifically the Central Quad on the CQ-507 room. My supervisor and I have made an educated guess that the height at which I placed the gateway is around 25 metres. 
 
Figure 23: LoRaWAN gateway at Central Quad for coverage testing
The end node location to test is just around 250 metres away from the gateway, nearby the Grangegorman Playground. The end node code has been slightly modified to send 10 data to the gateway every 30 seconds and therefore the test lasts for 5 minutes. Unfortunately, none of the transmissions were detected by the gateway. The likeliest culprit for this may have been the thick double-glazed windows have severely affected the signal’s attenuation.
 
Figure 24: End node nearby the Grangegorman Playground for coverage testing
There was an impromptu testing performed just outside the Central Quad’s main entrance which is a mere 50 metres away from the gateway. Puzzlingly the results are just as bad. The next test should be performed clear of obstacles. 
4.2 System run
This section shows how the system running looks like and any notable facts that typically occur during its runtime. It may even include an analysis of what may the problem may be. 
Running the Arduino Nano programs have no print statements and so the focus will be shifted to the WiMODino. When running the WimodModbusClientV2 program on the Arduino IDE with the console window opened, an example shown in Figure 25 is what is expected to run. Some of the registers read sometimes fail. This could be because the 100ms delays between each read may be insufficient.  
 
Figure 25: WiMODino console print
If the end node does pick up a downlink message it gets shown in the console window as well. This is shown in Figure 26.
 
Figure 26: WiMODino reading a downlink message
Figure 27 shows a couple of the data received by InfluxDB in a dashboard. Each of the three-phase data is placed within the same dashboard cell, with different colours to differentiate between them. 
 
Figure 27: InfluxDB dashboard of metre reads
The IDE to run the Python program is called PyCharm and Figure 28 are a couple of values read by the program when querying InfluxDB every 30 seconds. 
 
Figure 28: PyCharm IDE console query print to InfluxDB
5.5 Conclusions
5.5.1 Conclusion
Looking at the work plans I had completed stage 1, mostly stage 2 and stage 3 has not even been started. It was expected that stage 3 would not be completed as it has always been seen as a bonus objective just in case, I had managed to breeze through the work plans. Though Stage 2s last objective of making ” ...the LoRa end node transmit the inverter data to the gateway and the gateway to send downlink information back to the device mimicking the inverter” was only partially completed and therefore not to create a simulation of a PicoGrid or any type of smart grids, the main objective given to me by Smart M Power is creating a LoRaWAN 2.4 GHz network has been a success and any further work can be achieved at a later date.
5.5.2 Future Work
Many features and problems have not either been implemented or fixed due to a lack of time and resources. Some of the future work that can be done are:
•	Hardware case: All of the equipment is fully exposed with no casing to prevent unintentional tampering and the elements such as rain.
•	Different gateway: The SX1280Z3DSFGW1 is a demonstrator gateway meant for fast prototyping of the LoRaWAN 2.4 GHz could be a problem when used in mass production use. It uses a Raspberry Pi 4 which is quite expensive and is still in short supply worldwide. 
•	Real data: All of the data “read” by the Arduino Nanos are dummy data which may or may not have expected values that should give out in a building. In the future, the Arduino Nano should be physically connected to a smart metre and inverter.
•	Faster end node transmission: Theoretically the data rate by LoRa 2.4 GHz should be higher than the typical 868 MHz frequency. However, the end node has been failing to transmit data that are less than 30 seconds since the previous transmission. The cause could be that the end node has not been properly configured to ignore Duty Cycle or the gateway has not. 
•	Finish stage 2: As mentioned in the conclusion section, the next big step would be to create the actual PicoGrid network. 
•	Finish stage 3: While not a step that Smart M Power needed me to work towards, this would still be a valuable experience to have. Perhaps something similar can be achieved as a personal project.
