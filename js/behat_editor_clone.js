(function ($) {
    Drupal.behaviors.behat_editor_clone = {

        attach: function (context) {
            var token = Drupal.behat_editor.get_token();

            $('a.clone').click(function(e){
                var filename = $(this).data('filename');
                console.log(filename);
                $('#beModalClone .filename').text(filename);
                $('#clone-name').val(filename);
                $('#beModalClone').modal();
                e.preventDefault();
            });

            $('button.confirm-clone').click(function(e){
                $('#beModalClone').modal('hide');
                var clone = $('a.clone');
                var scenario = $('ul.scenario:eq(0) > li').not('.ignore');
                var scenario_array = Drupal.behat_editor.make_scenario_array(scenario);
                var filename = $('#clone-name').val();
                var module = $(clone).data('module');
                var url = $(clone).attr('href');
                var service_path = [module, filename];

                //@todo have not test the new path lines below
                //  will come back after the github clone work
//                var path = $(this).attr('href');
//                var path_with_file = path.substr(1) + '/' + filename;
//                var service_path = path_with_file.split('/');
//
//                var module = $(clone).data('module');
//                var url = $(clone).attr('href');
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