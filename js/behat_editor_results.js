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
                    table[key] = [value["filename"], value["module"], value["duration"], date.format('Y-m-d H:i')]
                });

                $('#past-results-table').dataTable(
                    {
                        "aaData": table,
                        "aoColumns": [
                            {"sTitle": "Filename"},
                            {"sTitle": "Module"},
                            {"sTitle": "Duration"},
                            {"sTitle": "Date"}
                        ]
                    }
                );
            } else {
                Drupal.renderMessageCustom($results['message'], 'warning')
            }
       }


    };

})(jQuery);