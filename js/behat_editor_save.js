(function ($) {
  Drupal.behaviors.behat_editor_save = {
        attach: function (context) {
                var token = Drupal.behat_editor.get_token();
                $('a.save').click(function(e){
                    e.preventDefault();
                    var scenario = $('ul.scenario:eq(0) > li').not('.ignore');
                    var scenario_array = Drupal.behat_editor.make_scenario_array(scenario);
                    var url = $(this).attr('href');
                    var parameters = {
                        "scenario": scenario_array
                    };
                    var data = Drupal.behat_editor.action('PUT', token, parameters, url);
                    Drupal.behat_editor.renderMessage(data);
                });
            }
        };
})(jQuery);