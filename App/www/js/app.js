angular.module('Fido', ['ionic', 'ngCordova', 'Fido.controllers'])

.run(function($ionicPlatform) {
	$ionicPlatform.ready(function() {
		if(window.cordova && window.cordova.plugins.Keyboard) {
			cordova.plugins.Keyboard.hideKeyboardAccessoryBar(true);

			cordova.plugins.Keyboard.disableScroll(true);
		}
		if(window.StatusBar) {
			StatusBar.styleDefault();
		}
	});
})

.config(function($stateProvider, $urlRouterProvider) {

	$stateProvider

	.state('login', {
		url: '/login',
		templateUrl: 'templates/login.html',
		controller: 'LoginCtrl'
	})

	.state('config', {
		url: '/config',
		templateUrl: 'templates/config.html',
		controller: 'ConfigCtrl'
	})

	.state('train', {
		url: '/train',
		templateUrl: 'templates/train.html',
		controller: 'TrainCtrl'
	})

	// if none of the above states are matched, use this as the fallback
	$urlRouterProvider.otherwise('/login');

});