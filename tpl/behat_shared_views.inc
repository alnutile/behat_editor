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
            $link = $row['action_path'];
            $latest = _behat_editor_get_latest_result($file_name, $module_name, $row);
            $all_results[$latest['rid']] = $latest['original'];
            $tags = array();
            if(!empty($row['tags_array'])){
              $tags = implode(', ', $row['tags_array']);
            }
            $rows[] = array(
                'data' =>
                    array
                        (
                            $latest['icon'],
                            _show_file_link($file_name, $module_name, $link),
                            _show_module_name($file_name, $module_name, $link),
                            $tags,
                            _show_link($file_name, $module_name, $link_text = 'view', $link),
                            _show_link($file_name, $module_name, $link_text = 'edit', $link),
                        ),
                'class' => array($latest['pass_fail']))
            ;
        }
    };

    $header = array(
        'Status',
        'Name',
        'Module/Folder',
        'Tags',
        'View',
        'Edit',
    );

    $behat_table = theme('table', array(
        'header' => $header,
        'rows' => $rows,
        'attributes' => array('class' => array('table-hover'), 'id' => 'admin-features')
    ));

    drupal_add_js(array('behat_editor' => array('results' => $all_results)), 'setting');
    return $behat_table;
}

function _behat_editor_get_latest_result($file_name, $module_name, $file_object = array()){
    composer_manager_register_autoloader();
    $path = drupal_get_path('module', 'behat_editor');
    $loader     = new Twig_Loader_Filesystem($path . '/twig/');
    $twig       = new Twig_Environment($loader);

    $output = '';
    $pass_fail = 'na';
    $rid = 0;
    $result = '';
    $results = BehatEditor\Results::getLatestResultForFile($module_name, $file_name, $file_object);
    if(isset($results['results'][0])) {

        $value = $results['results'][0];
        list($browser, $version) = explode('|', $value['settings']['browser_version']);
        $browser = ucwords($browser);
        $group   = $value['url']['gid'];
        if ($group != 0 ) {
            $group = node_load($group)->title;
        } else {
            $group = "Ran as user";
        }
        $title = "<h4>" . $value['filename'] . "</h4>";
        $created = date('m-d-Y H:i ', $value['created']);
        $os_version = $value['settings']['os_version'];
        $url        = $value['url']['base_url'];
        $nice_name  = $value['url']['nice_name'];
        $mail       = $value['user']->mail;
        $results    = $value['results'];

        $result['results'] = $twig->render('reports.html', compact('title', 'created', 'browser', 'version', 'group', 'os_version', 'url', 'nice_name'. 'mail', 'results'));

        $rid = $value['rid'];
        if($result['status'] == 0) {
            $pass_fail = "pass";
            $status = '<i class="glyphicon glyphicon-thumbs-up"></i>';
        } else {
            $pass_fail = 'fail';
            $status = '<i class="glyphicon glyphicon-thumbs-down"></i>';
        }
        $output .= '<a class="result" data-rid="'.$rid.'" href="/admin/behat/report/' . $rid .'" target="_blank"><span class="hidden">' . $pass_fail . '</span>' . $status . '</a>';
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
    $file = new Drupal\BehatEditor\FileModel();
    $params = array(
        'file_text' => $file_text,
        'parse_type' => 'html_edit',
    );
    $file_to_html_array = $file->output_file_text_to_html_array($params);
    $make_it_html = theme('behat_editor_item_list', $vars = array('items' => $file_to_html_array, 'attributes' => array('class' => 'scenario sortable ui-sortable', 'data-mode' => 'create-mode')));
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

function theme_behat_editor_form_wrapper($variables) {
}

function template_preprocess_behat_editor_form_wrapper(&$variables) {
    $variables['text_form_content'] = array();
    $text_form_hidden = array();

    foreach (element_children($variables['form']) as $key) {
        $type = $variables['form'][$key]['#type'];
        if ($type == 'hidden' || $type == 'token') {
            $text_form_hidden[] = drupal_render($variables['form'][$key]);
        }
        else {
            $variables['text_form_content'][$key] = drupal_render($variables['form'][$key]);
        }
    }
    $variables['text_form_content']['hidden'] = implode($text_form_hidden);

    $variables['text_form'] = implode($variables['text_form_content']);
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

    $file = new Drupal\BehatEditor\FileModel();
    $params = array(
        'file_text' => $file_text,
        'parse_type' => 'html_view',
    );

    $file_to_html_array = $file->output_file_text_to_html_array($params);

    $make_it_html = theme('item_list', $vars = array('attributes' => array('class' => array('scenario'), 'data-mode' => 'view-mode'), 'items' => $file_to_html_array));
    $variables['file_text'] = $make_it_html;
}

/**
 * Shared questions
 *
 * @return mixed
 */
function _behat_editor_questions_form() {

    $form['#theme'] = array('behat_editor_form_wrapper');

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
        '#maxlength' => 300,
        '#name' => 'feature',
        '#title' => t('Feature:'),
        '#description' => t('Name the Feature. This will name the Test. !help', array('!help' => _behat_editor_make_help_link('feature.html'))),
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
        '#maxlength' => 300,
        '#name' => 'scenario',
        '#title' => t('Scenario:'),
        '#description' => t('What do you want to call this Scenario? !help', array('!help' => _behat_editor_make_help_link('scenario.html'))),
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
        '#maxlength' => 300,
        '#name' => 'background',
        '#title' => t('Background:'),
        '#description' => t('Backgrounds run at the start so need to be at the top.
            They are great for a repeat task like logging in prior to each Scenerio in this one Feature. !help', array('!help' => _behat_editor_make_help_link('background.html'))),
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
        '#maxlength' => 300,
        '#title' => t('Given I am on'),
        '#description' => t('When I go to a URL. Full URL is best. !help', array('!help' => _behat_editor_make_help_link('given.html'))),
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

    /** Group: Form Fields and Elements **/

    $form['questions']['form_fields_and_elements'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="panel panel-info">
                          <div class="panel-heading"><i class="glyphicon-share glyphicon"></i>Form fields & elements</div>
                          <div class="panel-body">',
        '#suffix' => '</div>
                </div>'
    );

    $form['questions']['form_fields_and_elements']['when_i_check']['checkbox'] = array(
        '#type' => 'textfield',
        '#title' => t('And I check'),
        '#maxlength' => 300,
        '#description' => t('!help', array('!help' => _behat_editor_make_help_link('checkbox.html'))),
        '#name' => 'checkbox',
        '#attributes' => array(
            'class' => array('col-mg-12', 'when_i_check_group'),
            'placeholder' =>  t('id|name|label|value of element'),
            'data-type' => array('textfield'),
        ),
        '#suffix' => '<hr>'
    );

    $form['questions']['form_fields_and_elements']['when_i_check']['checkbox_button'] = array(
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
        '#maxlength' => 300,
        '#name' => 'uncheckbox',
        '#description' => t('!help', array('!help' => _behat_editor_make_help_link('checkbox.html'))),
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
        '#maxlength' => 300,
        '#name' => 'checked',
        '#description' => t('Checks, that checkbox with specified in|name|label|value is checked !help', array('!help' => _behat_editor_make_help_link('checkbox.html'))),
        '#attributes' => array(
            'class' => array('col-mg-12', 'is_checked_group'),
            'placeholder' =>  t('use id|name|label|value'),
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
    $form['questions']['not_checked']['not_checked'] = array(
        '#type' => 'textfield',
        '#title' => t('Then the'),
        '#name' => 'not_checked',
        '#maxlength' => 300,
        '#description' => t('Checks, that checkbox with specified in|name|label|value is NOT checked !help', array('!help' => _behat_editor_make_help_link('checkbox.html'))),
        '#attributes' => array(
            'class' => array('col-mg-12', 'not_checked'),
            'placeholder' =>  t('use id|name|label|value'),
            'data-type' => array('textfield'),
        ),
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
        '#maxlength' => 300,
        '#name' => 'form_field_and_i_select',
        '#description' => t('!help', array('!help' => _behat_editor_make_help_link('select.html'))),
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
        '#maxlength' => 300,
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
    <h2>Text Fields, Links and Buttons, Value Checking</h2>');

    $form['questions']['and_i_fill_in']['form_field_name'] = array(
        '#type' => 'textfield',
        '#title' => t('And I fill in'),
        '#name' => 'form_field_and_fill_in',
        '#maxlength' => 300,
        '#description' => t('!help', array('!help' => _behat_editor_make_help_link('fill_in_field.html'))),
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
        '#maxlength' => 300,
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
        '#description' => t('And I click or press on a link or button <b>called</b> !help', array('!help' => _behat_editor_make_help_link('press_click.html'))),
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
        '#maxlength' => 300,
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
        '#description' => t('Then I should see or not see the following text !help', array('!help' => _behat_editor_make_help_link('should_see.html'))),
        '#options' => array('should see' => t('see'), 'should not see' => t('not see')),
        '#validated' => TRUE,
        '#attributes' => array(
            'class' => array('col-mg-12', 'then_i_should_see_group'),
            'data-type' => array('select'),
        ),
    );

    $form['questions']['then_i_should_see']['see_not_see_some_text'] = array(
        '#type' => 'textarea',
        '#name' => 'see_not_see_some_text',
        '#attributes' => array(
            'class' => array('col-mg-12', 'then_i_should_see_group'),
            'data-type' => array('textarea'),
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

    /** Che the url via regular expression */
    $form['questions']['check_url']['url_to_check'] = array(
        '#type' => 'textfield',
        '#maxlength' => 300,
        '#title' => t('Then the url should match'),
        '#name' => 'url_to_check',
        '#description' => t('!help', array('!help' => _behat_editor_make_help_link('url_check.html'))),
        '#attributes' => array(
            'class' => array('col-mg-12', 'check_url_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('enter part of the URL or full url.'),
        ),
    );

    $form['questions']['check_url']['check_url_button'] = array(
        '#type' => 'button',
        '#name' => 'form_field_button',
        '#value' => t('Add'),
        '#attributes' => array(
            'data-step-group' => array('check_url_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<div class="help-block">use the full or relative URL</div><hr>'
    );

    /** Check for a value in a field*/

    //     Then the "answer" field should contain "8.9"
    $form['questions']['should_see_value']['form_field_css'] = array(
        '#type' => 'textfield',
        '#maxlength' => 300,
        '#title' => t('Then the'),
        '#name' => 'form_field_css',
        '#description' => t('!help', array('!help' => _behat_editor_make_help_link('check_element_style.html'))),
        '#attributes' => array(
            'class' => array('col-mg-12', 'should_see_value_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('the name|id|label of the field'),
        ),
    );

    $form['questions']['should_see_value']['form_field_should_should_not_contain'] = array(
        '#type' => 'select',
        '#name' => 'form_field_should_should_not_contain',
        '#prefix' => '<div class="qualifier">',
        '#suffix' => '</div>',
        '#description' => t('Should the value be there or not?'),
        '#options' => array('field should contain' => t('field should contain'), 'field should not contain' => t('field should not contain')),
        '#validated' => TRUE,
        '#attributes' => array(
            'class' => array('col-mg-12', 'should_see_value_group'),
            'data-type' => array('select'),
        ),
    );

    $form['questions']['should_see_value']['form_field_value'] = array(
        '#type' => 'textfield',
        '#maxlength' => 300,
        '#name' => 'form_field_value',
        '#attributes' => array(
            'class' => array('col-mg-12', 'should_see_value_group'),
            'data-type' => array('textfield'),
            'placeholder' =>  t('The value that should be in the field'),
        ),
    );

    $form['questions']['should_see_value']['form_field_button'] = array(
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
    $form['questions']['lower_level_repsonse_code']['header']['#markup'] = t('<h2>Testing Lower Level Items</h2>
    <small class="text-center">note the descriptions of each item. They may require special tags</small><br>
    ');
    $form['questions']['lower_level_response_code']['code'] = array(
        '#type' => 'textfield',
        '#maxlength' => 300,
        '#title' => t('Then the response status code should be'),
        '#name' => 'code',
        '#description' => t('!help', array('!help' => _behat_editor_make_help_link('status_code.html'))),
        '#attributes' => array(
            'class' => array('col-mg-12', 'lower_level_response_code_group'),
            'placeholder' =>  t('200|400|401'),
            'data-type' => array('textfield'),
            'data-strip-quotes' => array('true'),
        ),
    );

    $form['questions']['lower_level_response_code']['code_button'] = array(
        '#type' => 'button',
        '#name' => 'code_button',
        '#description' => t('Good for checking if a download link is working. You will have to tag it\'s scenario @mink:goutte'),
        '#value' => t('Add'),
        '#attributes' => array(
            'data-step-group' => array('lower_level_response_code_group'),
            'data-method' => array('append'),
            'class' => array('steps')
        ),
        '#suffix' => '<hr>'
    );
    _behat_editor_load_help_area($form);

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

function theme_behat_editor_questions_form($variables) {
}



