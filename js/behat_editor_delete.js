(function ($) {
    Drupal.behaviors.behat_editor_delete = {
        attach: function (context) {
            var token = Drupal.behat_editor.get_token();
            var data = {};
            $('a.delete').click(function(e){
                e.preventDefault();
                var url = $(this).attr('href');
                var parameters = {};
                var data = Drupal.behat_editor.action('DELETE', token, parameters, url);
                console.log(data);
                var filename = $(this).data('filename');
                if(data.error == 0) {
                    alert("The file will be deleted " + filename);
                    window.location.replace("/admin/behat/index");
                }
                Drupal.behat_editor.renderMessage(data);
            });
        }
    };
})(jQuery);