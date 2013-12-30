<?php

/**
 * @file functions to show the Admin / Index page
 */
use Drupal\BehatEditor;

/**
 * http://behateditor.localvm.com/behat_editor_v1/behat_editor_actions
 */

function behat_editor_index_api() {

    $modules = new BehatEditor\FileController();
    //Was getFilesArray for Files
    $files_array = $modules->index();
    watchdog('test_index_js', print_r($files_array, 1));
    drupal_json_output($files_array);
    exit();
}