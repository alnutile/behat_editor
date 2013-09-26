(function ($) {
    Drupal.behaviors.behat_editor_saucelabs_run = {

        attach: function (context) {

            $('a.sauce').click(function(e){
                e.preventDefault();
                if(!$(this).hasClass('disabled')) {
                    var method = 'view-mode';
                    var url = $(this).attr('href');
                    $.get('admin/behat/saucelabs/jobs', function(data){
                        //Start looking for new jobs
                        console.log(data);
                    });
//                    $.post(url, function(data){
//                        console.log(data);
//                        Drupal.behat_editor.renderMessage(data);
//                    }, "json");
                }
            });
        }
    };

})(jQuery);