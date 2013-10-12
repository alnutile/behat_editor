<?php

use Drupal\BehatEditor\File;
/**
 * @file functions need to make the Add interface
 *
 * @todo hook this up to services and add a button
 */


function behat_editor_delete_api($module, $filename) {
    composer_manager_register_autoloader();
    $filename = $filename . ".feature";
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    $file = new File(array(), $module, $filename, 'file');
    $response = $file->delete_file();
    if($response != FALSE){
        $results = array('file' => $filename, 'test' => '', 'error' => 0, 'message' => 'File Deleted');
        return $results;
    } else {
        return array('file' => array('message' => 'File deletion failed'), 'error' => 1);
    }
}