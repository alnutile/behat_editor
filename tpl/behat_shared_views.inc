<?php
use Drupal\BehatEditor;

/**
 * Create the table on the Admin page
 *
 * @files returned from _behat_editor_build_array_of_available_files
 * return table
 */
function _behat_editor_produce_table_array($files_array) {
    $rows = array();
    $all_results = array();
    foreach($files_array as $key => $value) {
        foreach($value as $row) {
            $file_name = $row['filename'];
            $file_name_no_ext = $row['filename_no_ext'];
            $module_name = $key;
            $latest = _behat_editor_get_latest_result($file_name, $module_name);
            $all_results[$latest['rid']] = $latest['original'];
            $tags = implode(', ', $row['tags_array']);
            $rows[] = array(
                'data' =>
                    array
                        (
                            $latest['icon'],
                            _show_file_link($file_name, $module_name),
                            _show_module_name($file_name, $module_name),
                            $tags,
                            _show_link($file_name, $module_name, $link_text = 'view'),
                            _show_link($file_name, $module_name, $link_text = 'edit'),
                            _show_link($file_name_no_ext, $module_name, $link_text = 'delete'),
                        ),
                'class' => array($latest['pass_fail']))
            ;
        }
    };

    $header = array(
        'Status',
        'Name',
        'Module',
        'Tags',
        'View',
        'Edit',
        'Delete',
    );

    $behat_table = theme('table', array(
        'header' => $header,
        'rows' => $rows,
        'attributes' => array('class' => array('table-hover'), 'id' => 'admin-features')
    ));

    drupal_add_js(array('behat_editor' => array('results' => $all_results)), 'setting');
    return $behat_table;
}

function _behat_editor_get_latest_result($file_name, $module_name){
    $output = '';
    $pass_fail = 'na';
    $rid = 0;
    $result = '';
    $results = BehatEditor\Results::getLatestResultForFile($module_name, $file_name);
    if(isset($results['results'][0])) {
        $result = $results['results'][0];
        $rid = $results['results'][0]['rid'];
        if($result['status'] == 0) {
            $pass_fail = "pass";
            $status = '<i class="glyphicon glyphicon-thumbs-up"></i>';
        } else {
            $pass_fail = 'fail';
            $status = '<i class="glyphicon glyphicon-thumbs-down"></i>';
        }
        $output .= '<a class="result" data-rid="'.$result['rid'].'" href="/admin/behat/report/' . $result['rid'] .'" target="_blank"><span class="hidden">' . $pass_fail . '</span>' . $status . '</a>';
    } else {
        $output = 'n/a';
    }
    return array('pass_fail' => $pass_fail, 'icon' => $output, 'original' => $result, 'rid' => $rid);
}


/**
 * Build out the html edit table
 * This is the output that allows the
 * user to edit the test
 *
 * @param $vars
 * @return mixed
 */
function theme_behat_editor_file_to_html_edit($vars) {
    $build['file_text'] = array(
        '#markup' => $vars['file_text']
    );
    return $build;
}

/**
 * Hook the theme to alter the output
 * Keeping more complex code in the preprocess
 * function.
 *
 * @param $variables
 */
function template_preprocess_behat_editor_file_to_html_edit(&$variables) {
    composer_manager_register_autoloader();
    $file_text = $variables['file_text'];
    //@todo should not have` to pass all this to instantiate File class
    $request['scenario'] = $file_text;
    $file = new Drupal\BehatEditor\File($request, FALSE, FALSE, 'html_edit');
    $make_it_html = theme('behat_editor_item_list', $vars = array('items' => $file->output_file_text_to_html_array($file_text), 'attributes' => array('class' => 'scenario sortable ui-sortable', 'data-mode' => 'create-mode')));
    $variables['file_text'] = $make_it_html;
}


/**
 * from theme_item_list
 * need a bit more control over the format.
 *
 * @param $variables
 * @return string
 */
