<?php

/**
 * @file functions to show the Admin / Index page
 */

use Drupal\BehatEditor;

/**
 * Build the output for the Index page of files.
 * You can see the related js file as well in the js folder
 *
 * @todo turn this into a form so it will be easier to hook and do
 * other bulk operations
 * @return mixed
 */

function behat_editor_index() {
    composer_manager_register_autoloader();
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
    libraries_load('behat_editor_data_table');
    $path = drupal_get_path('module', 'behat_editor');
    drupal_add_js($path . '/js/behat_editor_app.js');
    drupal_add_js($path . '/js/behat_editor_index.js');
    drupal_add_css($path . '/css/behat_editor_app.css');

    behat_editor_add_bootstrap();


    $build['messages_top'] = array(
        '#markup' => "<div id='messages-behat'></div>",
        '#weight' => -100
    );

    $build['actions'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('col-lg-12', 'well')),
        '#weight' => -100
    );

    $build['actions']['add_test'] = array(
        '#type' => 'link',
        '#title' => t('Add Test'),
        '#href' => "admin/behat/add",
        '#attributes' => array('role' => 'button', 'class' => array('btn', 'btn-success', 'btn-md', 'add')),
    );

    $build['actions']['manage_tags'] = array(
        '#type' => 'link',
        '#title' => t('Manage Tags'),
        '#href' => "admin/config/behat_editor/allowed_tags",
        '#attributes' => array('role' => 'button', 'class' => array('btn', 'btn-default', 'btn-md',  'manage_tags')),
    );

    $build['file_form_fieldset'] = drupal_get_form('behat_editor_file_uploader');

    //@todo convert to using the json index output
    //  see ticket #85 on github

    $modules = new BehatEditor\FileController();
    //Was getFilesArray for Files
    $files_array = $modules->index();
    //Allow users to hook the list of files.
    drupal_alter('behat_editor_index', $files_array);

    $build['intro'] = array(
        '#prefix' => "<div class='alert alert-warning'><i class='glyphicon glyphicon-question-sign'></i>",
        '#suffix' => "</div>",
        '#markup' => t("Type Pass or Fail into search to filter by those states"),
        '#weight' => 9
    );

    $behat_table =_behat_editor_produce_table_array($files_array);
    $build['behat_table'] = array(
        '#markup' => $behat_table,
        '#weight' => 10,
    );

    $title =  "Confirm Delete";
    $body = "The file <span class='filename'>...</span> will be deleted.";
    $buttons = array(
        'save' => array('title' => "Confirm", 'class' => array('btn', 'btn-danger', 'confirm-delete')),
        'delete' => array('title' => 'Cancel', 'class' => array('btn', 'btn-success'), 'data' => array('dismiss' => "modal"))
    );
    $build['message']['#markup'] = behat_editor_modal($title, $body, $buttons);
    behat_editor_results_history_modal($build);
    return $build;
}

/**
 * Helper function to make the array of
 * available files in the folder
 *
 * @param $modules
 *  return value of _behat_editor_has_folder
 * @return array
 *  and array keyed by module names of files folders etc.
 *
 * @todo this gets repeated in several places need to bring it into a class
 *   to use as needed.
 */
function _behat_editor_build_array_of_available_files_v2($modules) {
    $files_found = array();
    foreach($modules as $machine_name => $nice_name) {
        if ($machine_name == BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER) {
            $sub_folder = BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER;
            $files_folder =  file_build_uri("/{$sub_folder}/");
            $path = drupal_realpath($files_folder);
            $files_found[$machine_name] = _behat_editor_scan_directories($machine_name, $path);
        } else {
            $path = DRUPAL_ROOT . '/' . drupal_get_path('module', $machine_name) . '/' . BEHAT_EDITOR_FOLDER;
            $files_found[$machine_name] =  _behat_editor_scan_directories($machine_name, $path);
        }
    }
    return $files_found;
}
