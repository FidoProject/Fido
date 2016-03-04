angular.module('Fido.controllers', ['ngCordova', 'Fido.services'])

.controller('LoginCtrl', function($scope, $ionicPlatform, $fidoConnection, $location) {
	$scope.connectToFido = function() {
		$ionicPlatform.ready(function() {
			console.log("Ready");
			
			$fidoConnection.init();
			$fidoConnection.connect(function() {
				console.log("Did connect");
				console.log("Move to location");
				$fidoConnection.sendReward(1);
				$location.path('/train');
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
	}
});