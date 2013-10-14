(function ($) {
    Drupal.behaviors.behat_editor_add = {

        attach: function (context) {
            var token = Drupal.behat_editor.get_token();

            $('a.add').click(function(e){
                $('#beModal').modal();
                e.preventDefault();
            });

            $('button.confirm-add').click(function(e){
                $('#beModal').modal('hide');
            });

            $('#beModal').on('hide.bs.modal', function(){
                var add = $('a.add');
                var scenario = $('ul.scenario:eq(0) > li').not('.ignore');
                var scenario_array = Drupal.behat_editor.make_scenario_array(scenario);
                var filename = $(add).data('filename');
                var module = $(add).data('module');
                var url = $(add).attr('href');
                var parameters = {
                    "scenario": scenario_array,
                    "filename": filename,
                    "module": module
                };
                var data = Drupal.behat_editor.action('POST', token, parameters, url);
                if(data.error == 0) {
                    window.location.replace("/admin/behat/edit/" + module + "/" + filename + ".feature");
                }
                Drupal.behat_editor.renderMessage(data);
            });
        }


    };

})(jQuery);