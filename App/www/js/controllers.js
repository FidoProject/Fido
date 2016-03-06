angular.module('Fido.controllers', ['ngCordova', 'Fido.services'])

.controller('LoginCtrl', function($scope, $ionicPlatform, $fidoConnection, $state) {
	$scope.connectToFido = function() {
		$ionicPlatform.ready(function() {
			$fidoConnection.init();
			$fidoConnection.connect(function() {
				console.log("Did connect");
				$state.transitionTo('config');
			}, function() {
				console.log("Failed to connect");
			});
		});
	};
})

.controller('ConfigCtrl', function($scope, $fidoConnection) {	
	$scope.settings = {
		model: 'discrete'
	};

	$scope.goToTrain = function(){
		console.log($scope.settings.model);
		$fidoConnection.config($scope.settings);
		$state.transitionTo('train');
	}
})

.controller('TrainCtrl', function($scope, $fidoConnection) {
	$scope.$fidoConnection = $fidoConnection;
});