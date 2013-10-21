<?php
use Drupal\BehatEditor\File;

/**
 * @file File for viewing the Tests
 * You can see the related js file as well in the js folder
 */


function behat_editor_view_service($module, $filename) {
    composer_manager_register_autoloader();
    $file = new File(array(), $module, 'test.feature', FALSE);
    $file_info = $file->get_file_info();
    drupal_json_output($file_info);
    exit();
}