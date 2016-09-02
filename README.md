This is a project to make a six-wheel robot with Arduino and Raspberry Pi.  
It consists with

- Six-wheel car platform
- Dual VNH3SP30 Motor Driver Carrier MD03A
- Battery
- Arduino
- Raspberry Pi
- Wifi module
- camera

# Hardware architecture
![system_hardware](wiki/images/system_hardware.png)

# Software architecture
## Architecture with node.js
This is the planned architecture.
However, there was a problem with Socket.IO in C++ so the next simple architecture was used.
![system_software](wiki/images/system_software.png)

## Architecture without node.js
Since there was a problem, C++ process directly sends serial command to Arduino.
The disadvantage of this system is that we can't control the robot remotely.
![system_software2](wiki/images/system_software2.png)

# Contents

1. [Using motor driver(MD03A) with Arduino](https://github.com/mktk1117/six_wheel_robot/wiki/Using-motor-driver-with-Arduino)
2. [Hardware settings of six wheel platform](https://github.com/mktk1117/six_wheel_robot/wiki/Hardware-settings-of-six-wheel-platform)
3. [Raspberry Pi setup](https://github.com/mktk1117/six_wheel_robot/wiki/Raspberry-Pi-setup)
4. [ROS installation and setup for Raspberry Pi](https://github.com/mktk1117/six_wheel_robot/wiki/ROS-installation-and-setup-for-Raspberry-Pi)
5. [Communication between Arduino and Raspberry Pi](https://github.com/mktk1117/six_wheel_robot/wiki/Communication-between-Arduino-and-Raspberry-Pi)
6. [Communication between Raspberry Pi and PC (ROS)](https://github.com/mktk1117/six_wheel_robot/wiki/Communication-between-Raspberry-Pi-and-PC-(ROS))
7. [Communication between Raspberry Pi and PC (Node.js)](https://github.com/mktk1117/six_wheel_robot/wiki/Communication-between-Raspberry-Pi-and-PC-(Node.js))
7. [Make a ROS package to communicate with Arduino to control motors](https://github.com/mktk1117/six_wheel_robot/wiki/Make-a-ROS-package-to-communicate-with-Arduino-to-control-motors)
8. [Use raspberry pi camera module with OpenCV](https://github.com/mktk1117/six_wheel_robot/wiki/Use-raspberry-pi-camera-module-with-OpenCV)
9. [Black line detection with OpenCV](https://github.com/mktk1117/six_wheel_robot/wiki/Black-line-detection-with-OpenCV)
