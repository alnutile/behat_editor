angular.module('behat_reports', []).
    config(function($routeProvider) {
        $routeProvider.
            when('/', {controller:ReportsAll, templateUrl: Drupal.settings.angularjsApp.basePath + '/behat_editor_reports_service/tpl/behat_editor_reports_service_reports_tpl'}).
            otherwise({redirectTo:'/'});
    });


function ReportsAll($scope, $http) {
    console.log("here");
    var bar_url = Drupal.settings.angularjsApp.basePath;
    $http({method: 'GET', url: bar_url + '/behat_reports_v1/reports'}).
        success(function(data, status, headers, config) {
            console.log(data);
            // this callback will be called asynchronously
            // when the response is available
        }).
        error(function(data, status, headers, config) {
            // called asynchronously if an error occurs
            // or server returns response with an error status.
        });

    // Init local cache.
    $scope.cache = {};

}
