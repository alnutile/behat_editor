'use strict';

var reportsController = angular.module('reportsController', ['googlechart']);

reportsController.controller('Report', ['$scope', '$http', '$location', '$route', '$routeParams',
    function($scope, $http, $location, $route, $routeParam, googlechart){

    }]);

reportsController.controller('ReportsAll', ['$scope', '$http', '$location', '$route', '$routeParams', 'ReportService', '$sce', 'ngTableParams',
    function($scope, $http, $location, $route, $routeParams, ReportService, $sce, ngTableParams){

        $scope.query = {};
        $scope.query.uid = null;
        $scope.location = {};
        $scope.filename = null;
        $scope.status_state = ['Fail', 'Pass'];
        $scope.current_page = 1;

        var setData = function (data) {
            $scope.results = data.results;
            $scope.max = 500;
            $scope.total_found = $scope.results.length;
            $scope.total_page = Math.round($scope.results.length / $scope.max);
            $scope.results_table = data.results.slice(0, $scope.max);
            $scope.browsers = data.browsers;
            $scope.users = data.users;
            $scope.urls = data.urls;
            $scope.browser_pass_fail_count = data.browser_pass_fail_count;
            $scope.pass_fail_chart = data.pass_fail_chart;
            $scope.pass_fail_per_url = data.pass_fail_per_url;
            $scope.tags = data.tags;
//            $scope.tableParams = new ngTableParams({
//                page: 1,
//                count: 5
//            }, {
//                total: $scope.results.length,
//                getData: function ($defer, params) {
//                    $defer.resolve($scope.results.slice((params.page() - 1) * params.count(), params.page() * params.count()));
//                }
//            });
        };

        $scope.getNext = function () {
          var next = ($scope.current_page - 1) + $scope.max;
          var upto = next + $scope.max;
          $scope.results_table = $scope.results.slice(next, upto);
            $scope.current_page = $scope.current_page + 1;
        };

        $scope.getBack = function () {
            var back = ($scope.current_page * $scope.max) - $scope.max;
            var upto = back + $scope.max;
            $scope.results_table = $scope.results.slice(back, upto);
            $scope.current_page = $scope.current_page - 1;
        };

        $scope.getReports = function(params) {
            var params = params;
            if (Object.keys(params).length > 0) {
                $scope.getFilteredSet(params);
            } else {
                ReportService.query({}, function (data) {
                    setData(data);
                    buildCharts($scope);
                });
            }
        };

        $scope.getFilteredSet = function (params) {

            ReportService.get({
                user_id: params.user_id,
                browser: params.browser,
                pass_fail: params.pass_fail,
                filename: params.filename,
                tag: params.tag,
                url: params.url,
                tag_name: params.tag_name
            }, function (data) {
                setData(data, params);
                buildCharts($scope);
            });
        };

        //Render page on load
        $scope.getReports($routeParams);

        //Maybe this is overkill
        var swapNullForAll = function (value) {
            var value = value;
            if (value === null || value === '') {
                return 'all';
            } else {
                return value;
            }
        }

        //React to search
        $scope.filterReports = function(){
            var params = {};
            $location.search('filename', swapNullForAll(this.filename));
            params.browser = swapNullForAll(this.browser);
            params.page = (this.page) ? this.page : 1;
            params.url = swapNullForAll(this.url);
            params.user_id = swapNullForAll(this.user_id);
            params.pass_fail = swapNullForAll(this.pass_fail);
            params.filename = swapNullForAll(this.filename);
            params.tag_name = swapNullForAll(this.tag_name);
            $scope.getReports(params);
        };

        $scope.browser = $routeParams.browser;
        $scope.user_id = $routeParams.user_id;
        $scope.pass_fail = $routeParams.pass_fail;
        $scope.tag_name = $routeParams.tag_name;
        $scope.url = $routeParams.url;

        $scope.checkSelected = function() {
            $location.search('browser', swapNullForAll(this.browser));
            $location.search('pass_fail', swapNullForAll(this.pass_fail));
            $location.search('url', swapNullForAll(this.url));
            $location.search('user_id', swapNullForAll(this.user_id));
            $location.search('tag_name', swapNullForAll(this.tag_name));
        };

    }]);
