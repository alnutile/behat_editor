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
        if(data.file) {
            var message = data.file.message;
            var messages = "<div class='alert alert-info'>";
            messages += message;
            messages += "</div>";
            $('#messages').append(messages);
        }

        if(data.test) {
            var message = data.test.message;
            var messages = "<div class='alert alert-info'>";
            messages += message;
            messages += "</div>";
            $('#messages').append(messages);
            Drupal.behat_editor.setResultsIframe(data.test.file);
        }
    };

})(jQuery);