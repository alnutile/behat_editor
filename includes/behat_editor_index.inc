<?php

/**
 * @file functions to show the Admin / Index page
 */
use Drupal\BehatEditor;

/**
 * @todo wow this is out of date. Should be pulling from the Files class
 */

function behat_editor_index_api() {
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');

    //@todo convert to Files class
    $modules = _behat_editor_check_for_modules();
    $modules = array_merge($modules, _behat_editor_test_folder_array());
    $files_array = _behat_editor_build_array_of_available_files($modules);

    drupal_json_output($files_array);
    exit();
}