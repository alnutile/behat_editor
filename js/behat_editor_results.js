(function ($) {
    Drupal.behaviors.behat_editor_results = {

        attach: function (context) {
            var token = Drupal.behat_editor.get_token();
            var filename = $('input[name=filename]').val();
            var module = $('input[name=module]').val();
            var url = '/behat_editor_v1/behat_editor_actions/feature/results/' + module + '/' + filename;
            var parameters = {};
            var results = Drupal.behat_editor.action('POST', token, parameters, url);
            if(results['error'] === 0) {
                var table = new Array();
                var rows = results['data'];
                $.each(rows, function(key, value){
                    var date = new Date(value["created"]*1000);
                    var status;
                    if(value['status'] == 0 ) {
                        status = '<i class="glyphicon glyphicon-thumbs-up"></i>';
                    } else {
                        status = '<i class="glyphicon glyphicon-thumbs-down"></i>';
                    }
                    table[key] = [status, value["duration"], date.format('Y-m-d H:i'), value["rid"]]
                });

                $('#past-results-table').dataTable(
                    {
                        "aaData": table,
                        "aoColumns": [
                            {"sTitle": "Results"},
                            {"sTitle": "Duration"},
                            {"sTitle": "Date"},
                            {"sTitle": "View"}

                        ]
                    }
                );
            } else {
                Drupal.renderMessageCustom($results['message'], 'warning')
            }
       }


    };

})(jQuery);