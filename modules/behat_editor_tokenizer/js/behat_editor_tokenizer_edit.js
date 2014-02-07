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

    Drupal.behat_editor_tokenizer.setMessages = function (results, context) {
        if (results.message === 'File is missing please create one') {
            Drupal.behat_editor_tokenizer.show_add_group_buttons(context);
            results.message = "You an add a token file";
        } else {
            Drupal.behat_editor_tokenizer.show_edit_group_buttons(context);
            results.message = "You can update your token files above";
        }
    };

    Drupal.behat_editor_tokenizer.convertTableToArray = function () {
        //thanks! http://jsfiddle.net/uNvmT/1/
        var array = [];
        $('table.tokens tr').has('td').each(function() {
            var arrayItem = {};
            $('td', $(this)).each(function(index, item) {
                arrayItem[index] = $(item).text();
            });
            array.push(arrayItem);
        });
        return array;
    }

    Drupal.behat_editor_tokenizer.setText = function (token_text, context) {
        $(token_text).each(function(){
            var content = this.content;
            Drupal.behat_editor_tokenizer.appendToTable(content, context);
            Drupal.behat_editor_tokenizer.setMessages(content, context);
        });
    };

    Drupal.behat_editor_tokenizer.appendToTable = function (results, context) {
        $('.token-table', context).append(results);
        Drupal.behat_editor_tokenizer.add_row(context);
        var selectable = Drupal.behat_editor_tokenizer.selectable();
        $('a.selectable', context).editable({
            value: 0,
            source: selectable
        });
    };

    Drupal.behat_editor_tokenizer.show_edit_group_buttons = function (context) {
        $('.add-token', context).hide();
        $('#edit-token', context).show();
        $('form.form-item-use-token', context).show();
    };

    Drupal.behat_editor_tokenizer.show_add_group_buttons = function (context) {
        $('.add-token', context).show();
        $('#edit-token', context).hide();
        $('form.form-item-use-token', context).hide();
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
            url = '/behat_tokenizer_v1/tokenizer/retrieve';
            type = 'GET';
            results = Drupal.behat_editor.action('GET', token, parameters, url, false);
            token_text = ($(results).length) ? results : [];
            filename = results.filename;

            $('input[name=token_filename]').val(filename);

            Drupal.behat_editor_tokenizer.setText(token_text, context);



            Drupal.behat_editor_tokenizer.message(results);

            $('.add-token', context).on('click', function () {
                results = null;
                new_filename = 'newfile';
                token_content = Drupal.behat_editor_tokenizer.convertTableToArray();
                parameters = {
                    "filename": filename_full,
                    "fullpath": filepath,
                    "token_content": token_content,
                    "new_filename": new_filename
                };
                url = '/behat_tokenizer_v1/tokenizer/create';
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
                token_content = Drupal.behat_editor_tokenizer.convertTableToArray();
                token_filename = $('input[name=token_filename]').val();
                parameters = {
                    "filename": filename_full,
                    "fullpath": filepath,
                    "token_content": token_content,
                    "token_filename": token_filename
                };
                url = '/behat_tokenizer_v1/tokenizer/update';
                results = Drupal.behat_editor.action('PUT', token, parameters, url, true);
                Drupal.behat_editor_tokenizer.message(results);
            });

            $('a.editable', context).editable();
        }
    }

})(jQuery);