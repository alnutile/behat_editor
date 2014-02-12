(function ($) {

    Drupal.behat_editor_tokenizer = {};
    Drupal.behat_editor_tokenizer.selectable = function () {
        var options = [];
        $('#edit-group option').each(function () {
            var opt = $(this).text();
            options.push({ value: opt, text: opt });
        });
        return options;
    }

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

    Drupal.behat_editor_tokenizer.convertTableToArray = function (target) {
        //thanks! http://jsfiddle.net/uNvmT/1/
        var array = [];
        $('table#' + target + ' tr').has('td').each(function () {
            var arrayItem = {};
            $('td', $(this)).each(function (index, item) {
                arrayItem[index] = $(item).text();
            });
            array.push(arrayItem);
        });
        return array;
    }

    Drupal.behat_editor_tokenizer.setText = function (filename_id, context, content) {
            Drupal.behat_editor_tokenizer.appendToTable(content, filename_id, context);
    };

    Drupal.behat_editor_tokenizer.appendToTable = function (results, filename_id, context) {
        $('.token-table', context).append(results);
        $('table#' + filename_id + ' a.editable', context).editable();
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

    Drupal.behat_editor_tokenizer.add_row = function (context, filename_id) {

        $('button.new-row[data-target=' + filename_id + ']').on('click', function (e) {
            var targetTable = $('table#' + filename_id);
            e.preventDefault();
            var id = new Date().getTime();
            var row =
                '<tr>' +
                  '<td>' +
                    '<a class="editable" href="#" id="' + id + '3" data-type="text" ">Key</a>' +
                  '</td>' +
                  '<td>' +
                    '<a class="editable" href="#" id="' + id + '4" data-type="textarea" ">Token</a>' +
                   '</td>' +
                '</tr>';

            $(targetTable, context).append(row);
            $('a.editable', context).editable();
            results.errors = 0;
            results.message = 'Your new row has been added';
            Drupal.behat_editor_tokenizer.message(results);
        });
    };

    Drupal.behat_editor_tokenizer.append_actions = function ( context, token_filename_id ) {
        Drupal.behat_editor_tokenizer.add_row(context, token_filename_id);
        Drupal.behat_editor_tokenizer.create_row(context, token_filename_id);
        Drupal.behat_editor_tokenizer.set_editable(context, token_filename_id);
        Drupal.behat_editor_tokenizer.set_selectable(context, token_filename_id);
        Drupal.behat_editor_tokenizer.sesssion_set(token_filename_id, context);
        Drupal.behat_editor_tokenizer.clone(context, token_filename_id);
    };

    Drupal.behat_editor_tokenizer.add_new_token_table = function (context, token_filename, token_filename_id) {
        var testname_starts,
            token,
            testname;
        token = Drupal.behat_editor.get_token();
        testname = $('input[name=filename_full]').val();
        testname_starts = testname.split('.');
        testname = testname_starts[0];
        var timeStamp = new Date().getTime();
        if ( token_filename_id === undefined && token_filename === undefined) {
            var token_filename = testname + '.' + timeStamp + '.token';
            var token_filename_id = testname + '_' + timeStamp + '_token';
        }
        //Request a template from the url
        token = Drupal.behat_editor.get_token();
        parameters = {
            "token_filename": token_filename,
            "token_filename_id": token_filename_id
        };
        url = '/behat_tokenizer_v1/templates';
        type = 'GET';
        results = Drupal.behat_editor.action('GET', token, parameters, url, false);

        // figure out if this is the first token table or not
        if ( $('.token-table div.table-wrapper').length === 0 ) {
            var targetAppend = 'div.token-table';
            $(targetAppend).append(results);
        } else {
            var targetAppend = '.token-table div.table-wrapper:last';
            $(results).insertAfter(targetAppend);
        }
    }

    Drupal.behat_editor_tokenizer.set_editable = function ( context, token_filename_id ) {
        $('table#' + token_filename_id + ' a.editable', context).editable();
    };

    Drupal.behat_editor_tokenizer.set_selectable = function ( context, token_filename_id ) {
        var selectable = Drupal.behat_editor_tokenizer.selectable();
        $('table#' + token_filename_id + ' a.selectable', context).editable({
            prepend: "not selected",
            source:  selectable
        });
    };

    Drupal.behat_editor_tokenizer.create_row = function ( context, filename_id ) {
        $('button.create-row[data-target=' + filename_id + ']', context).on('click', function (e) {
            //dealing with trouble unloading this
            if ( $('button.create-row[data-target=' + filename_id + ']').length > 0 ) {
                e.preventDefault();
                var targetTable = $('table#' + filename_id);
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
                url = '/behat_tokenizer_v1/tokenizer/create';
                $('button.create-row[data-target=' + filename_id + ']', context).addClass('save-row').removeClass('create-row');
                Drupal.behaviors.behat_editor_tokenizer_update.attach(context);
                results = Drupal.behat_editor.action('POST', token, parameters, url, true);
                Drupal.behat_editor_tokenizer.message(results);
            }
        });
    }

    Drupal.behat_editor_tokenizer.add_selectable = function (context) {
        var selectable = Drupal.behat_editor_tokenizer.selectable();

        $('a.selectable', context).editable({
            prepend: "not selected",
            source:  selectable
        });
    }

    Drupal.behat_editor_tokenizer.sesssion_set = function (filename_id, context) {
        $('button.session-set[data-target=' + filename_id + '').on('click', function (e){
            e.preventDefault();
            var token_filename = $(this).data('filename');
            var test_path = $('input[name=filepath]').val();
            if ( !$(this).hasClass('active-session') ) {
                $('button.session-set').text('use for next test?');
                $('button.session-set').removeClass('btn-success').addClass('btn-warning');
                var set = 'true';
                $(this).text("active for next test");
                $(this).addClass('btn-success').removeClass('btn-warning');
            } else {
                $(this).text("use for next test?");
                $(this).removeClass('btn-success').addClass('btn-warning');
                var set = 'false';
            }
            var parameters = {
                "token_filename": token_filename,
                "set":  set,
                "test_path": test_path
            }
            var token = Drupal.behat_editor.get_token();
            url = '/behat_tokenizer_v1/session';
            $(this).toggleClass('active-session');
            results = Drupal.behat_editor.action('GET', token, parameters, url, false);
            Drupal.behat_editor_tokenizer.message(results);
        });
    }

    Drupal.behaviors.behat_editor_tokenizer_app = {

        attach: function (context) {
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
            if ( results === null ) {
                Drupal.behat_editor_tokenizer.add_new_token_table(context);
                Drupal.behat_editor_tokenizer.append_actions(context, token_filename_id);
                var results = {};
                results.errors = 0;
                results.message = 'Your new token set table has been added above';
                Drupal.behat_editor_tokenizer.message(results);
            } else {
                $(results).each(function(){
                    var content = this.content;
                    var errors = this.errors;
                    var message = this.message;
                    var filename = this.filename;
                    var filename_id = filename.replace(/\./g, '_');
                    Drupal.behat_editor_tokenizer.setText(filename_id, context, content);
                    Drupal.behat_editor_tokenizer.append_actions(context, filename_id);
                });
            }

            $('select#edit-group', context).on('change', function(){
                var selected = $('select#edit-group option:selected').text();
                $('body a.selectable').each(function() {
                    if ( $(this).text() === selected ) {
                        var targetButton = $(this).data('target');
                        $('button.session-set[data-target=' + targetButton + ']').click();
                    }
                });
            });
        }
    }

})(jQuery);