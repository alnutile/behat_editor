<?php

/**
 * @file functions to show the Admin / Index page
 */

/**
 * Build the output for the Index page of files.
 * You can see the related js file as well in the js folder
 *
 * @todo turn this into a form so it will be easier to hook and do
 * other bulk operations
 * @return mixed
 */

function behat_editor_index() {
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
    drupal_add_library('behat_editor', 'data_table');
    $path = drupal_get_path('module', 'behat_editor');
    drupal_add_js($path . '/js/behat_editor_app.js');
    drupal_add_js($path . '/js/behat_editor_index.js');

    $build['messages_top'] = array(
        '#markup' => "<div id='messages'></div>",
        '#weight' => -100
    );

    $build['actions'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('col-lg-12', 'well'))
    );

    $build['actions']['add_test'] = array(
        '#type' => 'link',
        '#title' => t('Add Test'),
        '#href' => "admin/behat/add",
        '#attributes' => array('class' => array('btn', 'btn-success', 'add')),
    );

    //@todo convert to Files class
    $modules = _behat_editor_check_for_modules();
    $modules = array_merge($modules, _behat_editor_test_folder_array());
    $files_array = _behat_editor_build_array_of_available_files($modules);
    $behat_table =_behat_editor_produce_table_array($files_array);
    $build['behat_table'] = array(
        '#markup' => $behat_table
    );

    $title =  "Confirm Delete";
    $body = "The file <span class='filename'>...</span> will be deleted.";
    $buttons = array(
        'save' => array('title' => "Confirm", 'class' => array('btn', 'btn-danger', 'confirm-delete')),
        'delete' => array('title' => 'Cancel', 'class' => array('btn', 'btn-success'), 'data' => array('dismiss' => "modal"))
    );
    $build['message']['#markup'] = behat_editor_modal($title, $body, $buttons);

    return $build;
}