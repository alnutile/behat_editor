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
    foreach($files_array as $key => $value) {
        foreach($value as $row) {
            $file_name = $row['filename'];
            $file_name_no_ext = $row['filename_no_ext'];
            $module_name = $key;
            $tags = implode(', ', $row['tags_array']);
            $rows[] = array(
                'data' =>
                array
                (
                    _show_file_link($file_name, $module_name),
                    _show_module_name($file_name, $module_name),
                    $tags,
                    _show_link($file_name, $module_name, $link_text = 'view'),
                    _show_link($file_name, $module_name, $link_text = 'edit'),
                    _show_delete_link($file_name_no_ext, $module_name, $link_text = 'delete'),
                )
            );
        }
    };

    $header = array(
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

    return $behat_table;
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

    /** Scenario */
    $form['questions']['test_name']['scenario'] = array(
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


    $form['questions']['test_name']['scenario_button'] = array(
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


    $form['questions']['when_i_go_to']['url'] = array(
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

    $form['questions']['when_i_go_to']['url_button'] = array(
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

    $form['questions']['when_i_check']['header']['#markup'] = t('<h2>Checkboxes and Radio Buttons</h2>');
    $form['questions']['when_i_check']['checkbox'] = array(
        '#type' => 'textfield',
        '#title' => t('And I check'),
        '#name' => 'checkbox',
        '#attributes' => array(
            'class' => array('col-mg-12', 'when_i_check_group'),
            'placeholder' =>  t('id|name|label|value of element'),
            'data-type' => array('textfield'),
        ),
        '#suffix' => '<hr>'
    );

    $form['questions']['when_i_check']['checkbox_button'] = array(
        '#type' => 'button',
        '#name' => 'checkbox_button',
        '#description' => t('Used on Checkbox and Radio buttons'),
        '#value' => t('Add'),
        '#attributes' => array(
            'data-step-group' => array('when_i_check_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );

    /** UN-CHECK CHECKBOX */
    $form['questions']['when_i_uncheck']['uncheckbox'] = array(
        '#type' => 'textfield',
        '#title' => t('And I uncheck'),
        '#name' => 'uncheckbox',
        '#attributes' => array(
            'class' => array('col-mg-12', 'when_i_uncheck_group'),
            'placeholder' =>  t('value of element'),
            'data-type' => array('textfield'),
        ),
        '#suffix' => '<hr>'
    );


    $form['questions']['when_i_uncheck']['uncheckbox_button'] = array(
        '#type' => 'button',
        '#name' => 'uncheckbox_button',
        '#description' => t('Used on Checkbox to Uncheck'),
        '#value' => t('Add'),
        '#attributes' => array(
            'data-step-group' => array('when_i_uncheck_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );

    /** IS CHECKED  */
    $form['questions']['is_checked']['checked'] = array(
        '#type' => 'textfield',
        '#title' => t('Then the'),
        '#name' => 'checked',
        '#description' => t('Checks, that checkbox with specified in|name|label|value is checked'),
        '#attributes' => array(
            'class' => array('col-mg-12', 'is_checked_group'),
            'placeholder' =>  t('use in|name|label|value'),
            'data-type' => array('textfield'),
        ),
    );

    $form['questions']['is_checked']['should_be_checked'] = array(
        '#title' => 'checkbox should be checked',
        '#type' => 'item',
        '#prefix' => '<div data-type="qualifier" class="is_checked_group">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('col-mg-12', 'is_checked_group'),
        ),
    );

    $form['questions']['is_checked']['checked_button'] = array(
        '#type' => 'button',
        '#name' => 'checked_button',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-step-group' => array('is_checked_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );

    /** IS NOT CHECKED  */
    $form['questions']['not_checked']['checked'] = array(
        '#type' => 'textfield',
        '#title' => t('Then the'),
        '#name' => 'checked',
        '#description' => t('Checks, that checkbox with specified in|name|label|value is NOT checked'),
        '#attributes' => array(
            'class' => array('col-mg-12', 'not_checked'),
            'placeholder' =>  t('use in|name|label|value'),
            'data-type' => array('textfield'),
        ),
        '#suffix' => '<hr>'
    );

    $form['questions']['not_checked']['should_not_be_checked'] = array(
        '#title' => 'checkbox should not be checked',
        '#type' => 'item',
        '#prefix' => '<div data-type="qualifier" class="not_checked">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('col-mg-12', 'not_checked'),
        ),
    );

    $form['questions']['not_checked']['not_checked_button'] = array(
        '#type' => 'button',
        '#name' => 'not_checked_button',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-step-group' => array('not_checked'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );

    /** SELECT LIST */
    $form['questions']['and_i_select']['header']['#markup'] = t('<h2>Select List</h2>');

    $form['questions']['and_i_select']['select_option'] = array(
        '#type' => 'textfield',
        '#title' => t('And I select'),
        '#name' => 'form_field_and_i_select',
        '#attributes' => array(
            'class' => array('col-mg-12', 'and_i_select_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('the select list option goes here'),
        ),
    );

    $form['questions']['and_i_select']['from'] = array(
        '#title' => 'from',
        '#type' => 'item',
        '#prefix' => '<div data-type="qualifier" class="and_i_select_group">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('col-mg-12', 'and_i_select_group'),
        ),
    );

    $form['questions']['and_i_select']['select_name'] = array(
        '#type' => 'textfield',
        '#name' => 'select_name',
        '#description' => t('This can be used on Select'),
        '#attributes' => array(
            'class' => array('col-mg-12', 'and_i_select_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('Label, Name or ID of the Select List'),
        ),
    );

    $form['questions']['and_i_select']['field_select_button'] = array(
        '#type' => 'button',
        '#name' => 'field_select_button',
        '#value' => t('Add'),
        '#description' => t('Choose which element to select from a select list.'),
        '#attributes' => array(
            'data-step-group' => array('and_i_select_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );


    /** Fill in Field */


    /**
     * Using this group to test
     * more dynamic method of
     * rendering forms and later js
     */
    $form['questions']['and_i_fill_in']['header']['#markup'] = t('
    <div class="alert alert-info">
        You are now entering into Steps. <br>
        <a href="http://docs.behat.org/guides/1.gherkin.html#steps" target="_blank">Read More</a>
    </div>
    <br>
    <h2>Text Fields, Links and Buttons</h2>');

    $form['questions']['and_i_fill_in']['form_field_name'] = array(
        '#type' => 'textfield',
        '#title' => t('And I fill in'),
        '#name' => 'form_field_and_fill_in',
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
    $form['questions']['and_i_fill_in']['with'] = array(
        '#title' => 'with',
        '#type' => 'item',
        '#prefix' => '<div data-type="qualifier" class="and_i_fill_in_group">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('col-mg-12', 'and_i_fill_in_group'),
        ),
    );

    $form['questions']['and_i_fill_in']['field_text'] = array(
        '#type' => 'textfield',
        '#name' => 'field_with_text',
        '#attributes' => array(
            'class' => array('col-mg-12', 'and_i_fill_in_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('with the following text field'),
        ),
    );

    $form['questions']['and_i_fill_in']['field_text_button'] = array(
        '#type' => 'button',
        '#name' => 'field_text_add',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-step-group' => array('and_i_fill_in_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );


    $form['questions']['and_i_click_on_a']['click_on_type'] = array(
        '#type' => 'select',
        '#title' => t('And I'),
        '#name' => 'click_on_type',
        '#description' => t('And I click or press on a link or button <b>called</b>'),
        '#options' => array('follow' => t('link'), 'press' => t('button')),
        '#validated' => TRUE,
        '#attributes' => array(
            'class' => array('col-mg-12', 'and_i_click_on_a_group'),
            'data-type' => array('select'),
        ),
    );

    $form['questions']['and_i_click_on_a']['field_text'] = array(
        '#type' => 'textfield',
        '#name' => 'click_on_text',
        '#attributes' => array(
            'class' => array('col-mg-12', 'and_i_click_on_a_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('enter a link of button name here'),
        ),
    );

    $form['questions']['and_i_click_on_a']['click_on_a'] = array(
        '#type' => 'button',
        '#name' => 'click_on_a',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-method' => array('append'),
            'data-step-group' => array('and_i_click_on_a_group'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );

    $form['questions']['then_i_should_see']['click_on_type'] = array(
        '#type' => 'select',
        '#name' => 'should_see',
        '#prefix' => '<div class="qualifier">',
        '#suffix' => '</div>',
        '#title' => t('Then I'),
        '#description' => t('Then I should see or not see the following text'),
        '#options' => array('should see' => t('see'), 'should not see' => t('not see')),
        '#validated' => TRUE,
        '#attributes' => array(
            'class' => array('col-mg-12', 'then_i_should_see_group'),
            'data-type' => array('select'),
        ),
    );

    $form['questions']['then_i_should_see']['see_not_see_some_text'] = array(
        '#type' => 'textfield',
        '#name' => 'see_not_see_some_text',
        '#attributes' => array(
            'class' => array('col-mg-12', 'then_i_should_see_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('some text on that page'),
        ),
    );

    $form['questions']['then_i_should_see']['see_not_see'] = array(
        '#type' => 'button',
        '#name' => 'see_not_see',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-method' => array('append'),
            'data-step-group' => array('then_i_should_see_group'),
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




