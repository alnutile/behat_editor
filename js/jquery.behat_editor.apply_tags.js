(function($) {
    $.fn.applyTagIts = function(placeholder, type) {
        var tagArea = this;
        if(type == 'scenario') {
            return this.each(function(){
                if(!$(this).hasClass('example-test')) {
                    var id = $('li.name', this).last().data('scenario-tag-box');
                    $('#scenario-input-'+id+'', this).tagit(
                        {
                            singleField: true,
                            singleFieldNode: $('#scenario-values-'+id+''),
                            placeholderText: placeholder,
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
                }
            });
        };

        if(type == 'scenario_v2') {
            return this.each(function(){
                if(!$(this).hasClass('example-test')) {
                    var id = $('li.scenario_group', this).last().data('scenario-tag-box');
                    $('#scenario-input-'+id+'', this).tagit(
                        {
                            singleField: true,
                            singleFieldNode: $('#scenario-values-'+id+''),
                            placeholderText: placeholder,
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
                }
            });
        };

        if(type == 'feature') {

            var target_id_input = $(tagArea).attr('id');
            if ( target_id_input !== undefined ) {
                var source_id_values = target_id_input.replace('input', 'values')
            } else {
                var source_id_values = '';
            }
            $('#'+target_id_input+'').tagit(
                {
                    singleField: true,
                    singleFieldNode: $('#'+source_id_values+''),
                    placeholderText: placeholder,
                    showAutocompleteOnFocus: true,
                    availableTags: Drupal.settings.tags_allowed,
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
            return this;
        };
    };
})(jQuery);