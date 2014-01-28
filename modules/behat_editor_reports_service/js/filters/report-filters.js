'use strict';

angular.module('reportFilters', [])
    .filter('passfail', function () {
        return function (status) {
            var icon;
            if (status === '0') {
                icon = "glyphicon glyphicon-minus-sign";
            } else {
                icon = "glyphicon glyphicon-ok-sign";
            }
            return icon;
        }
    });