function theme_behat_editor_item_list($variables) {
    $items = $variables['items'];
    $type = 'ul';
    $attributes = $variables['attributes'];
    $output = '<div class="item-list">';

    if (!empty($items)) {
        $output .= "<$type" . drupal_attributes($attributes) . '>';
        $num_items = count($items);
        $i = 0;
        foreach ($items as $item) {
            $id = '';
            $attributes = array();
            $data = '';
            $i++;
            $id = (isset($item['id'])) ? "id=\"{$item['id']}\"" : '';
            foreach ($item as $key => $value) {
                if ($key == 'data') {
                    if(is_array($value)) {
                        //@todo get this to be drupal_render
                        $data = _behat_editor_check_for_render_field($value);
                    } else {
                        $data = $value;
                    }
                }
                elseif ($key == 'children') {
                    $children = $value;
                }
                else {
                    $attributes[$key] = $value;
                }
            }
            if ($i == 1) {
                $attributes['class'][] = 'first';
            }
            if ($i == $num_items) {
                $attributes['class'][] = 'last';
            }
            $output .= '<li ' . $id . drupal_attributes($attributes) . '>' . $data . "</li>\n";
        }
        $output .= "</$type>";
    }
    $output .= '</div>';
    return $output;
}

/**
 * Render out the row in the item list
 *
 * @param $value
 * @return string
 *
 * @todo use drupal_render instead
 */
function _behat_editor_check_for_render_field($value) {
    $field_name = key($value);

    $id_input = (isset($value[$field_name]['#id'])) ? "id=\"{$value[$field_name]['#id']}\"" : '';
    $name = (isset($value[$field_name]['#name'])) ? "name=\"{$value[$field_name]['#name']}\"" : '';
    $class = (isset($value[$field_name]['#class'])) ? "class=\"{$value[$field_name]['#class']}\"" : '';
    $value = $value[$field_name]['#value'];
    $data = "<input type='hidden' value=\"{$value}\" $id_input $name $class>";
    return $data;
}


/**
 * Output file to HTML View only
 *
 * @param $vars
 * @return mixed
 */
function theme_behat_editor_file_to_html_view($vars) {
    $build['file_text'] = array(
        '#markup' => $vars['file_text']
    );
    return $build;
}

/**
 * Preprocess the html to view only
 *
 * @param $variables
 */
function template_preprocess_behat_editor_file_to_html_view(&$variables) {
    composer_manager_register_autoloader();
    $file_text = $variables['file_text'];
    $request['scenario'] = $file_text;
    $file = new Drupal\BehatEditor\File($request, FALSE, FALSE, 'html_view');
    $make_it_html = theme('item_list', $vars = array('attributes' => array('class' => array('scenario'), 'data-mode' => 'view-mode'), 'items' => $file->output_file_text_to_html_array($file_text)));
    $variables['file_text'] = $make_it_html;
}

/**
 * Shared questions
 *
 * @return mixed
 */
