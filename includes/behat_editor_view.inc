<?php
use Drupal\BehatEditor\File;

/**
 * @file File for viewing the Tests
 * You can see the related js file as well in the js folder
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

function behat_editor_view_form($form, $form_state, $module, $filename) {
    composer_manager_register_autoloader();
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');

    $file = new File($_REQUEST, $module, $filename, FALSE);
    $file_info = $file->get_file_info();

    $path = drupal_get_path('module', 'behat_editor');
    $form['#theme'] = array('theme_behat_editor_view');

    $form['#attached']['js']['behat_editor_app'] = $path . '/js/behat_editor_app.js';
    $form['#attached']['js']['behat_editor_run'] = $path . '/js/behat_editor_view.js';

    $form['#attached']['css']['behat_app_css'] = $path . '/css/behat_editor_app.css';
    $form['#attached']['css']['behat_view_css'] = $path . '/css/behat_editor_view.css';

    $form['header'] = array(
        '#markup' => '<h3>Feature: ' . $filename . '</h3>'
    );

    $file_to_html = theme('behat_editor_file_to_html_view', $vars = array('file_text' => $file_info['scenario']));

    $form['actions'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('col-lg-12', 'well'))
    );

    $form['actions']['run_test'] = array(
        '#type' => 'link',
        '#title' => t('Run Test'),
        '#name' => 'run_test',
        '#href' => "admin/behat/run/$module/$filename",
        '#attributes' => array('class' => array('btn', 'btn-success', 'run')),
    );

    $form['actions']['edit_test'] = array(
        '#type' => 'link',
        '#name' => 'edit_test',
        '#title' => t('Edit Test'),
        '#href' => "admin/behat/edit/$module/$filename",
        '#attributes' => array('class' => array('btn', 'btn-info', 'edit')),
    );

    $form['actions']['admin_area'] = array(
        '#type' => 'link',
        '#title' => t('Admin'),
        '#name' => 'admin_area',
        '#href' => "admin/behat/index",
        '#attributes' => array('class' => array('btn', 'btn-warning', 'back')),
    );

    $form['question'] = array(
        '#prefix' => '<div class="panel panel-default col-md-6">',
        '#suffix' => '</div>',
        '#markup' => theme('behat_editor_view_questions_area', array('questions' => $file_to_html['file_text'], 'actions' => array()))
    );

    $form['results'] = array(
        '#markup' => theme('behat_editor_results_area', $args = array('params' => array()))
    );

    return $form;
}