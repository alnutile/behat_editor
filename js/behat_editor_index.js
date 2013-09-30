(function ($) {
    Drupal.behaviors.behat_editor_index = {

        attach: function (context) {

            $('table#admin-features').dataTable(
                {
                    "iDisplayLength": 100
                }
            );
        }
    };

})(jQuery);