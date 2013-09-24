(function ($) {
    Drupal.behaviors.behat_editor_saucelabs_run = {

        attach: function (context) {

            $('a.sauce').click(function(e){
                e.preventDefault();
                var method = 'view-mode';
                var url = $(this).attr('href');
                $.post(url, function(data){
                    console.log(data);
                    Drupal.behat_editor.renderMessage(data);
                }, "json");
            });
        }
    };

})(jQuery);