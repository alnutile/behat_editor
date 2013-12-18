<?php

use Drupal\BehatEditor\File;
/**
 * @file functions need to make the Add interface
 *
 * @todo hook this up to services and add a button
 */

/**
 * Passing parameters to delete seems impossible so need to work off of URL
 *
 * @param $module
 * @param $filename
 * @param array $settings
 * @return array
 */
function behat_editor_delete_api($action, $module, $filename, $settings = array()) {
    composer_manager_register_autoloader();
    //@todo could not think of a better way to get delete to work
    if(!isset($_SESSION['delete_path'])) {
        throw new \RuntimeException(t('Delete path not se in sessions'));
    }
    //@todo make a controller class to help here and talk to the FileController/Model
    $file = new Drupal\BehatEditor\FileController();
    //  since i could not pass params
    $path = $_SESSION['delete_path'];
    watchdog('test_path_after_before_slice', print_r($path, 1));
    $path = array_slice($path, 3);
    watchdog('test_path_after_slice', print_r($path, 1));

    $path_pop = $path;
    $filename = array_pop($path_pop);
    $module = $path[0];
    $params = array(
        'service_path' => $path,
        'module' => $module,
        'filename' => $filename,
        'action' => 'delete',
        'parse_type' => 'file',
    );

    $file_object = $file->buildObject($params);
    watchdog('test_fo', print_r($file_object, 1));

    $response = $file->delete_file();
    if($response != FALSE){
        $results = array('file' => $filename, 'test' => '', 'error' => 0, 'message' => 'File Deleted');
        drupal_set_message("File deleted");
        return $results;
    } else {
        return array('file' => array('message' => 'File deletion failed'), 'error' => 1);
    }
}