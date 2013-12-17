<?php
use Drupal\BehatEditor\File;

/**
 * @file functions needed to save the file
 * You can see the related js file as well in the js folder
 */

function behat_editor_update_api($module, $filename, $scenario, $settings = array()) {
    composer_manager_register_autoloader();
    watchdog('test_save_settings', print_r($settings, 1));
    watchdog('test_save_scenarios', print_r($scenario, 1));
    watchdog('test_save_filename', print_r($filename, 1));
    $file = new Drupal\BehatEditor\FileBuilder();
    $path = explode('/', $settings['path']);
    $action = 'update';
    unset($path[0]); //comes in w/ empty item
    $params = array(
        'service_path' => $path,
        'module' => $settings['module'],
        'filename' => $settings['filename'],
        'action' => $action,
        'parse_type' => 'file',
    );
    $file_object = $file->buildObject($params);
    $response = $file->save_html_to_file($scenario);

    //$filename = $filename . ".feature";
    //module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    //$file = new File($query, $module, $filename, 'file');
    //$response = $file->save_html_to_file();

    if($response['file'] != FALSE){
        //$file_object = $file->get_file_info();
        $results = array('file' => $response, 'test' => '', 'error' => 0, 'message' => $file_object);
        return $results;
    } else {
        return array('file' => array('message' => $response), 'error' => 1);
    }
}