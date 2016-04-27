var SerialPort = require('serialport').SerialPort;

var http = require('http');
var express = require('express');
var app = express();
app.use(express.static(__dirname + '/public'));

var server = http.createServer(app);

server.listen(process.env.PORT || 3000);

var io = require('socket.io').listen(server);//we write "html listening to server" but relationship also goes both ways. server listens to html

var port = new SerialPort('COM4');
var connectionID = 0;

port.on('open', function () { //on "open" call this function (the constructor opens the port)

	//when any browser connects to the server, this function runs
	io.on('connection', function(socket){ //"on connection, call this function"
		console.log('a user connected');

		var currId = connectionID;

		connectionID++;

		//when that browser (socket) emits "hi", this function runs
		socket.on('hi', function(msg){ //on "hi", call this function
			console.log(currId);
			port.write([currId &0xFF], function(err, bytesWritten) {
				if (err) {
					return console.log('Error: ', err.message);
				}
			});
		});
	});

}); //put everything inside port.on so that we don't try to write to port before opening it


