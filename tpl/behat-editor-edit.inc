<?php

use Drupal\BehatEditor;

/**
 * @file functions needed to Edit a file
 */


/**
 * Implements hook_form().
 *
 * Form to take a file and turn it into editable html
 * You can see the related js file as well in the js folder
 *
 * @param module
 *   this should be the machine_name
 * @param filename
 *   this should be the filename.feature extension
 * @return array
 *
 * @todo my "controller" seems to have a ton of view related work?
 */
function behat_editor_edit_form($form, $form_state, $module, $filename) {
    behat_editor_add_bootstrap();
    module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
    drupal_add_library('system', 'ui.sortable');
    drupal_add_library('system', 'ui.autocomplete');
    libraries_load('behat_editor_tag_it');
    libraries_load('behat_editor_data_table');
    libraries_load('behat_editor_date_format');
    composer_manager_register_autoloader();

    $file = new BehatEditor\FileController();
    $filename = array_pop(arg());
    $module = arg(3);
    $service_path = array_slice(arg(), 3, count(arg()));
    $params = array(
        'service_path' => $service_path,
        'module' => $module,
        'filename' => $filename,
        'action' => 'edit'
    );
    drupal_alter('behat_editor_before_edit', $params);
    //need to pass this for delete to work
    $_SESSION['delete_path'] = $service_path;

    $file_info = $file->show($params);

    drupal_alter('behat_editor_edit', $file_info);

    $path = drupal_get_path('module', 'behat_editor');
    dpm($file_info);

    $form['#attached']['js']['behat_editor_tags'] = $path . '/js/jquery.behat_editor.apply_tags.js';
    $form['#attached']['js']['behat_editor_close'] = $path . '/js/jquery.behat_editor.close_button.js';
    $form['#attached']['js']['behat_editor_app'] = $path . '/js/behat_editor_app.js';
    $form['#attached']['js']['behat_editor_results'] = $path . '/js/behat_editor_results.js';
    $form['#attached']['js']['behat_editor_sids'] = $path . '/js/behat_editor_sids.js';

    $form['#attached']['js']['behat_editor_run'] = $path . '/js/behat_editor_run.js';
    $form['#attached']['js']['behat_editor_add'] = $path . '/js/behat_editor_edit.js';
    $form['#attached']['js']['behat_editor_save'] = $path . '/js/behat_editor_save.js';
    $form['#attached']['js']['behat_editor_delete'] = $path . '/js/behat_editor_delete.js';
    $form['#attached']['js']['behat_editor_sbuilder'] = $path . '/js/behat_editor_scenario_builder.js';

    $form['#attached']['css']['behat_app_css'] = $path . '/css/behat_editor_app.css';
    $form['#attached']['css']['behat_add_css'] = $path . '/css/behat_editor_edit.css';
    $form['#attached']['css']['behat_add_css'] = $path . '/css/behat_editor_help_bar.css';

    $form['questions_area'] = array(
        '#type' => 'container',
        '#attributes' => array('id' => 'accordion_questions', 'class' => array('col-sm-6', 'panel-group'))
    );

    if ( !empty($_SERVER['QUERY_STRING'])) {
        $query_array = array();
        $query = $_SERVER['QUERY_STRING'];
        parse_str($query, $query_array);
        if ( isset($query_array['ace'])) {
            //1 Make a text box instead
            //2 place the $file_info in there
            drupal_add_css($path . '/js/libraries/ace/ace_setup.css');
            drupal_add_js($path . '/js/libraries/ace/ace.js');
            drupal_add_js($path . '/js/libraries/ace/ace_setup.js');
            $plain_text = $file_info['scenario'];
            $form['ace']['existing_test_ace'] = array(
                '#prefix' => '<div id="ace-wrapper" class="col-sm-6 panel-group form-wrapper"><h4>Your Test</h4><hr><div id="test-textbox">',
                '#markup' =>  $plain_text,
                '#suffix' => '</div></div>'
            );
//            $form['questions_area']['existing_test_ace'] = array(
//                '#markup' => '<div id="custom_ace_container">
//                                <div name="custom_ace" id="custom_ace" style="border: 1px solid #DFDFDF; -moz-border-radius: 3px; -webkit-border-radius: 3px; border-radius: 3px; width: 100%; height: 400px; position: relative;"></div>
//                              </div>'
//            );
//            $form['questions_area']['existing_test'] = array(
//                '#prefix' => '<h4>Your Test</h4><hr>',
//                '#type'   => 'textarea',
//                '#value' =>  $plain_text,
//                '#attributes' => array('id' => 'test-textbox')
//            );
        }
    }
    $file_to_html = theme('behat_editor_file_to_html_edit', $vars = array('file_text' => $file_info['scenario']));
    $form['questions_area']['existing_test'] = array(
        '#markup' => theme('behat_editor_view_questions_area', array('questions' => $file_to_html['file_text'], 'actions' => array())),
        '#prefix' => '<div class="well test"><h4>Your Test</h4><hr>',
        '#suffix' => '</div>'
    );
    $form['questions_area']['question'] =  _behat_editor_questions_form();

    $form['#attributes'] = array('class' => array('scenario-builder'));
    $form['header'] = array(
        '#markup' => '<h3>Editing: ' . $filename . '</h3>'
    );

    $form['actions'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('col-lg-12', 'well', 'actions')),
        '#weight' => -100
    );

    $form['actions']['save_test'] = update_test_array($file_info['filename_no_ext'], $module);

    $form['actions']['run_test'] = run_test_array($file_info['filename_no_ext'], $module, 'create_run');

    if(user_access('behat delete test')) {
        $form['actions']['delete_test'] = delete_test_array($file_info['filename_no_ext'], $module);
    }

    $form['actions']['admin_area'] = admin_button_array();

    $help =  _behat_editor_make_help_link('index.html');

    $form['actions']['help'] = array(
        '#prefix' => '<hr><div class="text-success">',
        '#markup' => t('There are icons like this one !help for you to get help through the site', array('!help' => $help)),
        '#suffix' => '</div>',
        '#weight' => 200
    );




    $help =  _behat_editor_make_help_link('filename.html');

    $form['questions_area']['filename'] = array(
        '#type' => 'textfield',
        '#name' => 'filename',
        '#title' => t('Filename: !link', array('!link' => $help)),
        '#default_value' => $filename,
        '#attributes' => array('disabled' => array('disabled')),
        '#description' => t('What do you want to call this test? Only lowercase alpha/numeric with underscores. Finally end the name in .feature'),
        '#prefix' => '<div class="well">',
        "#suffix" => '</div>'
    );

    behat_editor_current_test_container($form);
    BehatEditor\BehatSettingsFormHelper::behatSettingsContainer($form);
    behat_editor_results_container($form);
    behat_editor_delete_modal($form, $filename);
    behat_editor_results_history_modal($form);

    $form['filename'] = array('#type' => 'hidden', '#default_value' => $file_info['filename_no_ext']);
    $form['module'] = array('#type' => 'hidden', '#default_value' => $module);

    return $form;
}

