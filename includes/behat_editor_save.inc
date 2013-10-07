<?php
use Drupal\BehatEditor;

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
        $file = new Drupal\BehatEditor\File($_REQUEST, $module, $filename, 'file');
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