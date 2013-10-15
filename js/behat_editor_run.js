(function ($) {

    Drupal.behat_editor.run_actions = function(type, token, parameters, url, async, globals, context){
        $.ajax({
                type: type,
                beforeSend: function (request) {
                    request.setRequestHeader("X-CSRF-Token", token);
                },
                url: url,
                data: JSON.stringify(parameters),
                dataType: "json",
                async: async,
                global: globals,
                contentType: 'application/json'
            }
        ).done(function(data){
                results = data;
                if($('#past-results-table').length) {
                    callbacks = ["Drupal.behat_editor.output_results(results, 'row')", "Drupal.behat_editor.results_modal(context)"];
                    Drupal.behat_editor.get_results(context, callbacks);
                };
                Drupal.behat_editor.renderMessage(data);
            });
    };


    Drupal.behaviors.behat_editor_run = {

        attach: function (context) {
            var token = Drupal.behat_editor.get_token();
            $('a.run').click(function(e){
                $('#edit-container1 a.collapsed').click();
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
                Drupal.behat_editor.run_actions('POST', token, parameters, url, true, true, context);
            });
        }
    };

})(jQuery);