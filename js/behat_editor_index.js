(function ($) {
    var filename;
    var url;
    var row;

    Drupal.behaviors.behat_editor_index = {

        attach: function (context) {
            var token = Drupal.behat_editor.get_token();

            $('a.result', context).on('click', function(e){
                e.preventDefault();
                var rid = $(this).data('rid');
                var body = Drupal.settings.behat_editor.results[rid].results;
                $('#modalResults div.test').html(body);
                $('#modalResults').modal();
            });

            $('a.index-delete-test').click(function(e){
                filename = $(this).data('filename');
                url = $(this).attr('href');
                row = $(this).closest('tr');
                $('#beModal span.filename').text(filename);
                $('#beModal').modal();
                e.preventDefault();
            });

            $('button.confirm-delete').click(function(e){
                $('#beModal').modal('hide');
                var parameters = {};
                var data = Drupal.behat_editor.action('DELETE', token, parameters, url);
                if(data.error == 0) {
                    Drupal.behat_editor.renderMessageCustom("File " + filename + " deleted and row removed", 'success', context);
                    $(row).fadeOut('slow').remove();
                } else {
                    Drupal.behat_editor.renderMessageCustom("File " + filename + " could now be deleted", 'success', context);
                }

            });

            $('table#admin-features').dataTable(
                {
                    "iDisplayLength": 200,
                    "aaSorting": [[ 1, "asc" ]]
                }
            );
        }
    };

})(jQuery);