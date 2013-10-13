(function ($) {
    Drupal.behaviors.behat_editor_add = {

        attach: function (context) {
            var token = Drupal.behat_editor.get_token();

            $('a.add').click(function(e){
                e.preventDefault();
                var scenario = $('ul.scenario:eq(0) > li').not('.ignore');
                var scenario_array = Drupal.behat_editor.make_scenario_array(scenario);
                var filename = $(this).data('filename');
                var module = $(this).data('module');
                var url = $(this).attr('href');
                var parameters = {
                    "scenario": scenario_array,
                    "filename": filename,
                    "module": module
                };
                var data = Drupal.behat_editor.action('POST', token, parameters, url);
                if(data.error == 0) {
                    alert("You will be redirected to the Edit page for file " + filename);
                    window.location.replace("/admin/behat/edit/" + module + "/" + filename + ".feature");
                }
                Drupal.behat_editor.renderMessage(data);
            });
        }


    };

})(jQuery);