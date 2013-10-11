(function ($) {
    Drupal.behaviors.behat_editor_run = {

        attach: function (context) {

            $('a.run').click(function(e){
                var token = Drupal.behat_editor.get_token();
                e.preventDefault();
                var url = $(this).attr('href');
                var parameters = {};
                var data = Drupal.behat_editor.action('POST', token, parameters, url);
                Drupal.behat_editor.renderMessage(data);
            });
        }
    };

})(jQuery);