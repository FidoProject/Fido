angular.module('Fido.controllers', ["cordova-plugin-bluetooth-serial"])

.controller('LoginCtrl', function($scope, $cordovaBarcodeScanner,$location) {
	bluetoothSerial.discoverUnpaired(function(devices) {
		console.log(devices);
	}, function() {
		console.log("fail")
	});
	$scope.connectToFido = function() {

	};
})

.controller('TestCtrl', function($scope, $cordovaBarcodeScanner,$location) {
	
});