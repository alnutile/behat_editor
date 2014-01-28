(function ($) {
    Drupal.behaviors.behat_editor_clone = {

        attach: function (context) {
            var token = Drupal.behat_editor.get_token();

            $('a.clone').click(function(e){
                var filename = $(this).data('filename');
                $('#beModalClone .filename').text(filename);
                $('#clone-name').val(filename);
                $('#beModalClone').modal();
                e.preventDefault();
            });

            $('button.confirm-clone').click(function(e){
                $('#beModalClone').modal('hide');
                var clone = $('a.clone');
                var scenario = $('ul.scenario:eq(0) > li').not('.ignore');

                var url_args = window.location.pathname;
                var url_args_array = url_args.split('/');

                var scenario_array = Drupal.behat_editor.make_scenario_array_from_view(scenario);
                var filename = $('#clone-name').val();
                var module = $(clone).data('module');
                var url = $(clone).attr('href');
                var service_path = [module, filename];

                var parameters = {
                    "scenario": scenario_array,
                    "filename": filename,
                    "module": module,
                    "path": service_path,
                    "clone": url_args_array
                };
                var data = Drupal.behat_editor.action('POST', token, parameters, url);
                if (data.error === 0) {
                    window.location.replace("/admin/behat/edit/" + module + "/" + filename);
                }
                Drupal.behat_editor.renderMessage(data);
            });
        }


    };

})(jQuery);