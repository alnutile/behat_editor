/**
 * @todo validation disable button when min requirements are not met on form
 */
(function ($) {
    Drupal.theme.prototype.tagItWrapper  = function(id) {
        var wrapper =  "<li class='tag'><input id='scenario-values-" + id + "' class='section-tag' type='hidden'></li>";
            wrapper += "<li class='ignore'><i class='glyphicon glyphicon-move pull-left'></i><ul id='scenario-input-"+id+"'></ul></li>";
        return wrapper;
    };
    Drupal.behat_editor_scenario_builder = {};
    Drupal.behat_editor_scenario_builder.string_replace = function(string) {
        var string = string || '';
        string = string.replace(/"/g,"'");
        string = Drupal.behat_editor.sanitizer(string);
	return string;
    }

    Drupal.behaviors.behat_editor_scenario_builder = {};
    Drupal.behaviors.behat_editor_scenario_builder = {
        attach: function (context) {


            var checkIfCanRun = function() {
                if($('li.feature').text() != 'Feature: Tests for ?'&&
                    $('li.scenario').text() != 'Scenario: Fill in a name below...')
                    {
                        $('#edit-run-test').removeClass('disabled');
                    }
            };

            /**
             * @todo remove v2 takes over
             */
            var createOutput = function(leaf_class, sortable, label, data_value, middle_words, data_value2, label_text, ending_words) {
                var data_field = '';
                var destination_wrapper = '';
                    if(leaf_class == 'name') {
                        var id = new Date().getTime();
                        destination_wrapper += Drupal.theme('tagItWrapper', id);
                        data_field = 'data-scenario-tag-box="' + id + '"';
                    }
                    destination_wrapper += '<li class="' +leaf_class+ '" ' + data_field + '>';      //Apply elements to the Steps area.
                    destination_wrapper += sortable + '</i>'                    //
                    destination_wrapper += label;                               //eg Scenario:
                    destination_wrapper += data_value;
                    (middle_words.length) ? destination_wrapper += ' ' + middle_words : '';
                    (data_value2.length) ? destination_wrapper += ' ' + data_value2 : '';
                    (ending_words.length) ? destination_wrapper += ' ' + ending_words : '';
                    destination_wrapper += ' <i class="remove glyphicon glyphicon-remove-circle"></i>';
                    destination_wrapper += '</li>';
                return destination_wrapper;
            };

            var createOutputv2 = function(leaf_class, sortable, draggable_step_string) {
                var data_field = '';
                var destination_wrapper = '';
                var spaces = (leaf_class != 'scenario_group' && leaf_class != 'background_group') ? ' spaces-four ' : '';
                if(leaf_class == 'scenario_group') {
                    var id = new Date().getTime();
                    destination_wrapper += Drupal.theme('tagItWrapper', id);
                    data_field = 'data-scenario-tag-box="' + id + '"';
                }
                destination_wrapper += '<li class="' +leaf_class+spaces+ '" ' + data_field + '>';      //Apply elements to the Steps area.
                destination_wrapper += sortable + '</i>';
                destination_wrapper += draggable_step_string;
                destination_wrapper += ' <i class="remove glyphicon glyphicon-remove-circle"></i>';
                destination_wrapper += '</li>';
                return destination_wrapper;
            };


            var wrapperCheck = function(label_text) {
               if (label_text == 'Given I am on' || label_text.search('Then I') != -1 || label_text.search('And I') != -1) {
                    return '"';
               } else {
                   return '';
               }
            };

            var sortableQuestion = function(row) {
                var sortIcon = '<i class="glyphicon glyphicon-move"> ';
                if(row != 'feature') {
                    return sortIcon;
                } else {
                    return ''
                }
            };


            var setFeature = function(destination_class, data_value) {
                if(destination_class == 'feature_group') {
                    $('.feature').empty().append('Feature: ' + data_value);
                }
            };

            /**
             * @todo merge this with above
             *
             * @param destination_class
             * @param data_value
             */
            var setBackground = function(destination_class, data_value) {
                $('li.background_group').html('<i class="glyphicon glyphicon-move"> </i>Background: ' + data_value + '<i class="remove glyphicon glyphicon-remove-circle"></i>');
            };

            /* offer an example */

            $('a.example-test-load', context).click(function(){
                var example = $('ul.example-test').html();
                var message = "You just loaded a test for Wikipedia click Run Test to see it start";
                Drupal.behat_editor.renderMessageCustom(message, 'success', context);
                $('ul.scenario:eq(0)').empty().append(example);
                checkIfCanRun();

                return false;
            });

            $('#edit-save-to', context).change(function(){
                var selectedModule = $('option:selected', this).val();
                var timestamp = $('input[name=timestamp]').val();
                var filename = selectedModule + '_' + timestamp;
                $('input[name=filename]').val(filename);
                $('input[name=title]').val(filename);
            });

            $('i.remove', context).click( function() {
                $(this).closeButton();
            });                                              //then see why it did not work as a behavior?

            $('#features-tagit-input', context).applyTagIts('@feature_tag', 'feature');

            $('ul.sortable').sortable();


            $('button.steps, input.steps', context).click(function(){
                var label = '';
                var label_text = '';
                var destination_class;
                var leaf_class = '';
                var get_value = '';
                var data_value = '';                                            //Get Element type an set as needed
                var data_value2 = '';
                var middle_words = '';
                var ending_words = '';

                //Try 2
                var draggable_step_string = '';
                var destination_wrapper = '';

                //1. Get group from button
                var group = $(this).data('step-group');
                var val = '';

                //  a. setup the target
                destination_class = group;
                leaf_class = group;
                get_value = group;
                $('.'+group+':not(li)').each(function(){
                    /**
                      * @todo need to figure out if middle or end or
                      * Make it so it does not matter and just append
                     */

                    if($(this).data('type') == 'qualifier') {
                        draggable_step_string += $('div label', this).text();
                    } else {
                        //1. Get the Label
                        label_text = $("label[for='"+$(this).attr('id')+"']");
                        //  quick : colon check
                        if(label_text.length)
                        {
                            label += label_text.text();
                            (label_text == 'Background' || label_text == 'Scenario' || label_text == 'Feature') ? label += ':' : false;
                            draggable_step_string += label;
                        }
                        if($(this).data('type') == 'select') {
                            val = $(':selected', this).val();
                            if(val.length > 0) {
                                draggable_step_string += val + ' ';
                            }
                        } else {
                            val = $(this).val();
                            val = Drupal.behat_editor_scenario_builder.string_replace(val);
                            if($(this).data('strip-quotes')) {
                                draggable_step_string += val+' ';
                            } else {
                                draggable_step_string += '"'+val+'" ';
                            }
                            setFeature(destination_class, val);
                        }
                    }

                });


                var sortable = sortableQuestion(destination_class);

                if(draggable_step_string && draggable_step_string.indexOf('Feature:') == -1){
                    if( draggable_step_string.indexOf('Background:') == 0 && $('li.background_group').length) {
                        setBackground(destination_class, val);
                    } else {
                        destination_wrapper = createOutputv2(leaf_class, sortable, draggable_step_string);
                        $('ul.scenario', context).append(destination_wrapper).applyTagIts('@scenario_tag', 'scenario_v2');
                    }
                }

                checkIfCanRun();
            });
       }
    };
   //@todo move this out of live
    $('document').ready(function(){
        $('i.remove').live('click', function(){
                $(this).closeButton();
            });
    });
})(jQuery);
