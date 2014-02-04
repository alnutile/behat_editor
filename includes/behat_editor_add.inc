<?php

use Drupal\BehatEditor;
/**
 * @file functions need to make the Add interface
 */


function behat_editor_create_api($query) {
    composer_manager_register_autoloader();

    if(isset($query['clone'])) {
        $clone = $query['clone'];
    } else {
        $clone = FALSE;
    }
    $action = 'add';
    $params = array(
        'service_path' => $query['path'],
        'module' => $query['module'],
        'filename' => $query['filename'],
        'action' => $action,
        'parse_type' => 'file',
        'scenario' => $query['scenario'],
        'clone' => $clone
    );
    $file = new Drupal\BehatEditor\FileController();
    $response = $file->create($params);


    if($response['file'] != FALSE){
        if($response['file'] != FALSE){
            drupal_alter('behat_editor_after_create', $response['data']);
            $results = array('file' => $response, 'test' => '', 'error' => 0, 'message' => $response['message']);
            drupal_set_message("Your file {$query['filename']} has been saved. You are now in edit mode");
            return $results;
        } else {
            return array('file' => array('message' => $response), 'error' => 1);
        }
    } else {
        return array('file' => array('message' => $response), 'error' => 1);
    }
}
