(function ($) {

    Drupal.behat_editor_tokenizer = Drupal.behat_editor_tokenizer || {};

    Drupal.behat_editor_tokenizer.clone = function (context, filename_id) {
        $('button.clone-row[data-target=' + filename_id + ']').on('click', function (e) {
            e.preventDefault();
            var targetTable = $('div.table-wrapper-' + filename_id + ' tr');
            var targetFilename = $(this).data('filename');
            var results = {};
            var testname,
                testname_starts,
                timeStampe,
                token_filename,
                token_filename_id;
            testname = $('input[name=filename_full]').val();
            testname_starts = testname.split('.');
            testname = testname_starts[0];
            var timeStamp = new Date().getTime();
            token_filename = testname + '.' + timeStamp + '.token';
            token_filename_id = testname + '_' + timeStamp + '_token';

//            targetTable = targetTable.replace(new RegExp(filename_id, "g"), token_filename_id);
//            targetTable = targetTable.replace(new RegExp(targetFilename, "g"), token_filename);
//
//            var content = '<div class="table-wrapper-' + token_filename_id + '">' +
//                targetTable +
//                '</div><hr>';
//            var targetAppend = 'div.token-table';

            Drupal.behat_editor_tokenizer.add_new_token_table(context, token_filename, token_filename_id);
            //@TODO had issues with the selectable inline feature so I did it this way
            $('div.table-wrapper-' + token_filename_id + ' tr:eq(2)').remove();
            $('div.table-wrapper-' + filename_id + ' tr').each(function (i, v) {
                if (i > 1) {
                    //skip header and default url
                    $(v).clone().appendTo('table#' + token_filename_id);
                    //$('table#' + token_filename_id).append(v);
                }
            });
            Drupal.behat_editor_tokenizer.append_actions(context, token_filename_id);

            Drupal.behat_editor_tokenizer.clone(context, token_filename_id);
            var results = {};
            results.errors = 0;
            results.message = 'Your new clone is ready';
            Drupal.behat_editor_tokenizer.message(results);

        });
    }

    Drupal.behaviors.behat_editor_tokenizer_clone = {
        attach: function (context) {

        }
    };

})(jQuery);