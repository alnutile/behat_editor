/**
 * @file
 *   some needed js for choosing groups
 */

(function ($) {
    Drupal.behaviors.behat_editor_sids = {

        attach: function (context) {
            $('#edit-group', context).on('change', function(){
                if($('option:selected', this).val() != '') {
                    $('#edit-users').attr('disabled', 'disabled');
                    $('label[for=edit-users]').after('<span class="help-block">DISABLED. Change group to --none-- to enable</span>');
                } else {
                    $('#edit-users').removeAttr('disabled');
                    $('label[for=edit-users]').next('span.help-block').remove();
                }
            });
        }
    }

})(jQuery);