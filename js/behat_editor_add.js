(function ($) {
    Drupal.behaviors.behat_editor_add = {

        attach: function (context) {
            var token = Drupal.behat_editor.get_token();

            $('a.add').click(function(e){
                e.preventDefault();
                var filename = $('input[name=filename]').val();
                $('#beModal .filename').text(filename);
                $('#beModal').modal();
                e.preventDefault();
            });

            $('button.confirm-add').click(function(){
                $('#beModal').modal('hide');
                var add = $('a.add');
                var scenario = $('ul.scenario:eq(0) > li').not('.ignore');
                var scenario_array = Drupal.behat_editor.make_scenario_array(scenario);
                var filename = $('input[name=filename]').val();
                var module = $(add).data('module');
                var url = $(add).attr('href');
                var service_path = [module, filename];
                //Drupal.behat_editor.split_filename(filename),
                var parameters = {
                    "scenario": scenario_array,
                    "filename": filename,
                    "module": module,
                    "path": service_path
                };
                var data = Drupal.behat_editor.action('POST', token, parameters, url);
                if(data.error == 0) {
                    window.location.replace("/admin/behat/edit/" + module + "/" + filename);
                }
                Drupal.behat_editor.renderMessage(data);
            });
        }


    };

})(jQuery);