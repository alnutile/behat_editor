'use strict';

var buildCharts = function($scope){
    var chart1 = {};

    chart1.type = "ColumnChart";
    chart1.displayed = true;
    chart1.cssStyle = "height:300px; width:100%;";
    var chart_browser_data = {"cols": [], "rows": []};

    chart_browser_data.cols.push({id: 'browser', label: 'Browser', type: "string"});
    chart_browser_data.cols.push({id: 'pass', label: "Pass", type: "number"});
    chart_browser_data.cols.push({id: 'fail', label: "Fail", type: "number"});

    angular.forEach($scope.browser_pass_fail_count, function(value, key){

        chart_browser_data.rows.push({c: [{v: key}, {v: value.pass }, {v: value.fail}]});

    });

    chart1.data =  chart_browser_data;
    chart1.options = {
        "title": "Per Browser Results",
        "isStacked": "true",
        "fill": 20,
        "displayExactValues": true,
        "vAxis": {
            "title": "Total Tests", "gridlines": {"count": 10}
        },
        "hAxis": {
            "title": "Browser"
        }
    };

    $scope.chart_browser_pass_fail = chart1;

    var chart2 = {};

    chart2.type = "PieChart";
    chart2.displayed = true;
    chart2.cssStyle = "height:300px; width:100%;";
    var chart_pass_fail = {"cols": [], "rows": []};

    chart_pass_fail.cols.push({id: 'status', label: 'Pass or Fail', type: "string"});
    chart_pass_fail.cols.push({id: 'pass', label: "Pass", type: "number"});
    chart_pass_fail.cols.push({id: 'fail', label: "Fail", type: "number"});
    chart_pass_fail.rows.push({c: [{v: 'Pass'}, {v: $scope.pass_fail_chart.pass }]});
    chart_pass_fail.rows.push({c: [{v: 'Fail'}, {v: $scope.pass_fail_chart.fail }]});

    chart2.data =  chart_pass_fail;
    chart2.options = {
        "title": "Pass Fail Total",
        "isStacked": "true",
        "fill": 20,
        "displayExactValues": true,
        "vAxis": {
            "title": "Total Tests", "gridlines": {"count": 10}
        },
        "hAxis": {
            "title": "Browser"
        }
    };

    $scope.chart_fail_pass = chart2;

    var chart3 = {};

    chart3.type = "ColumnChart";
    chart3.displayed = true;
    chart3.cssStyle = "height:300px; width:100%;";
    var chart_url_data = {"cols": [], "rows": []};

    chart_url_data.cols.push({id: 'url', label: 'URL', type: "string"});
    chart_url_data.cols.push({id: 'pass', label: "Pass", type: "number"});
    chart_url_data.cols.push({id: 'fail', label: "Fail", type: "number"});

    angular.forEach($scope.pass_fail_per_url, function(value, key){

        chart_url_data.rows.push({c: [{v: key}, {v: value.pass }, {v: value.fail}]});

    });

    chart3.data =  chart_url_data;
    chart3.options = {
        "title": "Per URL Results",
        "isStacked": "true",
        "fill": 20,
        "displayExactValues": true,
        "vAxis": {
            "title": "Total Tests", "gridlines": {"count": 10}
        },
        "hAxis": {
            "title": "URL"
        }
    };

    $scope.chart_url_pass_fail = chart3;


    return $scope;
};