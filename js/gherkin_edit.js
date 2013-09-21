/**
 * @todo validation disable button when min requirements are not met on form
 */
(function ($) {
    Drupal.theme.prototype.tagItWrapper  = function(id) {
        var wrapper =  "<li class='tag'><input id='scenario-values-" + id + "' class='section-tag' type='hidden'></li>";
            wrapper += "<li class='ignore'><i class='icon-move pull-left'></i><ul id='scenario-input-"+id+"'></ul></li>";
        return wrapper;
    };
    Drupal.behaviors.gherkin_generator_edit = {};
    Drupal.behaviors.gherkin_generator_edit = {
        attach: function (context) {


            var checkIfCanRun = function() {
                if($('li.feature').text() != 'Feature: Tests for ?'&&
                    $('li.scenario').text() != 'Scenario: Fill in a name below...')
                    {
                        $('#edit-run-test').removeClass('disabled');
                    }
            };

            var createOutput = function(leaf_class, sortable, label, data_value, middle_words, data_value2, label_text) {
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
                    destination_wrapper += ' <i class="remove icon-remove-circle"></i>';
                    destination_wrapper += '</li>';
                return destination_wrapper;
            };

            var parseSecondWordSetp = function(value, label_text, self) {
                var data_value2 = '';
                var middle_words = '';
                var get_value2 = value;
                    data_value2 += wrapperCheck(label_text);
                    data_value2 += $('input[name='+get_value2+']').val();
                    data_value2 += wrapperCheck(label_text);
                if($(self).data('middle-words')) {
                    middle_words = $(self).data('middle-words');
                }
                return  {
                            "data_value2": data_value2,
                            "middle_words": middle_words
                };
            };

            var wrapperCheck = function(label_text) {
               if (label_text == 'Given I am on' || label_text.search('Then I') != -1 || label_text.search('And I') != -1) {
                    return '"';
               } else {
                   return '';
               }
            };

            var sortableQuestion = function(row) {
                var sortIcon = '<i class="icon-move"> ';
                if(row != 'feature') {
                    return sortIcon;
                } else {
                    return ''
                }
            };


            var setFeature = function(destination_class, data_value) {
                if(destination_class == 'url') {
                    $('.feature').empty().append('Feature: Tests for ' + data_value);
                }
            };

            var renderMessage = function(message, error_type) {
                var messages = "<div class='alert alert-" + error_type + "'>";  //@todo pull out error = FALSE/TRUE
                messages += message;                                            //@todo pull out error type eg error, info, success etc
                messages += "</div>";
                $('#messages', context).append(messages);
            };


            /* offer an example */
            $('a.example-test-load', context).click(function(){
                var example = $('ul.example-test').html();
                var message = "You just loaded a test for Wikipedia click Run Test to see it start";
                renderMessage(message, 'success');
                $('ul.scenario:eq(0)').empty().append(example);
                checkIfCanRun();
                checkIfCanSave();
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


            $('button.steps', context).click(function(){
                var label = '';
                var label_text = '';

                if($(this).data('test-message')) {
                    label_text = $(this).data('test-message');
                    label += label_text;
                    (label_text == 'Scenario') ? label += ':' : false;
                    label += ' '; //ending space
                }

                var destination_class = $(this).data('parent-input');           //Set the target class

                if($(this).data('target')) {                                    //If different target
                    destination_class = $(this).data('target');                 //than existing targets
                };

                var leaf_class = $(this).data('parent-input');                  //Set more uses of this term
                var get_value = $(this).data('parent-input');

                var data_value = '';                                            //Get Element type an set as needed
                if($(this).data('element-type')) {                              // eg select
                    if($(this).data('element-type') == 'select') {              // default is input
                        label = $(this).data('test-message') + ' ';
                        label += $('select[name='+get_value+'] :selected').val();
                        data_value = '';
                    }
                } else {
                    var val = $('input[name='+get_value+']').val();
                    setFeature(destination_class, val);
                    data_value += wrapperCheck(label_text);
                    data_value += val;
                    data_value += wrapperCheck(label_text);
                }

                var data_value2 = '';                                           //Setup possible 2nd input and label
                var middle_words = '';

                if($(this).data('value-2')) {
                    var results = parseSecondWordSetp($(this).data('value-2'), label_text, this);
                    data_value2 = results['data_value2'];
                    middle_words = results['middle_words'];
                }

                var sortable = sortableQuestion(destination_class);
                var destination_wrapper = createOutput(leaf_class, sortable, label, data_value, middle_words, data_value2, label_text);

                $('ul.scenario', context).append(destination_wrapper).applyTagIts('@scenario_tag', 'scenario');
                Drupal.attachBehaviors($('ul.scenario'));
                checkIfCanRun();
            });
       }
    };

})(jQuery);