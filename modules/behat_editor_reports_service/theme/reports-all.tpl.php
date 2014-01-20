
    <div ng-controller="ReportsAll">
        <form class="form-inline" role="form">
            <div class="form-group">
                <label>Pass/Fail</label>
                <select ng-model="query.status" class="form-control">
                        <option value="1">Pass</option>
                        <option value="0">Fail</option>
                </select>
            </div>
            <div class="form-group">
                <label>Browser</label>
                <select ng-model="query.settings" class="form-control">
                    <option ng-repeat="browser in browsers" value="{{browser}}">{{browser}}</option>
                </select>
            </div>
            <div class="form-group">
                <input ng-model="query.filename" placeholder="Filename">
            </div>
            <div class="form-group">
                <label>User</label>
                <select ng-model="query.uid" class="form-control">
                    <option ng-repeat="user in users" value="{{user.uid}}">{{user.mail}}</option>
                </select>
            </div>
            <div class="form-group">
                <label>URL</label>
                <select ng-model="query.settings" class="form-control">
                    <option ng-repeat="url in urls" value="{{url.nice_name}}">{{url.nice_name}}</option>
                </select>
            </div>
        </form>

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
            <tr ng-repeat="result in results | filter:query" ng-class="{danger: result.status === '0'}">
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
                <td>{{result.settings.url}}</td>
            </tr>
            </tbody>
        </table>
    </div>

