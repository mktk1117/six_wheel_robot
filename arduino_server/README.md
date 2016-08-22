# Node.js server for Arduino control
In here, we use Node.js and Socket.IO to communicate between Raspberry Pi and PC.
In addition, it can be used to communicate between other process in Raspberry Pi. 

In this project, image processing is done by C++ and OpenCV and, Node.js server recieves the control 
signal from image processing. At the same time, the server can recieve the signal from other PC, 
so the user can stop or control the vehicle remotely.

##What this server does
- Build a http server
- Wait connection from client 
- After connection, receives an JSON message with the eventname "cmd_vel"
- Translate the cmd_vel message to Arduino command
- Send command to Arduino with serial

##Usage
First, download this folder.
```bash
svn export https://github.com/mktk1117/six_wheel_robot/trunk/arduino_server
```
Then, you have to install socket.io and node-serialport.
```bash
cd arduino_server
npm install socket.io
npm install serialport
```
Run the server
```bash
node app.js
```

##Room Name definition
In this server, room is used to select who to send.  
The name is like below.
- C++ program: `autocontrol`
- web client: `remotecontrol`

## Event Name
### enter_room
To join in the room, the client has to send messages of its room name.
- event name: `enter_room`
- key name of the JSON data: `room`

It can be sent like this.
```javascript
socket.emit("enter_room", {room : "remotecontrol"});
```
In here, the client can be connected in the room `remotecontrol`.

### cmd_vel
This event is to control the vehicle with cmd_vel message.
This message contains linear velocity and angular velocity.

cmd_vel
- linear
	- x
	- y
	- z
- angular
	- x
	- y
	- z

It can be decoded like this.
```javascript
socket.on('cmd_vel', function(data) {
        var linear = data.linear;
        var angular = data.angular;
        console.log("linear = {" + linear.x + ", " + linear.y + "," + linear.z + "}");
        console.log("angular = {" + angular.x + ", " + angular.y + "," + angular.z + "}");
    });
```
### transfer
By using this event data can be transferred to other room.

- event name: `transfer`
- JSON data:
	- `room`: name of the destination room
	- `eventname`: name of the event sent to the room
	- `data`: data to be sent

	

