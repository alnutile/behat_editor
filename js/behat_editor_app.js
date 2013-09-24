(function ($) {
    $( document ).ajaxStart(
        function(){
            $('.running-tests').fadeIn();
        }
    );

    $( document ).ajaxStop(
        function(){
            $('.running-tests').fadeOut();
        }
    );

    Drupal.behat_editor = {};

    Drupal.behat_editor.setResultsIframe = function(url) {
        $('.test-result').empty();
        var iframe = '<iframe src="' + url + '"';
        iframe += " width='500' height='750' frameborder='0'";
        iframe += " scrolling='yes' marginheight='0' marginwidth='0'>";
        iframe += '</iframe>';
        $('.test-result').append(iframe);
    }

    Drupal.behat_editor.renderMessage = function(data) {

        if(data.error == 1) {
            var message = data.message;
            var messages = "<div class='alert alert-error'><a href='#' class='close' data-dismiss='alert'>&times;</a>";
            messages += message;
            messages += "</div>";
            $('#messages').append(messages);
        } else {
            if(data.file) {
                var message = data.file.message;
                var messages = "<div class='alert alert-info'><a href='#' class='close' data-dismiss='alert'>&times;</a>";
                messages += message;
                messages += "</div>";
                $('#messages').append(messages);
            }

            if(data.test) {
                var message = data.test.message;
                var messages = "<div class='alert alert-info'><a href='#' class='close' data-dismiss='alert'>&times;</a>";
                messages += message;
                messages += "</div>";
                $('#messages').append(messages);
                Drupal.behat_editor.setResultsIframe(data.test.file);
            }
        }
    };

    Drupal.behat_editor.renderMessageCustom = function(message, error_type, context) {
        var messages = "<div class='alert alert-" + error_type + "'><a href='#' class='close' data-dismiss='alert'>&times;</a>";  //@todo pull out error = FALSE/TRUE
        messages += message;                                            //@todo pull out error type eg error, info, success etc
        messages += "</div>";
        $('#messages', context).append(messages);
    };


    Drupal.behat_editor.make_scenario_array = function(scenario) {
        var items = scenario.length;
        var scenario_array = new Array()
        for(var i = 0; i < items; i++) {
            if($(scenario[i]).hasClass('tag')) {
                var tags = $('input', scenario[i]).val();
                scenario_array[i] = tags;
            } else {
                scenario_array[i] = $(scenario[i]).text();
            }
        }

        return scenario_array;
    }

})(jQuery);