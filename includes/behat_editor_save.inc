<?php
use Drupal\BehatEditor\File;

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
    watchdog('test_module', print_r($module, 1));
    watchdog('test_filename', print_r($filename, 1));
    watchdog('test_scenario', print_r($scenario, 1));
    watchdog('test_settings', print_r($settings, 1));

    $file = new Drupal\BehatEditor\FileController();
    $action = 'update';
    $params = array(
        'service_path' => $settings['path'],
        'module' => $settings['module'],
        'filename' => $settings['filename'],
        'action' => $action,
        'parse_type' => 'file',
    );
    $response = $file->save($params);

    if($response['file'] != FALSE){
        $results = array('file' => $response, 'test' => '', 'error' => 0, 'message' => $file_object);
        return $results;
    } else {
        return array('file' => array('message' => $response), 'error' => 1);
    }
}