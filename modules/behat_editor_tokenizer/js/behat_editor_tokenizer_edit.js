(function ($) {

    Drupal.behat_editor_tokenizer = Drupal.behat_editor_tokenizer || {};

    Drupal.behat_editor_tokenizer.message = function (results) {
        var type = null;
        if (results !== null && results.errors === 1) {
            type = 'danger';
        } else {
            type = 'success';
        };

        if (results !== null && results.message !== null) {
            var message = results.message;
            var alert = '<div class="alert alert-' + type + '">' +
                '<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>' +
                ' ' + message + '</div>';
            $('#tokenizer-messages').append(alert);
        }

    };

    Drupal.behat_editor_tokenizer.show_edit_group_buttons = function() {
        $('.add-token').hide();
        $('#edit-token').show();
        $('#use-token').show();
    };

    Drupal.behat_editor_tokenizer.show_add_group_buttons = function() {
        $('.add-token').show();
        $('#edit-token').hide();
        $('#use-token').hide();
    };

    Drupal.behaviors.behat_editor_tokenizer_edit = {

        //1. Stash the filename of the token file for later

        attach: function (context) {
            //1. Check if a file exists if so load it
            var filepath,
                filename_full,
                token,
                parameters,
                url,
                type,
                results,
                token_text,
                filename,
                token_content,
                new_filename;
            filepath = $('input[name=filepath]').val();
            filename_full = $('input[name=filename_full]').val();
            token = Drupal.behat_editor.get_token();
            parameters = {
                "filename": filename_full,
                "fullpath": filepath,
                "token_content": {}
            };
            url = '/behat_tokenizer_v1/tokenizer/getfile';
            type = 'GET';
            results = Drupal.behat_editor.action('GET', token, parameters, url, false);

            if (results.errors === 1) {
                Drupal.behat_editor_tokenizer.show_add_group_buttons();
            } else {
                token_text = (results.content !== null) ? results.content : null;
                filename = results.filename;
                $('input[name=token_filename]').val(filename);
                results.message = "Ready to edit the file";
                $('#tokenizer_text').val(token_text);
                Drupal.behat_editor_tokenizer.show_edit_group_buttons();
            }

            Drupal.behat_editor_tokenizer.message(results);

            $('.add-token').on('click', function () {
                results = null;
                new_filename = 'newfile';
                token_content = $('#tokenizer_text').val().split("\n");
                console.log(token_content);
                parameters = {
                    "filename": filename_full,
                    "fullpath": filepath,
                    "token_content": token_content,
                    "new_filename": new_filename
                };
                url = '/behat_tokenizer_v1/tokenizer';
                results = Drupal.behat_editor.action('POST', token, parameters, url, true);
                Drupal.behat_editor_tokenizer.message(results);

                if (results.errors !== 1) {
                    Drupal.behat_editor_tokenizer.show_edit_group_buttons();
                }
            });

            $('#edit-token').on('click', function () {
                var token_content,
                    token_filename,
                    parameters,
                    url,
                    results;
                token_content = $('#tokenizer_text').val().split("\n");
                token_filename = $('input[name=token_filename]').val();
                parameters = {
                    "filename": filename_full,
                    "fullpath": filepath,
                    "token_content": token_content,
                    "token_filename": token_filename
                };
                url = '/behat_tokenizer_v1/tokenizer/settoken';
                results = Drupal.behat_editor.action('PUT', token, parameters, url, true);
                Drupal.behat_editor_tokenizer.message(results);
            });


        }
    }

})(jQuery);