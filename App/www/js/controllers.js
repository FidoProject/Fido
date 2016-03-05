angular.module('Fido.controllers', ['ngCordova', 'Fido.services'])

.controller('LoginCtrl', function($scope, $ionicPlatform, $fidoConnection, $state) {
	$scope.connectToFido = function() {
		$ionicPlatform.ready(function() {
			$fidoConnection.init();
			$fidoConnection.connect(function() {
				console.log("Did connect");
				$state.transitionTo('train');
			}, function() {
				console.log("Failed to connect");
			});
		});
	};
})

.controller('TrainCtrl', function($scope, $fidoConnection) {
	$scope.$fidoConnection = $fidoConnection;
});