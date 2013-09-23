(function ($) {
    Drupal.behaviors.behat_editor_run = {

        attach: function (context) {

            $('a.run').click(function(e){
                e.preventDefault();
                var method = 'view-mode';
                var url = $(this).attr('href');
                var parameters = {
                    "method": method
                };
                $.post(url, parameters, function(data){
                    //this console.log forces reload of iframe cache in Chromes
                    console.log(data);
                    Drupal.behat_editor.renderMessage(data);
                }, "json");
            });
        }
    };

})(jQuery);