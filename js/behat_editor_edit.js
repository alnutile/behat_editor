(function ($) {
    Drupal.settings.tags_allowed = {};
    Drupal.behaviors.behat_editor_edit = {

        attach: function (context) {

            $('ul.tagit').each(function(){
                var inputId = $(this).data('scenario-id');
                $(this).tagit(
                    {
                        singleField: true,
                        singleFieldNode: $('#scenario-values-'+inputId+''),
                        placeholderText: '@scenario_tag',
                        availableTags: Drupal.settings.tags_allowed,
                        showAutocompleteOnFocus: true,
                        autocomplete: {delay: 0, minLength: 1},
                        beforeTagAdded:  function(event, ui) {
                            var tag = ui.tagLabel;
                            if($(Drupal.settings.tags_allowed).length) {
                                var allowed = Drupal.settings.tags_allowed;
                                if(jQuery.inArray( tag, allowed, 0) == -1) {
                                    if (!ui.duringInitialization) {
                                        $("#dialog").empty().html("Tag " + tag + " not allowed. <br> It will not be saved. ").dialog();
                                        event.preventDefault();
                                    }
                                }
                            }
                        }
                    }
                );
            });

            $('a.run').click(function(e){
                e.preventDefault();
                var method = 'create-mode';
                var scenario = $('ul.scenario:eq(0) > li').not('.ignore');
                var scenario_array = Drupal.behat_editor.make_scenario_array(scenario);
                var url = $(this).attr('href');
                var parameters = {
                    "method": method,
                    "scenario[]": scenario_array
                };
                $.post(url, parameters, function(data){
                    Drupal.behat_editor.renderMessage(data);
                }, "json");
            });
        }
    };

})(jQuery);