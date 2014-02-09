(function ($) {

    Drupal.behat_editor_tokenizer = Drupal.behat_editor_tokenizer || {};

    Drupal.behaviors.behat_editor_tokenizer_add = {

        attach: function (context) {
            $('.add-token', context).on('click', function (e) {
                e.preventDefault();
                Drupal.behat_editor_tokenizer.add_new_token_table(context);
            });
        }
    }
})(jQuery);