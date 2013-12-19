<?php
use Drupal\BehatEditor;

/**
 * @todo
 * clean up the service
 * it no longer works relative to the filename since it is a longer and dynamic url
 *
 * @file functions needed to save the file
 * You can see the related js file as well in the js folder
 */

function behat_editor_update_api($module, $filename, $scenario, $settings = array()) {
    composer_manager_register_autoloader();
    $file = new Drupal\BehatEditor\FileController();
    $action = 'update';
    $params = array(
        'service_path' => $settings['path'],
        'module' => $settings['module'],
        'filename' => $settings['filename'],
        'action' => $action,
        'parse_type' => 'file',
        'scenario' => $scenario
    );
    $response = $file->save($params);
    watchdog('test_save', print_r($response, 1));
    if($response['file'] != FALSE){
        $results = array('file' => $response, 'test' => '', 'error' => 0, 'message' => $response['message']);
        return $results;
    } else {
        return array('file' => array('message' => $response), 'error' => 1);
    }
}