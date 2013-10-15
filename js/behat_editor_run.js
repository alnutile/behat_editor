(function ($) {
    Drupal.behaviors.behat_editor_run = {

        attach: function (context) {
            var token = Drupal.behat_editor.get_token();
            $('a.run').click(function(e){
                e.preventDefault();
                var scenario = $('ul.scenario:eq(0) > li').not('.ignore');
                var url = $(this).attr('href');
                //See if I need to pass scenario
                if(url.split('/')[4] == 'run') {
                    var parameters = {};
                } else {
                    var scenario_array = Drupal.behat_editor.make_scenario_array(scenario);
                    var parameters = {
                        "scenario": scenario_array
                    };
                }
                var data = Drupal.behat_editor.action('POST', token, parameters, url);
                var callbacks = ["Drupal.behat_editor.renderMessage(data)"];
                Drupal.behat_editor.actions('POST', token, parameters, url, true, true, callbacks, context);
            });
        }
    };

})(jQuery);