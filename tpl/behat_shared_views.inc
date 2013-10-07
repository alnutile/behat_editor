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
                    _show_link($file_name, $module_name, $link_text = 'delete'),
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
    //@todo should not have to pass all this to instantiate File class
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
        '#type' => 'container',
        '#attributes' => array('class' => array('well', 'pull-left')),
        '#weight' => -100
    );

    $form['questions']['test_name']['name'] = array(
        '#type' => 'textfield',
        '#title' => t('What do you want to call this test?'),
        '#attributes' => array('class' => array('span12'), 'placeholder' =>  t('A simple descriptive name of the test')),

    );

    $form['questions']['test_name']['name_button'] = array(
        '#type' => 'button',
        '#value' => t('Name it'),
        '#attributes' => array(
            'data-method' => array('replaceWith'),
            'data-test-message' => array('Scenario'),
            'data-parent-input' => array('name'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );


    $form['questions']['when_i_go_to']['url'] = array(
        '#type' => 'textfield',
        '#title' => t('When I go to'),
        '#name' => 'url',
        '#attributes' => array('class' => array('span12'), 'placeholder' =>  t('a full website address')),
    );

    $form['questions']['when_i_go_to']['url_button'] = array(
        '#type' => 'button',
        '#name' => 'go_to',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-method' => array('replaceWith'),
            'data-test-message' => array('Given I am on'),
            'data-parent-input' => array('url'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );


    $form['questions']['and_i_fill_in']['form_field_name'] = array(
        '#type' => 'textfield',
        '#title' => t('And I fill in'),
        '#name' => 'form_field_and_fill_in',
        '#attributes' => array('class' => array('span12'), 'placeholder' =>  t('a form field name')),
    );

    $form['questions']['and_i_fill_in']['with'] = array(
        '#markup' => 'with',
        '#prefix' => '<div class="qualifier">',
        '#suffix' => '</div>',
    );

    $form['questions']['and_i_fill_in']['field_text'] = array(
        '#type' => 'textfield',
        '#name' => 'field_with_text',
        '#attributes' => array('class' => array('span12'), 'placeholder' =>  t('with the following text field')),
    );

    $form['questions']['and_i_fill_in']['field_text_button'] = array(
        '#type' => 'button',
        '#name' => 'field_text_add',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-method' => array('append'),
            'data-test-message' => array('And I fill in'),
            'data-parent-input' => array('form_field_and_fill_in'),
            'data-value-2' => array('field_with_text'),
            'data-middle-words' => array('with'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );


    $form['questions']['and_i_click_on_a']['click_on_type'] = array(
        '#type' => 'select',
        '#name' => 'click_on_type',
        '#title' => t('And I click on a'),
        '#options' => array('follow' => t('link'), 'press' => t('button')),
        '#validated' => TRUE
    );

    $form['questions']['and_i_click_on_a']['called'] = array(
        '#prefix' => '<div class="qualifier">',
        '#suffix' => '</div>',
        '#markup' => 'called',
    );

    $form['questions']['and_i_click_on_a']['field_text'] = array(
        '#type' => 'textfield',
        '#name' => 'click_on_text',
        '#attributes' => array('class' => array('span12'), 'placeholder' =>  t('enter a link of button name here'))
    );

    $form['questions']['and_i_click_on_a']['click_on_a'] = array(
        '#type' => 'button',
        '#name' => 'click_on_a',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-method' => array('append'),
            'data-element-type' => array('select'),
            'data-target' => array('click_on'),
            'data-test-message' => array('And I'),
            'data-parent-input' => array('click_on_type'),
            'data-value-2' => array('click_on_text'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );

    $form['questions']['then_i_should_see']['click_on_type'] = array(
        '#type' => 'select',
        '#name' => 'should_see',
        '#prefix' => '<div class="qualifier">',
        '#suffix' => '</div>',
        '#title' => t('Then I should see'),
        '#options' => array('should see' => t('see'), 'should not see' => t('not see')),
        '#validated' => TRUE
    );

    $form['questions']['then_i_should_see']['see_not_see_some_text'] = array(
        '#type' => 'textfield',
        '#name' => 'see_not_see_some_text',
        '#attributes' => array('class' => array('span12'), 'placeholder' =>  t('some text on that page'))
    );

    $form['questions']['then_i_should_see']['see_not_see'] = array(
        '#type' => 'button',
        '#name' => 'see_not_see',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-method' => array('append'),
            'data-target' => array('see'),
            'data-element-type' => array('select'),
            'data-test-message' => array('Then I'),
            'data-parent-input' => array('should_see'),
            'data-value-2' => array('see_not_see_some_text'),
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




