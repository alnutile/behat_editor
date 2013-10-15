<?php
use Drupal\BehatEditor\Results;

/**
 * @file functions needed to save the file
 * You can see the related js file as well in the js folder
 */

function behat_editor_results_api($action, $module, $filename) {
    composer_manager_register_autoloader();
    $filename = $filename . '.feature';
    $response = Results::getResultsForFile($module, $filename);
    if($response['error'] === 0){
        //@todo shows that the message object is to specific to files should just pass message and error 1/0
        return array('error' => $response['error'], 'data' => $response['results']);
    } else {
        watchdog('behat_editor', t('Trouble getting results for @filename', array('@filename' => $filename)));
        return array('message' => t('Error getting results'), 'error' => 1);
    }
}