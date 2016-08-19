var http = require('http');
var socketio = require('socket.io');
var fs = require('fs');
var server = http.createServer(function(req, res) {
    res.writeHead(200, {'Content-Type' : 'text/html'});
    res.end(fs.readFileSync(__dirname + '/index.html', 'utf-8'));
}).listen(3000); 

var serialPort = require("serialport")
var sp = new serialPort.SerialPort("/dev/ttyUSB0", {
  baudrate: 115200,
  parser:serialPort.parsers.readline("\n")
});

// sp.on("open", function () {
//   console.log('open');
//   setTimeout(function() {
//     sp.write("Hello "); 
//     sp.write("world\n");
//     console.log('Sended msg'); 
//   }, 2000); 
// });

  sp.on('data', function(data) {
  console.log('data received: ' + data);
});
 
var io = socketio.listen(server);


// connection
io.sockets.on('connection', function(socket) {
    console.info("connected");
    var room = '';
    var name = '';
 
    // join to the room
    socket.on('enter_room', function(data) {
        room = data.room;
        socket.join(room);
        console.log("client joined the room :" + room);
    });

    // recieve cmd_vel
    socket.on('cmd_vel', function(data) {
        var linear = data.linear;
        var angular = data.angular;
        var right = (linear.x + angular.z) * 50;
        var left = (linear.x - angular.z) * 50;
        var command = right + "," + left + "," + right + "," + left + "," + right + "," + left + "*";

        // send cmd_vel to remotecontrol client
        io.to("remotecontrol").emit('cmd_vel', data);
        // send command to Arduino
        sp.write(command);
    });

    // transfer data to other room
    socket.on('transfer', function(data) {
        var transfer_data = data.data;
        var dstroom = data.room;
        var eventname = data.event;
        io.to(dstroom).emit(eventname, {data : transfer_data});
    });

    // transfer messaget to other room
    socket.on('transfer_message', function(data) {
        var dstroom = data.room;
        var eventname = data.event;
        var message = data.message
        io.to(dstroom).emit(eventname, {message : message});
    });

    // receive disconnection
    socket.on('disconnect', function() {
        if (room == '') {
            console.log("disconnected without entering a room");
        } else {
            var endMessage = "disconnected from " + room
            console.log(endMessage);     
            io.to(room).emit('server_to_client', {value : endMessage});
        }
    });
});
 
