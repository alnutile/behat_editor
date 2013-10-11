<?php
use Drupal\BehatEditor\File;

/**
 * @file functions needed to save the file
 * You can see the related js file as well in the js folder
 */

/**
 * Save the file to the module folder or tmp folder
 *
 * @param module
 *   this should be the machine_name
 * @param filename
 *   this should be the filename.feature extension
 */
function behat_editor_save($module, $filename) {
    composer_manager_register_autoloader();
    $response = '';
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    if(isset($_REQUEST['scenario'])) {
        $file = new File($_REQUEST, $module, $filename, 'file');
        $response = $file->save_html_to_file();
        if($response['file'] != FALSE){
            $file_object = $file->get_file_info();
            $results = array('file' => $response, 'test' => '', 'error' => 0, 'message' => $file_object);
            drupal_json_output($results);
            exit();
        } else {
            drupal_json_output(array('file' => array('message' => $response), 'error' => 1));
            exit();
        }
    } else {
        //Send back the Error
        watchdog('behat_editor', "Bad Request: Scenario could not be parsed", $variables = array(), $severity = WATCHDOG_ERROR, $link = NULL);
        $response = array('message' => t('Bad Request: Scenario could not be parsed'), 'file' => FALSE, 'error' => 1);
        drupal_json_output($response);
        exit();
    }
}

function behat_editor_update_api($module, $filename, $query) {
    watchdog('test_module', print_r($module, 1));
    watchdog('test_file', print_r($filename, 1));
    watchdog('test_api_query', print_r($query, 1));
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