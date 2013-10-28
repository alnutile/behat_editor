(function ($) {

    Drupal.behat_editor.output_results = {};
    Drupal.behat_editor.get_results = {};
    Drupal.behat_editor.results_modal = {};

    Drupal.behat_editor.results_modal = function(context) {
        $('a.results', context).on('click', function(e){
            e.preventDefault();
            var body = $(this).data('results');
            body = body.replace(/,/g, "<br />");
            $('#modalResults div.test').html(body);
            $('#modalResults').modal();
        });
    }

    Drupal.behat_editor.output_results = function(results, type) {
        if(type == 'all') {
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
                table[key] = [status, value["duration"], date.format('Y-m-d H:i:s'), view]
            });

            $('#past-results-table').dataTable(
                {
                    "aaData": table,
                    "aoColumns": [
                        {"sTitle": "Results"},
                        {"sTitle": "Duration"},
                        {"sTitle": "Date"},
                        {"sTitle": "View"}

                    ],
                    "aaSorting": [[ 2, "desc" ]]
                }
            );
        }

        //Output row
        if(type == 'row') {
            var data = results['data'][0];
            var date = new Date(data["created"]*1000);
            if(data['status'] == 0 ) {
                status = '<i class="glyphicon glyphicon-thumbs-up"></i>';
            } else {
                status = '<i class="glyphicon glyphicon-thumbs-down"></i>';
            }
            test_results = data['results'];
            view = "<a href='#' class='results' id='"+data['rid']+"' data-results='"+test_results+"'><i class='glyphicon glyphicon-eye-open'></i></a>";
            table = [status, data["duration"], date.format('Y-m-d H:i:s'), view]
            $('#past-results-table').dataTable().fnAddData(table);
        }
    };

    Drupal.behat_editor.get_results = function(context, callbacks) {
        var token = Drupal.behat_editor.get_token();
        var filename = $('input[name=filename]').val();
        var module = $('input[name=module]').val();
        var url = '/behat_editor_v1/behat_editor_actions/feature/results/' + module + '/' + filename;
        var parameters = {};
        var async = true;
        var globals = false;
        var type = "POST";
        Drupal.behat_editor.actions(type, token, parameters, url, async, globals, callbacks, context);
    };

    Drupal.behaviors.behat_editor_results = {
        attach: function (context) {
            var callbacks = new Array;
            callbacks = ["Drupal.behat_editor.output_results(results, 'all')", "Drupal.behat_editor.results_modal(context)"];
            Drupal.behat_editor.get_results(context, callbacks);
       }


    };

})(jQuery);