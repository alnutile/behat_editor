<?php

/**
 * @todo this is running and saving it should only run
 */
function behat_editor_save($module, $filename) {
    $response = '';
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'helpers/behat_editor_file_parsing');
    if(isset($_REQUEST['scenario'])) {
        $response = _behat_editor_save_html_to_file($_REQUEST, $module, $filename, 'file');
        if($response['file'] != FALSE){
            $file_object = _behat_get_file_info($module, $filename);
            watchdog('test_file_info', print_r($file_object, 1));
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