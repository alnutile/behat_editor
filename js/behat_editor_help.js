(function ($) {
    Drupal.behat_editor_help = Drupal.behat_editor_help || {};

    Drupal.behat_editor_help.get = function (doc_name, context) {
        var pathToImage = Drupal.settings.behat_editor.behat_editor_loader_url;
        $('.help-content').html("Loading Docs <img src='/" + pathToImage + "'>");
        $.get('/behat/help/' + doc_name).success(function (data) {
            $('.help-content', context).html(data);
            if (doc_name !== 'index.html') {
                $('.help-content').prepend("<br><a href=\"index.html\">Help Index</a><hr>");
            };
            Drupal.behat_editor_help.click(context);
        });
    }

    Drupal.behat_editor_help.click = function (context) {
        jQuery('.help-content a', context).filter(function(){
            if (this.href.indexOf(location.origin) === 0) {
               $(this).addClass('local');
            } else {
               $(this).attr('target', '_blank');
            }

        });

        jQuery('.help-content a', context).on('click', function (e) {
            if($(this).hasClass('local')){
                e.preventDefault();
                var doc_name = $(this).attr('href');
                Drupal.behat_editor_help.get(doc_name, context);
            }
        });
    }

    Drupal.behaviors.behat_editor_help = {
        attach: function (context) {
            $('.help-get', context).on('click', function(e){
                e.preventDefault();
                var doc_name = $(this).data('doc-name');
                Drupal.behat_editor_help.get(doc_name, context);
            });

            $(".help-get").pageslide({ direction: "left", modal: true, offset: '100' });

        }
    };

})(jQuery);