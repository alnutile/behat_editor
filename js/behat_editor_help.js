(function ($) {
    Drupal.behaviors.behat_editor_help = {
        attach: function (context) {
            $('.help-get').on('click', function(e){
                e.preventDefault();
                var doc_name = $(this).data('doc-name');
                $.get('/behat/help/' + doc_name).success(function(data){
                    $('.help-content').html(data);
                });
            });
            $(".help-get").pageslide({ direction: "left", modal: true, offset: '100' });
        }
    };

})(jQuery);