
    <div ng-controller="ReportsAll">
        <div class="well">
            <form class="form-inline" role="form" ng-submit="filterReports()">
                <div class="form-group">
                    <label class="sr-only">Pass/Fail</label>
                    <select ng-change="checkSelected()" ng-model="pass_fail" class="form-control pass_fail">
                            <option value="">all</option>
                            <option value="1">Pass</option>
                            <option value="0">Fail</option>
                    </select>
                </div>
                <div class="form-group">
                    <label class="sr-only">Browser</label>
                    <select ng-change="checkSelected()" ng-model="browser" class="form-control"  ng-options="key as value for (key, value) in browsers">
                        <option value="">all</option>
                    </select>
                </div>
                <div class="form-group">
                    <input placeholder="Filename">
                </div>
                <div class="form-group">
                    <label class="sr-only">User</label>
                    <select  ng-change="checkSelected()" ng-model="user_id" class="form-control user" ng-options="key as value for (key, value) in users">
                        <option value="">all</option>
                    </select>
                </div>
                <div class="form-group">
                    <label class="sr-only">URL</label>
                    <select ng-model="url" ng-change="checkSelected()" class="form-control urls" ng-options="value as key for (key, value) in urls">
                        <option value="">all</option>
                    </select>
                </div><input type="submit" class="btn btn-warning" value="Search">
            </form>
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
            <tr ng-repeat="result in results" ng-class="{danger: result.status === '0'}">
                <td>
                    <span ng-switch on="result.status">
                        <span ng-switch-when="1">Pass</span>
                        <span ng-switch-when="0">Fail</span>
                    </span>
                </td>
                <td>{{result.filename}}</td>
                <td>{{result.module}}</td>
                <td>{{result.settings.browser_version}}</td>
                <td>{{result.created + '000' | date:'medium'}}</td>
                <td>{{result.settings.url}}</td>
            </tr>
            </tbody>
        </table>
    </div>

