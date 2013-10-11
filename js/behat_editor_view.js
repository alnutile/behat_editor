(function ($) {
    Drupal.behaviors.behat_editor_run = {

        attach: function (context) {

            $('a.run').click(function(e){
                var token = Drupal.behat_editor.get_token();
                console.log(token);
                e.preventDefault();
                var method = 'view-mode';
                var url = $(this).attr('href');
                var parameters = {
                    "method": method
                };
                var data = Drupal.behat_editor.action('POST', token, parameters, url);
                Drupal.behat_editor.renderMessage(data);
            });
        }
    };

})(jQuery);