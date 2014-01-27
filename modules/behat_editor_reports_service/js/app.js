'use strict';

var behat_reports = angular.module('behat_reports', [
    'ngRoute',
    'reportsController',
    'reportServices',
    'reportFilters',
    'ngSanitize',
    'ngTable'
]);

behat_reports.config(['$routeProvider',
    function ($routeProvider) {
        $routeProvider.
            when('/', {
                templateUrl: '/behat_editor_reports_service/tpl/behat_editor_reports_service_reports_tpl',
                controller: 'ReportsAll'
            }).
            when('/details/:rid', {
                templateUrl: '/behat_editor_reports_service/tpl/behat_editor_reports_service_reports_tpl',
                controller: 'Report'
            }).
            otherwise({
                redirectTo: '/'
            });
    }]);
