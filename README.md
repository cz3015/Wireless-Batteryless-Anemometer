# Wireless-Batteryless-Anemometer


This project concerns the development of an existing energy harvesting platform based on a miniature wind turbine into a wireless, battery-less anemometer. Working on this project entails a thorough understanding of a micro wind turbine (MWT) which was developed as a department research project previously in Imperial College London. MWT is used in this project as the sole generator to power the system. The usage of Bluetooth Low Energy allows us to achieve the “wireless” function.

Softwares to download:
1. Code Composer Studio (CCS)
Integrated development environment for the CC2650 and has debugging capabilities for embedded applications. The CCS version I'm currently using is version 8.2. I didn't use version 9 and above because the classic layout of the Resource Explorer tab (where you can import example code) was no longer there and was replaced by a newer layout which I find it more annoying to use.

The source code was written based on the tutorial given by SimpleLink Academy under the Sensor Controller Studio > Task Creation and Control section. You can refer to the tutorials in the link http://software-dl.ti.com/lprf/simplelink_academy/overview.html first to learn how to get started with implementing BLE and RTOS (Real Time Operating System) for the CC2650. The BLE software stack version I used was v2.2.1.18 but I think the latest one shouldn't be a problem.

Import the "energy_harvester" project by going to File > Open Projects from File System.

2. Sensor Controller Studio (SCS)
GUI tool to configure peripheral drivers of the CC2650 Sensor Controllers (ADC, I2C, etc). This tool generates a set of source files which will be integrated into the CCS code. Instructions on how to use SCS can also be found in SimpleLink Academy. 

3. BLE Device Monitor
Windows application used for monitoring and controlling TI BLE devices. Download link: http://processors.wiki.ti.com/index.php/BLE_Device_Monitor_User_Guide

4. LabView
Used for acquisition, visualisation and analysis BLE data. You can install LabView from the College's software hub website and its free for students. You'll need to install the BLE toolkit which contains drivers for the USB BLE dongle. Download link: https://forums.ni.com/t5/Community-Documents/LabVIEW-BLE-Bluetooth-Low-Energy-toolkit/ta-p/3538612?profile.language=en

