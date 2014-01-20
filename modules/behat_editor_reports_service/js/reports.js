angular.module('behat_reports', []).
    config(function($routeProvider) {
        $routeProvider.
            when('/', {controller:ReportsAll, templateUrl: Drupal.settings.angularjsApp.basePath + 'behat_editor_reports_service/tpl/behat_editor_reports_service_reports_tpl'}).
            otherwise({redirectTo:'/'});
    });


function ReportsAll($scope, $http, $location, $route, $routeParams) {
    $scope.query = {};
    $scope.query.uid = null;
    $scope.location = {};


    var lastRoute = $route.current;
    $scope.$on('$locationChangeSuccess', function(event) {
        $route.current = lastRoute;
    });

    $scope.$on('$routeChangeSuccess', function(event, current, previous){
        console.log("New route");
        console.log("Event");
        console.log(event);
        console.log("Current");
        console.log(current);
    });


    if($routeParams.user_id) {
        $location.search('user_id', $routeParams.user_id);
        $scope.location.user_id = $routeParams.user_id;
    } else {
        $location.search('user_id', null);
        $scope.location.user_id = null;
    }

    var bar_url = Drupal.settings.angularjsApp.basePath;


    $http({method: 'GET', url: bar_url + '/behat_reports_v1/reports'}).
        success(function(data, status, headers, config) {
            $scope.results = data.results;
            $scope.browsers = data.browsers;
            $scope.users = data.users;
            $scope.urls = data.urls;

        }).
        error(function(data, status, headers, config) {
            // called asynchronously if an error occurs
            // or server returns response with an error status.
        });


//    $scope.$watch('query.uid', function(user_id) {
//        if(user_id) {
//            $location.search('user_id', user_id);
//        } else {
//            $location.search('user_id', '2');
//            $scope.query.uid = null;
//        }
//    });

    $scope.setLocation = function(locationToSet, valueScope) {
        $location.search('user_id', eval(valueScope));
    }

    $scope.$watch('query.settings', function(browser) {
        if(browser) {
            $location.search('browser', browser);
        } else {
            $location.search('browser', null);
        }
    });

    $scope.checkSelected = function(value, compare) {
        if (value == compare) {
            console.log("match " + value)
            return 'selected';
        } else {
            return false;
        }
    }

//    // Init local cache.
//    $scope.cache = {};

}
