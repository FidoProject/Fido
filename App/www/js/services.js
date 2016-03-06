var FIDO_IP = "192.168.1.5";
var FIDO_PORT = 2008;

var RESET = 1;
var BEGIN = 2;
var STOP = 3;
var REWARD = 4;

angular.module('Fido.services', ['ngCordova'])

.service('$fidoConnection', function($ionicPlatform) {
	var socket = null;
	var data = new Uint8Array;

	var init = function() {
		socket = new Socket();
		console.log(socket);
		socket.onData = function(buffer) {
			data = data.concat(buffer);
		};
		socket.onError = function() {
			console.log("There was an error in the connection.");
		};
		socket.onClose = function() {
			console.log("The socket connection was closed.");
		};
	};

	var config = function(settings) {
		sendString("MODEL: " + settings.model);
	};

	var sendString = function(string) {
		string += "\n";
		var data = new Uint8Array(string.length);
		for (var i = 0; i < data.length; i++) {
			data[i] = string.charCodeAt(i);
		}
		socket.write(data);
	};

	var sendNumber = function(num) {
		sendString(num+"");
	};
	
	var connect = function(success, failure) {
		socket.open(FIDO_IP, FIDO_PORT, success, failure);
	};

	var disconnect = function() {
		socket.shutdownWrite();
		socket.close();
	};

	var resetModel = function() {
		sendNumber(RESET);
	};

	var beginLearning = function() {
		sendNumber(BEGIN);
	};

	var stopLearning = function() {
		sendNumber(STOP);
	};

	var sendReward = function(rewardVal) {
		sendNumber(REWARD);
		sendNumber(rewardVal);
	};
	
	return {
		init: init,
		connect: connect,
		disconnect: disconnect,
		resetModel: resetModel,
		beginLearning: beginLearning,
		stopLearning: stopLearning,
		sendReward: sendReward
	};
});