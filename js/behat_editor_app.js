(function ($) {

    $(document).bind("ajaxSend", function(){
        Drupal.behat_editor.buttons('disable');
        if(Drupal.behat_editor.ifNoty) {
            var message = $('.running-tests').text();
            var type = 'information';
            var n = noty({text: message, type: type, dismissQueue: false, timeout: 5000, maxVisible: 1});
        } else {
            $('.running-tests').fadeIn();
        }
    }).bind("ajaxComplete", function(){
        Drupal.behat_editor.buttons('enable');
            if(Drupal.behat_editor.ifNoty) {
                //$.noty.clearQueue();
            } else {
                $('.running-tests').fadeOut();
            }
    });

    Drupal.behat_editor = {};

    Drupal.behat_editor.ifNoty = function() {
        if(Drupal.settings.simple_noty && Drupal.settings.simple_noty.active == 1) {
            return true;
        } else {
            return false;
        }
    }

    Drupal.behat_editor.split_filename = function(filename) {
        var filename_array = filename.split('.');
        return filename_array[0];
    };

    Drupal.behat_editor.get_token = function() {
        var token = 'null';
        $.ajax(
            {
                url:'/services/session/token',
                async: false,
                global: false
            }
        ).done(function(data){
                token = data;
        });
        return token;
    }

    Drupal.behat_editor.action = function(type, token, parameters, url) {
        var results = '';
        $.ajax({
                type: type,
                beforeSend: function (request) {
                    request.setRequestHeader("X-CSRF-Token", token);
                },
                url: url,
                data: JSON.stringify(parameters),
                dataType: "json",
                async: false,
                contentType: 'application/json'
            }
        ).done(function(data){
                results = data;
            });
        return results;
    };

    Drupal.behat_editor.actions = function(type, token, parameters, url, async, globals, callbacks, context){
        $.ajax({
                type: type,
                beforeSend: function (request) {
                    request.setRequestHeader("X-CSRF-Token", token);
                },
                url: url,
                data: JSON.stringify(parameters),
                dataType: "json",
                async: async,
                global: globals,
                contentType: 'application/json'
            }
        ).done(function(data){
                results = data;
                $.each(callbacks, function(index, value){
                    //using eval for content only system is passing
                    eval(value);
                });
        });
    };

    Drupal.behat_editor.setResultsIframe = function(url) {
        $('.test-result').empty();
        var iframe = '<iframe src="' + url + '"';
        iframe += " width='500' height='750' frameborder='0'";
        iframe += " scrolling='yes' marginheight='0' marginwidth='0'>";
        iframe += '</iframe>';
        $('.test-result').append(iframe);
    }

    Drupal.behat_editor.setResultsBox = function(text) {
        $('.test-result').empty();
        $('.test-result').html(text);
    }

    Drupal.behat_editor.renderMessage = function(data) {

        if(data.error == 1) {
            var message = data.message;
            if(Drupal.behat_editor.ifNoty){
                var n = noty({text: message, type: 'success', dismissQueue: false});
            } else {
                var messages = "<div class='alert alert-error'><a href='#' class='close' data-dismiss='alert'>&times;</a>";
                messages += message;
                messages += "</div>";
                $('#messages-behat').append(messages);
            }
        } else {
            if(data.file) {
                var message = data.file.message;
                if(Drupal.behat_editor.ifNoty){
                    var n = noty({text: message, type: 'success', dismissQueue: false});
                } else {
                    var messages = "<div class='alert alert-info'><a href='#' class='close' data-dismiss='alert'>&times;</a>";
                    messages += message;
                    messages += "</div>";
                    $('#messages-behat').append(messages);
                }
            }

            if(data.test) {
                var message = data.test.message;
                if(Drupal.behat_editor.ifNoty){
                    var n = noty({text: message, type: 'success', dismissQueue: false});
                } else {
                    var messages = "<div class='alert alert-info'><a href='#' class='close' data-dismiss='alert'>&times;</a>";
                    messages += message;
                    messages += "</div>";
                    $('#messages-behat').append(messages);
                }
                Drupal.behat_editor.setResultsBox(data.test.test_output);
            }
        }
    };


    Drupal.behat_editor.buttons = function(state) {
        $('div.actions a').toggleClass('disabled');
    }

    Drupal.behat_editor.renderMessageCustom = function(message, error_type, context) {
        if(Drupal.behat_editor.ifNoty){
            var n = noty({text: message, type: error_type, dismissQueue: false});
        } else {
            var messages = "<div class='alert alert-" + error_type + "'><a href='#' class='close' data-dismiss='alert'>&times;</a>";  //@todo pull out error = FALSE/TRUE
            messages += message;                                            //@todo pull out error type eg error, info, success etc
            messages += "</div>";
            $('#messages-behat', context).append(messages);
        }
    };

    Drupal.behat_editor.renderNotyCustom = function(message, type, dismiss_queue) {
        var n = noty({
            text: message,
            type: type,
            dismissQueue: false,
            maxVisible: 3,
            timeout: 2000,
            buttons: [
                {
                    addClass: 'btn btn-danger', text: 'close', onClick: function($noty) {
                        $noty.close();
                    }
                }
            ]
        });
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

    Drupal.behaviors.behat_editor_action_buttons = {

        attach: function (context) {
            $('form .actions a').on('click', function(){
                if(!$('div#edit-container1 a.accordion-toggle').hasClass('collapsed')){
                    $('a.accordion-toggle').click();
                }
            });
        }
    }

})(jQuery);