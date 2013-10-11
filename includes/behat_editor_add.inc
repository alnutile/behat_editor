<?php

use Drupal\BehatEditor\File;
/**
 * @file functions need to make the Add interface
 */


function behat_editor_create_api($query) {
    composer_manager_register_autoloader();
    $filename = $query['filename'] . ".feature";
    $module = $query['module'];
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
