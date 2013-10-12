<?php
use Drupal\BehatEditor\File;

/**
 * @file functions needed to save the file
 * You can see the related js file as well in the js folder
 */

function behat_editor_update_api($module, $filename, $query) {
    composer_manager_register_autoloader();
    $filename = $filename . ".feature";
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    $file = new File($query, $module, $filename, 'file');
    $response = $file->save_html_to_file();
    if($response['file'] != FALSE){
        $file_object = $file->get_file_info();
        $results = array('file' => $response, 'test' => '', 'error' => 0, 'message' => $file_object);
        return $results;
    } else {
        return array('file' => array('message' => $response), 'error' => 1);
    }
}