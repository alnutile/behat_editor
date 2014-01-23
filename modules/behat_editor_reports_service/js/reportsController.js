'use strict';

var reportsController = angular.module('reportsController', []);

reportsController.controller('Report', ['$scope', '$http', '$location', '$route', '$routeParams',
    function($scope, $http, $location, $route, $routeParam){
        $http.get('/behat_reports_v1/reports').success(function(data) {
            $scope.report = data;
        });
    }]);

reportsController.controller('ReportsAll', ['$scope', '$http', '$location', '$route', '$routeParams', 'Report',
    function($scope, $http, $location, $route, $routeParams, Report){
        $scope.query = {};
        $scope.query.uid = null;
        $scope.location = {};

        //Prevent Reload on Location update
        //So the user has to click submit
        var lastRoute = $route.current;
        $scope.$on('$locationChangeSuccess', function(event) {
            $route.current = lastRoute;
        });

        $scope.getReports = function(params) {
            var params = params;
            Report.get({
                user_id: params.user_id,
                browser: params.browser,
                pass_fail: params.pass_fail,
                filename: params.filename,
                tag: params.tag,
                url: params.url
            }, function(data){
                $scope.results = data.results;
                $scope.browsers = data.browsers;
                $scope.users = data.users;
                $scope.urls = data.urls;
            });
        };

        //Maybe this is overkill
        var swapNullForAll = function(value) {
            var value = value;
            if(value == null) {
                return 'all';
            } else {
                return value;
            }
        }

        //Trigger load of data to start
        $scope.getReports($routeParams);

        //React to search
        $scope.filterReports = function(){
            var params = {};
            params.browser = swapNullForAll(this.browser);
            params.url = swapNullForAll(this.url);
            params.user_id = swapNullForAll(this.user_id);
            params.pass_fail = swapNullForAll(this.pass_fail);
            params.filename = swapNullForAll(this.filename);
            console.log(params);
            console.log($routeParams);
            $scope.getReports(params);
        };


        $scope.browser = $routeParams.browser;
        $scope.user_id = $routeParams.user_id;
        $scope.pass_fail = $routeParams.pass_fail;
        $scope.url = $routeParams.url;

        $scope.checkSelected = function() {
            $location.search('fiilename', swapNullForAll($scope.filename));
            $location.search('browser', swapNullForAll($scope.browser));
            $location.search('pass_fail', swapNullForAll($scope.pass_fail));
            $location.search('url', swapNullForAll($scope.url));
            $location.search('user_id', swapNullForAll($scope.user_id));
        };

    }]);
