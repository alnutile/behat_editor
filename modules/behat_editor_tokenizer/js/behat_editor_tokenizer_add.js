(function ($) {

    Drupal.behat_editor_tokenizer = Drupal.behat_editor_tokenizer || {};

    Drupal.behaviors.behat_editor_tokenizer_add = {

        attach: function (context) {
            $('.add-token').on('click', function(){
                console.log("Make a token");
            });
        }
    }

})(jQuery);