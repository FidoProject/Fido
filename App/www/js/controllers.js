angular.module('Fido.controllers', ['ngCordova', 'Fido.services'])

.controller('LoginCtrl', function($scope, $ionicPlatform, $fidoConnection, $state) {
	$scope.connectToFido = function() {
		$ionicPlatform.ready(function() {
			console.log("Ready");
			
			$fidoConnection.init();
			$fidoConnection.connect(function() {
				console.log("Did connect");
				console.log("Move to location");
				$state.transitionTo('train');
			}, function() {
				console.log("Failed to connect");
			});
		});
	};
})

.controller('TrainCtrl', function($scope, $fidoConnection) {
	console.log("Did move");
	$scope.reward = function() {
		console.log("reward")
		$fidoConnection.sendReward(1);
	};
	$scope.punish = function() {
		console.log("punish")
		$fidoConnection.sendReward(-1);
	};
	$scope.end = function() {
		console.log("end")
		$fidoConnection.sendReward(-2);
	};
});