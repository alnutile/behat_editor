<?php
use Drupal\BehatEditor\File,
    Drupal\BehatEditor;
/**
 * @file output the form on the View page
 */

/**
 * View form to see the file in a new editable state
 *
 * @param module
 *   this should be the machine_name
 * @param filename
 *   this should be the filename.feature extension
 * @return array
 *
 */

function behat_editor_view_form($form, $form_state) {
    behat_editor_add_bootstrap();
    composer_manager_register_autoloader();
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
    libraries_load('behat_editor_data_table');
    libraries_load('behat_editor_date_format');

    //@todo make this a controller to talk to a model based on the FileBuilder
    $file = new BehatEditor\FileBuilder();
    $filename = array_pop(arg());
    $module = arg(3);
    $params = array(
        'service_path' => arg(),
        'module' => $module,
        'filename' => $filename,
        'action' => 'view'
    );
    $file_info = $file->buildObject($params);

    $path = drupal_get_path('module', 'behat_editor');
    $form['#theme'] = array('theme_behat_editor_view');

    $form['#attached']['js']['behat_editor_app'] = $path . '/js/behat_editor_app.js';
    $form['#attached']['js']['behat_editor_results'] = $path . '/js/behat_editor_results.js';
    $form['#attached']['js']['behat_editor_run'] = $path . '/js/behat_editor_run.js';
    $form['#attached']['js']['behat_editor_view'] = $path . '/js/behat_editor_view.js';
    $form['#attached']['js']['behat_editor_clone'] = $path . '/js/behat_editor_clone.js';
    $form['#attached']['js']['behat_editor_sids'] = $path . '/js/behat_editor_sids.js';

    $form['#attached']['css']['behat_app_css'] = $path . '/css/behat_editor_app.css';
    $form['#attached']['css']['behat_view_css'] = $path . '/css/behat_editor_view.css';

    $form['header'] = array(
        '#markup' => '<h3>Feature: ' . $filename . '</h3>'
    );

    $file_to_html = theme('behat_editor_file_to_html_view', $vars = array('file_text' => $file_info['scenario']));

    $form['actions'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('col-lg-12', 'well', 'actions'))
    );

    $form['actions']['run_test'] = run_test_array($file_info['filename_no_ext'], $module, 'run');
    $form['actions']['admin_area'] = admin_button_array();

    $form['actions']['edit_test'] = edit_test_array($file_info['filename_no_ext'] . ".feature", $module);

    $form['actions']['clone_test'] = clone_test_array($file_info['filename_no_ext'] . ".feature", $module);

    $form['question'] = array(
        '#prefix' => '<div class="panel panel-default col-md-6">',
        '#suffix' => '</div>',
        '#markup' => theme('behat_editor_view_questions_area', array('questions' => $file_to_html['file_text'], 'actions' => array()))
    );




    behat_editor_current_test_container($form);
    BehatEditor\BehatSettingsFormHelper::behatSettingsContainer($form);
    behat_editor_results_container($form);
    behat_editor_results_history_modal($form);
    behat_editor_clone_modal($form);


    $form['filename'] = array('#type' => 'hidden', '#name' => 'filename', '#default_value' => $file_info['filename_no_ext']);
    $form['module'] = array('#type' => 'hidden', '#default_value' => $module);

    return $form;
}
