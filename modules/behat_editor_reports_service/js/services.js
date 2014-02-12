'use strict';

var reportServices = angular.module('reportServices', ['ngResource']);

reportServices.factory('ReportService', ['$resource',
    function($resource){
        return $resource('/behat_reports_v1/reports', {}, {
            query: {method:'GET', params:{user_id: 'all', browser: 'all', url: 'all', filename: 'all', 'pass_fail': 'all', 'tag_name': 'all', 'page': '1'}, isArray:false}
        });
    }]);