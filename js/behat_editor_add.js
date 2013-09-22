(function ($) {
    Drupal.behaviors.behat_editor_add = {

        attach: function (context) {

            $('a.run').click(function(e){
                e.preventDefault();
                var method = 'create-mode';
                var scenario = $('ul.scenario:eq(0) li');
                var scenario_array = Drupal.behat_editor.make_scenario_array(scenario);
                var url = $(this).attr('href');
                var parameters = {
                    "method": method,
                    "scenario[]": scenario_array,
                };
                $.post(url, parameters, function(data){
                    console.log(data);
                    Drupal.behat_editor.renderMessage(data);
                }, "json");
            });
        }
    };

})(jQuery);