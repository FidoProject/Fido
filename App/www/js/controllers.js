angular.module('Fido.controllers', ['ngCordova', 'Fido.services'])

.controller('LoginCtrl', function($scope, $window, $ionicPlatform, $fidoConnection) {
		$ionicPlatform.ready(function() {
			console.log("Ready");
			$fidoConnection.init();
		});
})

.controller('TestCtrl', function($scope) {
		
});