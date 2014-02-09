(function ($) {

    Drupal.behat_editor_tokenizer = Drupal.behat_editor_tokenizer || {};


    Drupal.behaviors.behat_editor_tokenizer_update = {

        attach: function (context) {
            $('button.save-row', context).on('click', function (e) {
                e.preventDefault();
                var token_content,
                    token_filename,
                    parameters,
                    url,
                    results,
                    fullpath,
                    token,
                    target;
                token = Drupal.behat_editor.get_token();
                fullpath = $('input[name=filepath]').val() + '/tokens';
                target = $(this).data('target');
                token_filename = $(this).data('filename');
                token_content = Drupal.behat_editor_tokenizer.convertTableToArray(target);
                parameters = {
                    "filename": token_filename,
                    "fullpath": fullpath,
                    "token_content": token_content
                };
                url = '/behat_tokenizer_v1/tokenizer/update';
                results = Drupal.behat_editor.action('PUT', token, parameters, url, true);
                Drupal.behat_editor_tokenizer.message(results);
            });
        }
    };

})(jQuery);