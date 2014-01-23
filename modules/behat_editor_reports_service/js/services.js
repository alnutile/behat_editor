'use strict';

var reportServices = angular.module('reportServices', ['ngResource']);

reportServices.factory('ReportService', ['$resource',
    function($resource){
        return $resource('/behat_reports_v1/reports', {}, {
            query: {method:'GET', params:{user_id: 'user_id', browser: 'browser', url: 'url', filename: 'filename'}, isArray:false}
        });
    }]);