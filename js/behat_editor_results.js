(function ($) {

    Drupal.behat_editor.output_results = {};
    Drupal.behat_editor.get_results = {};
    Drupal.behat_editor.results_modal = {};

    Drupal.behat_editor.results_modal = function(context) {
        $('a.result', context).on('click', function(e){
            e.preventDefault();
            var rid = $(this).attr('id');
            var body = $('body').data('behat_results')[rid];
            body = body.replace(/,/g, "<br />");
            $('#modalResults div.test').html(body);
            $('#modalResults').modal();
        });
    };

    Drupal.behat_editor.output_results = function(results, type) {
        if(type == 'all') {
            var table = new Array();
            var rows = results['data'];
            var status,
                view,
                test_results,
                print_link;
            var behat_results = new Array();

            $.each(rows, function(key, value){
                var date = new Date(value["created"]*1000);
                if(value['status'] == 0 ) {
                    status = '<i class="glyphicon glyphicon-thumbs-up"></i>';
                } else {
                    status = '<i class="glyphicon glyphicon-thumbs-down"></i>';
                }
                print_link = '<a href="/admin/behat/report/' + value['rid'] + '" target="_blank"><i class="glyphicon glyphicon-print"></a>';
                test_results = value['results'];
                view = "<a href='#' class='results' id='"+value['rid']+"'><i class='glyphicon glyphicon-eye-open'></i></a>";
                table[key] = [status, value["duration"], date.format('Y-m-d H:i:s'), view, print_link]
                //Setup the data for later reference
                behat_results[value['rid']] = test_results;
            });

            $('body').data('behat_results', behat_results);

            $('#past-results-table').dataTable(
                {
                    "aaData": table,
                    "aoColumns": [
                        {"sTitle": "Results"},
                        {"sTitle": "Duration"},
                        {"sTitle": "Date"},
                        {"sTitle": "View"},
                        {"sTitle": "Print"}

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
            var rid = data['rid'];
            test_results = data['results'];
            print_link = '<a href="/admin/behat/report/' + data['rid'] +'" target="_blank"><i class="glyphicon glyphicon-print"></a>';
            view = "<a href='#' class='results' id='"+data['rid']+"'><i class='glyphicon glyphicon-eye-open'></i></a>";
            table = [status, data["duration"], date.format('Y-m-d H:i:s'), view, print_link]
            $('#past-results-table').dataTable().fnAddData(table);
            $('body').data('behat_results')[rid] = test_results;
        }
    };

    Drupal.behat_editor.get_results = function(context, callbacks) {
        //@todo something is trigger this on load
        // this type of prevents this while I fix it.

            var token = Drupal.behat_editor.get_token();
            var filename = Drupal.behat_editor.split_filename($('input[name=filename]').val());
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