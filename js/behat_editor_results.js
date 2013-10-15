(function ($) {
    Drupal.behaviors.behat_editor_results = {

        attach: function (context) {
            var token = Drupal.behat_editor.get_token();
            var filename = $('input[name=filename]').val();
            var module = $('input[name=module]').val();
            var url = '/behat_editor_v1/behat_editor_actions/feature/results/' + module + '/' + filename;
            var parameters = {};
            var results = Drupal.behat_editor.action('POST', token, parameters, url);
            if(results['error'] === 0) {
                var table = new Array();
                var rows = results['data'];
                var status,
                    view,
                    test_results;
                $.each(rows, function(key, value){
                    var date = new Date(value["created"]*1000);
                    if(value['status'] == 0 ) {
                        status = '<i class="glyphicon glyphicon-thumbs-up"></i>';
                    } else {
                        status = '<i class="glyphicon glyphicon-thumbs-down"></i>';
                    }
                    test_results = value['results'];
                    view = "<a href='#' class='results' id='"+value['rid']+"' data-results='"+test_results+"'><i class='glyphicon glyphicon-eye-open'></i></a>";
                    table[key] = [status, value["duration"], date.format('Y-m-d H:i'), view]
                });

                $('#past-results-table').dataTable(
                    {
                        "aaData": table,
                        "aoColumns": [
                            {"sTitle": "Results"},
                            {"sTitle": "Duration"},
                            {"sTitle": "Date"},
                            {"sTitle": "View"}

                        ]
                    }
                );
            } else {
                Drupal.renderMessageCustom($results['message'], 'warning')
            }

            $('a.results').click(function(e){
                e.preventDefault();
                var body = $(this).data('results');
                body = body.replace(/,/g, "<br />");
                $('#beModal div.test').html(body);
                $('#beModal').modal();
            });
       }


    };

})(jQuery);