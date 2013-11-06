(function ($) {
    Drupal.behaviors.behat_editor_clone = {

        attach: function (context) {
            var token = Drupal.behat_editor.get_token();

            $('a.clone').click(function(e){
                var filename = $('input[name=filename]').val();
                $('#beModalClone .filename').text('clone_' + filename + '.feature');
                $('#beModalClone').modal();
                e.preventDefault();
            });

            $('button.confirm-clone').click(function(e){
                $('#beModalClone').modal('hide');
                if( $('input.new-clone-name').val() ){
                    var filename = $('input.new-clone-name').val();
                    var filename_with_out_suffix = Drupal.behat_editor.split_filename(filename);
                } else {
                    var filename_with_out_suffix = $('input[name=filename]').val();
                    filename_with_out_suffix = 'clone_' + filename;
                    var filename = filename_with_out_suffix + '.feature';
                }
                var clone = $('a.clone');
                var scenario = $('ul.scenario:eq(0) > li').not('.ignore');
                var scenario_array = Drupal.behat_editor.make_scenario_array(scenario);
                var module = $(clone).data('module');
                var url = $(clone).attr('href');
                var parameters = {
                    "scenario": scenario_array,
                    "filename": filename_with_out_suffix,
                    "module": module
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