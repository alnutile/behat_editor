<?php
use Drupal\BehatEditor,
    Drupal\BehatEditor\BehatEditorRun;

/**
 * @file functions need to run a test
 */

/**
 * Services Targeted Action
 * This will run the tests
 * no saving of file needed.
 *
 * @param $action
 * @param $module
 * @param $filename
 * @return array
 */
function behat_editor_run_api($action, $settings) {
    composer_manager_register_autoloader();
    //@todo make a better interface for this class
    $file_object = _setFileObject($settings, 'run');
    $res = new BehatEditorRun($file_object);
    $run_test_response = $res->exec(FALSE, $settings);

    if($run_test_response['rid']) {
        $results = $res->generateReturnPassOutput();
        return $results;
    } else {
        $results = $res->generateReturnFailOutput();
        return $results;
    }
}

/**
 * Services Targeted Action
 * this will save the updates to the file
 * then run the tests
 *
 * @param $action
 * @param $module
 * @param $filename
 * @param $query
 * @return array
 */
function behat_editor_create_and_run_api($action, $scenario, $settings = array()) {
    composer_manager_register_autoloader();

    //@todo make a better interface for this class
    //@todo tighten this up so it can be shared with above
    $file = new Drupal\BehatEditor\FileBuilder();
    $path = explode('/', $settings['path']);
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
    //Make sure the file was made
    if($response['file'] != FALSE){
        $res = new BehatEditorRun($file_object);
        $run_test_response = $res->exec(FALSE, $settings);
        $response = $run_test_response['response'];

        if($run_test_response['rid']) {
            $results = $res->generateReturnPassOutput();
            return $results;
        } else {
            $results = $res->generateReturnFailOutput();
            return $results;
        }
    } else {
        return array('message' => $response['message'], 'file' => array('message' => $response), 'error' => 1);
    }
}

function _setFileObject($settings, $action){
    composer_manager_register_autoloader();

    $file = new Drupal\BehatEditor\FileBuilder();
    $path = explode('/', $settings['path']);
    unset($path[0]); //comes in w/ empty item
    $params = array(
        'service_path' => $path,
        'module' => $settings['module'],
        'filename' => $settings['filename'],
        'action' => $action
    );

    $file_object = $file->buildObject($params);
    return $file_object;
}



