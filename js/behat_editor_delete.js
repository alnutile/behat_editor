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
            });

            $('#beModal').on('hide.bs.modal', function(){
                var url = $('a.delete').attr('href');
                var parameters = {};
                var data = Drupal.behat_editor.action('DELETE', token, parameters, url);
                var filename = $(this).data('filename');
                if(data.error == 0) {
                    window.location.replace("/admin/behat/index");
                }
                Drupal.behat_editor.renderMessage(data);
            });
        }
    };
})(jQuery);