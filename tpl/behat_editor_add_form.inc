<?php

/**
 * @file Add form for the Behat Editor
 *
 */

use Drupal\BehatEditor;

/**
 * Implements hook_form().
 *
 * Form for Add file interface
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
function behat_editor_add_form($form, $form_state) {
    composer_manager_register_autoloader();
    behat_editor_add_bootstrap();
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
    drupal_add_library('system', 'ui.sortable');
    drupal_add_library('system', 'ui.autocomplete');
    libraries_load('behat_editor_tag_it');
    libraries_load('behat_editor_data_table');
    libraries_load('behat_editor_date_format');
    $module = BEHAT_EDITOR_DEFAULT_FOLDER;
    $filename_no_ext = REQUEST_TIME;
    $filename = $filename_no_ext . '.feature';
    $default_path = variable_get('behat_editor_default_folder', BEHAT_EDITOR_DEFAULT_FOLDER);

    drupal_add_js(array('behat_editor' => array('gherkinGeneratorDefaultPath' => $default_path)), 'setting');

    $path = drupal_get_path('module', 'behat_editor');
    $form['#attached']['js']['behat_editor_tags'] = $path . '/js/jquery.behat_editor.apply_tags.js';
    $form['#attached']['js']['behat_editor_close'] = $path . '/js/jquery.behat_editor.close_button.js';
    $form['#attached']['js']['behat_editor_app'] = $path . '/js/behat_editor_app.js';
    $form['#attached']['js']['behat_editor_results'] = $path . '/js/behat_editor_results.js';

    $form['#attached']['js']['behat_editor_run'] = $path . '/js/behat_editor_run.js';
    $form['#attached']['js']['behat_editor_add'] = $path . '/js/behat_editor_add.js';
    $form['#attached']['js']['behat_editor_save'] = $path . '/js/behat_editor_save.js';
    $form['#attached']['js']['behat_editor_sbuilder'] = $path . '/js/behat_editor_scenario_builder.js';

    $form['#attached']['css']['behat_app_css'] = $path . '/css/behat_editor_app.css';
    $form['#attached']['css']['behat_add_css'] = $path . '/css/behat_editor_add.css';
    $form['#attributes'] = array('class' => array('scenario-builder'));
    $form['header'] = array(
        '#markup' => '<h3>Feature: ' . $filename . '</h3>'
    );

    $form['actions'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('col-lg-12', 'well', 'actions'))
    );

    $form['actions']['run_test'] = run_test_array($filename_no_ext, $default_path, 'create_run');
    $form['actions']['add_test'] = add_test_array($filename_no_ext, $default_path);
    $form['actions']['admin_area'] = admin_button_array();

    $form['questions_area'] = array(
        '#type' => 'container',
        '#attributes' => array('id' => 'accordion_questions', 'class' => array('col-sm-6', 'panel-group'))
    );

    $form['questions_area']['intro'] = array(
        '#theme' => 'behat_editor_questions_area_intro',
        '#prefix' => '<div class="nothing-here">',
        '#suffix' => '</div>',
    );

    $form['questions_area']['filename'] = array(
        '#type' => 'textfield',
        '#name' => 'filename',
        '#title' => t('Filename:'),
        '#default_value' => $filename,
        '#description' => t('What do you want to call this test? Only lowercase alpha/numeric with underscores. Finally end the name in .feature'),
        '#prefix' => '<div class="well">',
        "#suffix" => '</div>'
    );

    $form['questions_area']['existing_test'] = _behat_editor_existing_test();

    $form['questions_area']['question'] =  _behat_editor_questions_form();

    behat_editor_current_test_container($form);

    BehatEditor\BehatSettingsFormHelper::behatSettingsContainer($form);

    $title =  "Add File";
    $body = "The file <span class='filename'>coming soon..</span> will be added and you will be redirected to the Edit page..";
    $buttons = array(
        'save' => array('title' => "Continue", 'class' => array('btn', 'btn-danger', 'confirm-add')),
        'delete' => array('title' => 'Cancel', 'class' => array('btn', 'btn-success'), 'data' => array('dismiss' => "modal"))
    );
    $form['message']['#markup'] = behat_editor_modal($title, $body, $buttons);

    return $form;
}