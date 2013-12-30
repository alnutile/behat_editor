<?php

/**
 * @file functions to show the Admin / Index page
 */
use Drupal\BehatEditor;

/**
 * @todo wow this is out of date. Should be pulling from the Files class
 */

function behat_editor_index_api() {

    $modules = new BehatEditor\FileController();
    //Was getFilesArray for Files
    $files_array = $modules->index();

    //Allow users to hook the list of files.
    drupal_alter('behat_editor_index', $files_array);

    drupal_json_output($files_array);
    exit();
}