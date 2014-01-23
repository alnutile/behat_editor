'use strict';

var reportsController = angular.module('reportsController', ['googlechart']);

reportsController.controller('Report', ['$scope', '$http', '$location', '$route', '$routeParams',
    function($scope, $http, $location, $route, $routeParam, googlechart){

    }]);

reportsController.controller('ReportsAll', ['$scope', '$http', '$location', '$route', '$routeParams', 'ReportService',
    function($scope, $http, $location, $route, $routeParams, ReportService){


        $scope.query = {};
        $scope.query.uid = null;
        $scope.location = {};

        //Prevent Reload on Location update
        //So the user has to click submit
//        var lastRoute = $route.current;
//        $scope.$on('$locationChangeSuccess', function(event) {
//            $route.current = lastRoute;
//        });
        $scope.status_state = ['Fail', 'Pass'];

        $scope.getReports = function(params) {
            var params = params;
            ReportService.get({
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
                $scope.filename = params.filename;
                $scope.browser_pass_fail_count = data.browser_pass_fail_count;
                $scope.pass_fail_chart = data.pass_fail_chart;
                $scope.pass_fail_per_url = data.pass_fail_per_url;
                //@TODO move into own service or directive
                buildCharts($scope);
                //END CHARTS
            });
        };

        //Maybe this is overkill
        var swapNullForAll = function(value) {
            var value = value;
            if(value == null || value == '') {
                return 'all';
            } else {
                return value;
            }
        }

        $scope.getReports($routeParams);

        //React to search
        $scope.filterReports = function(){
            var params = {};
            $location.search('filename', swapNullForAll(this.filename));
            params.browser = swapNullForAll(this.browser);
            params.url = swapNullForAll(this.url);
            params.user_id = swapNullForAll(this.user_id);
            params.pass_fail = swapNullForAll(this.pass_fail);
            params.filename = swapNullForAll(this.filename);
            $scope.getReports(params);
        };


        $scope.browser = $routeParams.browser;
        $scope.user_id = $routeParams.user_id;
        $scope.pass_fail = $routeParams.pass_fail;
        $scope.url = $routeParams.url;

        $scope.checkSelected = function() {
            $location.search('browser', swapNullForAll(this.browser));
            $location.search('pass_fail', swapNullForAll(this.pass_fail));
            $location.search('url', swapNullForAll(this.url));
            $location.search('user_id', swapNullForAll(this.user_id));
        };




    }]);
