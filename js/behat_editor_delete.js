(function ($) {
    Drupal.behaviors.behat_editor_delete = {
        attach: function (context) {
            var token = Drupal.behat_editor.get_token();
            var data = {};
            $('a.delete').click(function(e){
                $('#beModal').modal();
                e.preventDefault();
            });

            $('button.confirm-delete').click(function(e){
                $('#beModal').modal('hide');
                var url = $('a.delete').attr('href');
                var url_args = window.location.pathname;
                var url_args_array = url_args.split('/');
                var module = url_args_array[4];
                var service_path = url_args_array.slice(4, url_args_array.length);
                var filename = url_args_array[url_args_array.length - 1];
                var parameters = {
                    "settings":
                    {
                        "module": module,
                        "filename": filename,
                        "path": service_path
                    }
                };
                var data = Drupal.behat_editor.action('DELETE', token, parameters, url);
                if(data.error == 0) {
                    window.location.replace("/admin/behat/index");
                }
                Drupal.behat_editor.renderMessage(data);
            });
        }
    };
})(jQuery);