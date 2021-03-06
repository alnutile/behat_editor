(function ($) {
  Drupal.behaviors.behat_editor_save = {
        attach: function (context) {
                var token = Drupal.behat_editor.get_token();
                $('a.save').click(function(e){
                    e.preventDefault();

                    //look for ace editor
                    if ( $('#test-textbox').length ) {
                        var scenario = Drupal.ace.editor.getValue();
                        var scenario_array = scenario.split("\n");
                    } else {
                        var scenario = $('ul.scenario:eq(0) > li').not('.ignore');
                        var scenario_array = Drupal.behat_editor.make_scenario_array(scenario);
                    }
                    var url = $(this).attr('href');
                    var url_args = window.location.pathname;
                    var url_args_array = url_args.split('/');
                    var service_path = url_args_array.slice(4, url_args_array.length);
                    var module = url_args_array[4];
                    var filename = url_args_array[url_args_array.length - 1];
                    var parameters = {
                        "scenario": scenario_array,
                        "settings":
                            {
                                "module": module,
                                "filename": filename,
                                "path": service_path
                            }
                    };
                    var data = Drupal.behat_editor.action('PUT', token, parameters, url);
                    Drupal.behat_editor.renderMessage(data, true);
                });
            }
        };
})(jQuery);