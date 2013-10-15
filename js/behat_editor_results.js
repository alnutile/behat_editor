(function ($) {
    Drupal.behaviors.behat_editor_results = {

        attach: function (context) {
            var token = Drupal.behat_editor.get_token();
            var url = '/behat_editor_v1/behat_editor_actions/feature/results/behat_editor/wikipedia';
            var parameters = {};
            var results = Drupal.behat_editor.action('POST', token, parameters, url);
            if(results['error'] === 0) {
                $('#past-results-table').dataTable(

                    {
                        "aaData": results['data'],
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