function _behat_editor_questions_form() {
    $form['questions'] = array(
        '#type' => 'fieldset',
        '#title' => t('Build your test using Steps below'),
        '#attributes' => array('class' => ''),
        '#weight' => -100,
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
    );
    
    $form['questions']['feature_name'] = array(
        '#type' => 'fieldset',
        '#title'=> 'Feature Name',
        '#weight'=> -10,
    );

    $form['questions']['feature_name']['feature'] = array(
        '#type' => 'textfield',
        '#name' => 'feature',
        '#title' => t('Feature:'),
        '#description' => t('Name the Feature. This will name the Test<br>
        <a href="http://docs.behat.org/guides/1.gherkin.html#features" target="_blank">Read More</a>'),
        '#attributes' => array(
            'class' => array('col-mg-12', 'feature_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('A simple descriptive name of the feature'),
        ),
    );

    $form['questions']['feature_name']['feature_button'] = array(
        '#type' => 'button',
        '#value' => t('Name it'),
        '#name' => 'feature_button',
        '#attributes' => array(
            'data-step-group' => array('feature_group'),
            'data-method' => array('replace'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );
    $form['questions']['test_background'] = array(
        '#type' => 'fieldset',
        '#title'=> 'Test Background',
        '#weight'=> -8,
    );
    /** Background */
    $form['questions']['test_background']['background'] = array(
        '#type' => 'textfield',
        '#name' => 'background',
        '#title' => t('Background:'),
        '#description' => t('Backgrounds run at the start so need to be at the top.
            They are great for a repeat task like logging in prior to each Scenerio in this one Feature.
            <a href="http://docs.behat.org/guides/1.gherkin.html#backgrounds" target="_blank">Read More</a>'),
        '#attributes' => array(
            'class' => array('col-mg-12', 'background_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('A simple descriptive name of the background'),
        ),
    );

    $form['questions']['test_background']['background_button'] = array(
        '#type' => 'button',
        '#value' => t('Name it'),
        '#name' => 'background_button',
        '#attributes' => array(
            'data-step-group' => array('background_group'),
            'data-method' => array('replace'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );
    /**Starting point Group */
    $form['questions']['starting_point'] = array(
        '#type' => 'fieldset',
        '#title'=> 'Starting Point',
        '#weight'=> -8,
    );
        /** Scenario */
    $form['questions']['starting_point']['scenario'] = array(
        '#type' => 'textfield',
        '#name' => 'scenario',
        '#title' => t('Scenario:'),
        '#description' => t('What do you want to call this Scenario?<br>
        <a href="http://docs.behat.org/guides/1.gherkin.html#scenario" target="_blank">Read More</a>'),
        '#attributes' => array(
            'class' => array('col-mg-12', 'scenario_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('A simple descriptive name of the scenario'),
        ),
    );


    $form['questions']['starting_point']['scenario_button'] = array(
        '#type' => 'button',
        '#value' => t('Name it'),
        '#name' => 'scenario_button',
        '#attributes' => array(
            'data-step-group' => array('scenario_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );
    
    $form['questions']['starting_point']['when_i_go_to']['url'] = array(
        '#type' => 'textfield',
        '#title' => t('Given I am on'),
        '#description' => t('When I go to a URL. Full URL is best'),
        '#name' => 'url',
        '#attributes' => array(
            'placeholder' =>  t('a full website address'),
            'class' => array('col-mg-12', 'url_group'),
            'data-type' => array('textfield'),
        ),
        '#suffix' => '<hr>'
    );

    $form['questions']['starting_point']['when_i_go_to']['url_button'] = array(
        '#type' => 'button',
        '#name' => 'go_to',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-step-group' => array('url_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );
    
    /** CHECKBOX or Radio */
    $form['questions']['checkboxes_and_radios'] = array(
          '#type' => 'fieldset',
          '#title'=> 'Checkboxes, Radios',
    );
    
    $form['questions']['checkboxes_and_radios']['when_i_check_uncheck']['when_i_check_uncheck_select'] = array(
            '#type' => 'select',
            '#title' => t('And I'),
            '#name' => 'when_i_check_uncheck_select',
            '#prefix' => '<div class="qualifier">',
            '#suffix' => '</div>',
            '#description' => t('Either if the checkbox should be checked or not?'),
            '#options' => array('check' => t('check'), 'uncheck' => t('uncheck')),
            '#validated' => TRUE,
            '#attributes' => array(
                'class' => array('col-mg-12', 'when_i_check_uncheck_value_group'),
                'data-type' => array('select'),
            ),
        );
    $form['questions']['checkboxes_and_radios']['when_i_check_uncheck']['when_i_check_uncheck_type'] = array(
            '#type' => 'select',
            //'#title' => t('And I'),
            '#name' => 'when_i_check_uncheck_type',
            '#prefix' => '<div class="qualifier">',
            '#suffix' => '</div>',
            '#description' => t('Choose the type eg a css path like div > p:nth-child(1) or name|label|id'),
            '#options' => array(
                            'the element' => t('the element (css path)'),
                            ' ' => t('name|label|id'),
                            'the xpath' => t('the xpath'),
                          ),
            '#validated' => TRUE,
            '#attributes' => array(
                'class' => array('col-mg-12', 'when_i_check_uncheck_group'),
                'data-type' => array('select'),
            ),
        );
    $form['questions']['checkboxes_and_radios']['when_i_check_uncheck']['when_i_check_uncheck_css'] = array(
        '#type' => 'textfield',
        '#name' => 'when_i_check_uncheck_css',
        '#attributes' => array(
            'class' => array('col-mg-12', 'when_i_check_uncheck_value_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('the name|id|label of the checkbox'),
        ),
    );

    $form['questions']['checkboxes_and_radios']['when_i_check_uncheck']['when_i_check_uncheck_button'] = array(
        '#type' => 'button',
        '#name' => 'when_i_check_uncheck_button',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-step-group' => array('when_i_check_uncheck_value_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );

    /** IS CHECKED  */
    $form['questions']['checkboxes_and_radios']['is_checked_not_checked']['is_checked_not_checked_type'] = array(
        '#type' => 'select',
        '#title' => t('Then the'),
        '#name' => 'is_checked_not_checked_type',
        '#description' => t('Choose the type eg a css path like div > p:nth-child(1) or name|label|id'),
        '#options' => array(
                        'the element' => t('the element (css path)'),
                        ' ' => t('name|label|id'),
                        'the xpath' => t('the xpath'),
                      ),
        '#validated' => TRUE,
        '#attributes' => array(
            'class' => array('col-mg-12', 'is_checked_not_checked_group'),
            'data-type' => array('select'),
        ),
    );
    
    $form['questions']['checkboxes_and_radios']['is_checked_not_checked']['is_checked_not_checked_name'] = array(
        '#type' => 'textfield',
        '#name' => 'is_checked_not_checked_name',
        '#attributes' => array(
            'class' => array('col-mg-12', 'is_checked_not_checked_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('the name|id|label of the checkbox'),
        ),
    );
    
    $form['questions']['checkboxes_and_radios']['is_checked_not_checked']['is_checked_not_checked_select'] = array(
        '#type' => 'select',

        '#name' => 'is_checked_not_checked_select',
        '#prefix' => '<div class="qualifier">',
        '#suffix' => '</div>',
        '#description' => t('Either if the checkbox should be checked or not?'),
        '#options' => array('checkbox should be checked' => t('checkbox should be checked'), 'checkbox should not be checked' => t('checkbox should not be checked')),
        '#validated' => TRUE,
        '#attributes' => array(
            'class' => array('col-mg-12', 'is_checked_not_checked_value_group'),
            'data-type' => array('select'),
        ),
    );

    $form['questions']['checkboxes_and_radios']['is_checked_not_checked']['is_checked_not_checked_button'] = array(
        '#type' => 'button',
        '#name' => 'is_checked_not_checked_button',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-step-group' => array('is_checked_not_checked_value_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );


    /** SELECT LIST */
    $form['questions']['selects'] = array(
          '#type' => 'fieldset',
          '#title'=> 'Select Lists',
        );
    

    $form['questions']['selects']['and_i_select']['and_i_select_select'] = array(
        '#type' => 'textfield',
        '#title' => t('And I select'),
        '#name' => 'and_i_select_select',
        '#attributes' => array(
            'class' => array('col-mg-12', 'and_i_select_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('the select list option goes here'),
        ),
    );

    $form['questions']['selects']['and_i_select']['and_i_select_type'] = array(
        '#type' => 'select',
        '#title'=> t('from'),
        '#name' => 'and_i_select_type',
        '#description' => t('Choose the type eg a css path like div > p:nth-child(1) or name|label|id'),
        '#options' => array(
                        'the element' => t('the element (css path)'),
                        ' ' => t('name|label|id'),
                        'the xpath' => t('the xpath'),
                      ),
        '#validated' => TRUE,
        '#attributes' => array(
            'class' => array('col-mg-12', 'and_i_select_group'),
            'data-type' => array('select'),
        ),
    );
    
    $form['questions']['selects']['and_i_select']['and_i_select_name'] = array(
        '#type' => 'textfield',
        '#name' => 'and_i_select_name',
        '#attributes' => array(
            'class' => array('col-mg-12', 'and_i_select_group'),
            'data-type' => array('textfield'),
            //'placeholder' =>  t('the name|id|label of the checkbox'),
        ),
    );

    $form['questions']['selects']['and_i_select']['and_i_select_button'] = array(
        '#type' => 'button',
        '#name' => 'and_i_select_button',
        '#value' => t('Add'),
        '#description' => t('Choose which element to select from a select list.'),
        '#attributes' => array(
            'data-step-group' => array('and_i_select_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );


    /** Group: Form Fields and Elements **/

    $form['questions']['form_fields_and_elements'] = array(
        '#type' => 'fieldset',
        '#title'=> 'Form Fields',
    );

    /**
     * Using this group to test
     * more dynamic method of
     * rendering forms and later js
     */

    $form['questions']['form_fields_and_elements']['and_i_fill_in']['and_i_fill_in_name'] = array(
        '#type' => 'textfield',
        '#title' => t('And I fill in'),
        '#name' => 'and_i_fill_in_name',
        '#attributes' => array(
            'class' => array('col-mg-12', 'and_i_fill_in_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('a form field name'),
        ),
    );

    /**
     * Type works good for the qualifier type text
     * but the type needs to be in the prefix due to how it is
     * rendered.
     * @todo test in non bootstrap render
     */
    $form['questions']['form_fields_and_elements']['and_i_fill_in']['and_i_fill_in_with'] = array(
        '#title' => 'with',
        '#type' => 'item',
        '#prefix' => '<div data-type="qualifier" class="and_i_fill_in_group">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('col-mg-12', 'and_i_fill_in_group'),
        ),
    );

    $form['questions']['form_fields_and_elements']['and_i_fill_in']['and_i_fill_in_text'] = array(
        '#type' => 'textfield',
        '#name' => 'form_and_i_fill_in_text',
        '#attributes' => array(
            'class' => array('col-mg-12', 'and_i_fill_in_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('with the following text field'),
        ),
    );

    $form['questions']['form_fields_and_elements']['and_i_fill_in']['and_i_fill_in_button'] = array(
        '#type' => 'button',
        '#name' => 'and_i_fill_in_button',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-step-group' => array('and_i_fill_in_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );


    $form['questions']['form_fields_and_elements']['and_i_click_on_a']['and_i_click_on_a_select'] = array(
        '#type' => 'select',
        '#title' => t('And I'),
        '#name' => 'and_i_click_on_a_select',
        '#description' => t('And I click or press on a link or button <b>called</b>'),
        '#options' => array('click' => t('click'), 'press' => t('press'), 'follow' => t('follow (link)')),
        '#validated' => TRUE,
        '#attributes' => array(
            'class' => array('col-mg-12', 'and_i_click_on_a_group'),
            'data-type' => array('select'),
        ),
    );
    
    $form['questions']['form_fields_and_elements']['and_i_click_on_a']['and_i_click_on_a_type'] = array(
        '#type' => 'select',
        '#name' => 'and_i_click_on_a_type',
        '#description' => t('Choose the type eg a css path like div > p:nth-child(1) or name|label|id'),
        '#options' => array(
                        'the element' => t('the element (css path)'),
                        ' ' => t('name|label|id'),
                        'the xpath' => t('the xpath'),
                      ),
        '#validated' => TRUE,
        '#attributes' => array(
            'class' => array('col-mg-12', 'and_i_click_on_a_group'),
            'data-type' => array('select'),
        ),
    );

    $form['questions']['form_fields_and_elements']['and_i_click_on_a']['and_i_click_on_a_text'] = array(
        '#type' => 'textfield',
        '#name' => 'and_i_click_on_a_text',
        '#attributes' => array(
            'class' => array('col-mg-12', 'and_i_click_on_a_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('enter a link of button name here'),
        ),
    );

    $form['questions']['form_fields_and_elements']['and_i_click_on_a']['and_i_click_on_a_button'] = array(
        '#type' => 'button',
        '#name' => 'and_i_click_on_a_button',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-method' => array('append'),
            'data-step-group' => array('and_i_click_on_a_group'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );

    $form['questions']['form_fields_and_elements']['then_i_should_see']['then_i_should_see_select'] = array(
        '#type' => 'select',
        '#name' => 'then_i_should_see_select',
        '#prefix' => '<div class="qualifier">',
        '#suffix' => '</div>',
        '#title' => t('Then I'),
        '#description' => t('Then I should see or not see the following text'),
        '#options' => array('should see' => t('see'), 'should not see' => t('not see'),'should see this class exists' => t('should see this class exists')),
        '#validated' => TRUE,
        '#attributes' => array(
            'class' => array('col-mg-12', 'then_i_should_see_group'),
            'data-type' => array('select'),
        ),
    );

    $form['questions']['form_fields_and_elements']['then_i_should_see']['then_i_should_see_text'] = array(
        '#type' => 'textarea',
        '#name' => 'then_i_should_see_text',
        '#attributes' => array(
            'class' => array('col-mg-12', 'then_i_should_see_group'),
            'data-type' => array('textarea'),
            'placeholder' =>  t('some text on that page'),
        ),
    );

    $form['questions']['form_fields_and_elements']['then_i_should_see']['then_i_should_see_button'] = array(
        '#type' => 'button',
        '#name' => 'then_i_should_see_button',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-method' => array('append'),
            'data-step-group' => array('then_i_should_see_group'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );

    /** Check for a value in a field*/

    //     Then the "answer" field should contain "8.9"
    $form['questions']['form_fields_and_elements']['should_see_value']['should_see_value_type'] = array(
        '#type' => 'select',
        '#title' => t('Then'),
        '#name' => 'should_see_value_type',
        '#description' => t('Choose the type eg a css path like div > p:nth-child(1) or name|label|id'),
        '#options' => array(
                        'the element' => t('the element (css path)'),
                        'the' => t('name|label|id'),
                        'the xpath' => t('the xpath'),
                      ),
        '#validated' => TRUE,
        '#attributes' => array(
            'class' => array('col-mg-12', 'should_see_value_group'),
            'data-type' => array('select'),
        ),
    );
    
    $form['questions']['form_fields_and_elements']['should_see_value']['should_see_value_name'] = array(
        '#type' => 'textfield',
        '#name' => 'should_see_value_name',
        '#attributes' => array(
            'class' => array('col-mg-12', 'should_see_value_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('the pattern of the field'),
        ),
    );
    
    $form['questions']['form_fields_and_elements']['should_see_value']['should_see_value_select'] = array(
        '#type' => 'select',
        '#name' => 'should_see_value_select',
        '#prefix' => '<div class="qualifier">',
        '#suffix' => '</div>',
        '#description' => t('Should the value be there or not?'),
        '#options' => array('field should contain' => t('field should contain'), 'field should not contain' => t('field should not contain'), 'should have style'=> t('should have style')),
        '#validated' => TRUE,
        '#attributes' => array(
            'class' => array('col-mg-12', 'should_see_value_group'),
            'data-type' => array('select'),
        ),
    );

    $form['questions']['form_fields_and_elements']['should_see_value']['should_see_value_value'] = array(
        '#type' => 'textfield',
        '#name' => 'should_see_value_value',
        '#attributes' => array(
            'class' => array('col-mg-12', 'should_see_value_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('The value that should be in the field'),
        ),
    );

    $form['questions']['form_fields_and_elements']['should_see_value']['should_see_value_button'] = array(
        '#type' => 'button',
        '#name' => 'form_field_button',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-step-group' => array('should_see_value_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );

    /**
     * Lower level steps like checking downloads
     */
    $form['questions']['lower_level'] = array(
        '#type' => 'fieldset',
        '#title'=> 'Wait and Low Level Elements',
    );
    
    $form['questions']['lower_level']['lower_level_repsonse_code']['code_text'] = array(
        '#type' => 'textfield',
        '#title' => t('Then the response status code should be'),
        '#name' => 'code_text',
        '#attributes' => array(
            'class' => array('col-mg-12', 'lower_level_repsonse_code_group'),
            'placeholder' =>  t('200|400|401'),
            'data-type' => array('textfield'),
            'data-strip-quotes' => array('true'),
        ),
    );

    $form['questions']['lower_level']['lower_level_repsonse_code']['code_button'] = array(
        '#type' => 'button',
        '#name' => 'code_button',
        '#description' => t('Good for checking if a download link is working. You will have to tag it\'s scenario @mink:goutte'),
        '#value' => t('Add'),
        '#attributes' => array(
            'data-step-group' => array('lower_level_repsonse_code_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );
    
    $form['questions']['lower_level']['then_i_destroy_my_cookies']['then_i_destroy_my_cookies_item'] = array(
        '#title' => 'Then I destroy my cookies',
        '#type' => 'item',
        '#prefix' => '<div data-type="qualifier" class="then_i_destroy_my_cookies_group">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('col-mg-12', 'then_i_destroy_my_cookies_group'),
        ),
    );
    
    $form['questions']['lower_level']['then_i_destroy_my_cookies']['then_i_destroy_my_cookies_button'] = array(
        '#type' => 'button',
        '#name' => 'then_i_destroy_my_cookies_button',
        '#description' => t('deletes all cookies'),
        '#value' => t('Add'),
        '#attributes' => array(
            'data-step-group' => array('then_i_destroy_my_cookies_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );
    
    $form['questions']['lower_level']['then_url_should_redirect_to']['then_url_should_redirect_to_text'] = array(
        '#type' => 'textfield',
        '#title' => t('Then url should redirect to'),
        '#name' => 'then_url_should_redirect_to_text',
        '#attributes' => array(
            'class' => array('col-mg-12', 'then_url_should_redirect_to_group'),
            'placeholder' =>  t('200|400|401'),
            'data-type' => array('textfield'),
            'data-strip-quotes' => array('true'),
        ),
    );
    
    $form['questions']['lower_level']['then_url_should_redirect_to']['then_url_should_redirect_to_button'] = array(
        '#type' => 'button',
        '#name' => 'then_url_should_redirect_to_button',
        '#description' => t('checks if the url gets redirected to url'),
        '#value' => t('Add'),
        '#attributes' => array(
            'data-step-group' => array('then_url_should_redirect_to_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );
    
    $form['questions']['lower_level']['given_i_wait_for_arg_seconds']['given_i_wait_for_arg_seconds_text'] = array(
        '#type' => 'textfield',
        '#title' => t('Given I wait for'),
        '#name' => 'given_i_wait_for_arg_seconds_text',
        '#attributes' => array(
            'class' => array('col-mg-12', 'given_i_wait_for_arg_seconds_group'),
            'placeholder' =>  t('number of seconds'),
            'data-type' => array('textfield'),
            'data-strip-quotes' => array('true'),
        ),
    );
    
    $form['questions']['lower_level']['given_i_wait_for_arg_seconds']['given_i_wait_for_arg_seconds_button'] = array(
        '#type' => 'button',
        '#name' => 'given_i_wait_for_arg_seconds_button',
        '#description' => t('Waits for n seconds'),
        '#value' => t('Add'),
        '#attributes' => array(
            'data-step-group' => array('given_i_wait_for_arg_seconds_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );

    return $form;
}

/**
 * Output existing tests
 *
 * @return mixed
 */
function _behat_editor_existing_test() {

    $form['existing_test'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('well'), 'id' => 'existing-test'),
        '#weight' => -101,
    );

    $form['existing_test']['build'] = array(
        '#theme' => 'behat_editor_questions_area_edit'
    );
    return $form;
}

/**
 * the goal here to output these fields in the questions form.
 * But this should have been doable with hide / render.
 * @todo come back and see what my render mistake is
 */
function template_preprocess_behat_editor_questions_area_edit(&$variables) {

    $variables['features_tags_value'] = array(
        '#type' => 'hidden',
        '#attributes' => array(
            'id' => 'features-tagit-values',
        ),
        '#name' => 'features_tag_value',
    );
    $variables['features_tags_input'] = array(
        '#markup' => "<ul id='features-tagit-input'></ul><div class='help-block'>Start each tag with @. Just separate by comma for more than one tags. Tags can not have spaces.</div>"
    );
}

/**
 * Render the file to html
 *
 * @param $array
 */
function theme_behat_editor_create_html($array) {

}




