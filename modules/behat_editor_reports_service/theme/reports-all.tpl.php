
    <div ng-controller="ReportsAll">


            <div class="row well">
                <span class="col-md-4">
                    <h2>Browsers Being Tested</h2>
                    <p>
                        <ul>
                            <li ng-repeat="(key, value) in browser_pass_fail_count"> {{ key }}</li>
                        </ul>
                    </p>
                </span>
                <span class="col-md-4">
                    <h2>Pass Fail Summary</h2>
                    <p>
                        <ul>
                            <li>Pass: {{pass_fail_chart.pass}}</li>
                            <li>Fail: {{pass_fail_chart.fail}}</li>
                        </ul>
                    </p>
                </span>
                <span class="col-md-4">
                    <h2>URLs Being Tested</h2>
                    <p>
                        <ul>
                            <li ng-repeat="(key, value) in pass_fail_per_url"> {{ key }}</li>
                        </ul>
                    </p>
                </span>
             </div>

            <div class="row well">
                <div class="col-md-4">
                    <div google-chart chart="chart_url_pass_fail" style="{{chart_url_pass_fail.cssStyle}}" on-ready="chartReady()"></div><!-- explicit close of tag seems to be necessary -->
                </div>

                <div class="col-md-4">
                    <div google-chart chart="chart_browser_pass_fail" style="{{chart_browser_pass_fail.cssStyle}}" on-ready="chartReady()"></div><!-- explicit close of tag seems to be necessary -->
                </div>

                <div class="col-md-4">
                    <div google-chart chart="chart_fail_pass" style="{{chart_fail_pass.cssStyle}}" on-ready="chartReady()"></div><!-- explicit close of tag seems to be necessary -->
                </div>
            </div>

        <div class="well">
            <div class="form-group">
                <input ng-model="query.all" placeholder="Quick Search" id="quicksearch">
            </div>
            <form class="form-inline" role="form" ng-submit="filterReports()">
                <div class="form-group">
                    <label class="sr-only">Pass/Fail</label>
                    <select ng-change="checkSelected()" ng-model="pass_fail" class="form-control pass_fail" ng-options="key as value for (key, value) in status_state">
                            <option value="">all</option>
                    </select>
                </div>
                <div class="form-group">
                    <label class="sr-only">Browser</label>
                    <select ng-change="checkSelected()" ng-model="browser" class="form-control"  ng-options="key as value for (key, value) in browsers">
                        <option value="">all</option>
                    </select>
                </div>
                <div class="form-group">
                    <input ng-change="checkChange()" ng-model="filename" placeholder="Filename">
                </div>
                <div class="form-group">
                    <label class="sr-only">User</label>
                    <select  ng-change="checkSelected()" ng-model="user_id" class="form-control user" ng-options="key as value for (key, value) in users">
                        <option value="">all</option>
                    </select>
                </div>
                <div class="form-group">
                    <label class="sr-only">URL</label>
                    <select ng-model="url" ng-change="checkSelected()" class="form-control urls" ng-options="key as value for (key, value) in urls | orderBy: key ">
                        <option value="">all</option>
                    </select>
                </div><input type="submit" class="btn btn-warning" value="Search"> | <a href="/admin/behat/reports">reset</a>
            </form>
        </div>

        <div class="well" ng-class="{hidden: total_found < 501}">
            Current Page: {{current_page}}</br>
            Total Pages: {{total_page}}<br>
            Per Page: {{max}}<br>

            <button class="btn btn-warning" ng-click="getNext()">Next</button> |
            <button class="btn btn-warning" ng-click="getBack()" ng-class="{disabled: current_page == 1}">Back</button>
        </div>
        <table
            class="table table-bordered table-hover">
            <thead>
            <tr>
                <th>Status</th>
                <th>Filename</th>
                <th>Module</th>
                <th>Browser</th>
                <th>Created</th>
                <th>URL</th>
            </tr>
            </thead>
            <tbody>
                <tr ng-repeat="result in results_table | filter:query.all" ng-class="{danger: result.status === '0'}">
                    <td data-title="'Status'">
                        <i class="{{result.status|passfail}}"></i>
                    </td>
                    <td data-title="'Filename'">{{result.filename}}</td>
                    <td data-title="'Module'">{{result.module}}</td>
                    <td data-title="'Browser'">{{result.settings.browser_version}}</td>
                    <td data-title="'Created'">{{result.created + '000' | date:'medium'}}</td>
                    <td data-title="'URL'">{{result.nice_name}}</td>
                </tr>
            </tbody>
        </table>

    </div>
