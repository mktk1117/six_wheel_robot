var http = require('http');
var socketio = require('socket.io');
var fs = require('fs');
var path = require('path')
var server = http.createServer(function(req, res) {
    res.writeHead(200, {'Content-Type' : 'text/html'});
    res.end(fs.readFileSync(__dirname + '/index.html', 'utf-8'));
}).listen(3000); 

var io = socketio.listen(server);

var auto_control = false;

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
        // S06. server_to_clientイベント・データを送信する
        var linear = data.linear;
        var angular = data.angular;
        var right = (linear.x + angular.z) * 50;
        var left = (linear.x - angular.z) * 50;
        var command = right + "," + left + "," + right + "," + left + "," + right + "," + left + "*";
        if(auto_control == true){
            io.to("remotecontrol").emit('cmd_vel', data);
            // sp.write(command);
        }
        else if(data.from == "remotecontrol"){
            io.to("remotecontrol").emit('cmd_vel', data);
            // sp.write(command);
        }
        else{
            var command = "0,0,0,0,0,0*";
            // sp.write(command);
        }


    });

    socket.on('startstop', function(data) {
        var startstop_flag = data.flag;
        if(startstop_flag == "start"){
            auto_control = true;
        }
        else{
            auto_control = false;
            var command = "0,0,0,0,0,0*";
            // sp.write(command);
        }
    });

    // recieve cmd_vel
    socket.on('transfer', function(data) {
        // S06. server_to_clientイベント・データを送信する
        var transfer_data = data.data;
        var dstroom = data.room;
        var eventname = data.event;
        io.to(dstroom).emit(eventname, {data : transfer_data});
    });

    socket.on('transfer_message', function(data) {
        // S06. server_to_clientイベント・データを送信する
        var dstroom = data.room;
        var eventname = data.event;
        var message = data.message
        io.to(dstroom).emit(eventname, {message : message});
    });

    // S09. dicconnectイベントを受信し、退出メッセージを送信する
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
 
