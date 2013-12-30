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
    $file = new Drupal\BehatEditor\FileController();
    $params = array(
        'service_path' => $settings['path'],
        'module' => $settings['module'],
        'filename' => $settings['filename'],
        'action' => $action
    );

    $file_object = $file->show($params);

    if(isset($settings['context'])) { $context = $settings['context']; } else { $context = 'behat_run'; }
    $res = new BehatEditorRun($file_object);
    $run_test_response = $res->exec(FALSE, $settings, $context);

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
    $file = new Drupal\BehatEditor\FileController();
    $action = 'save';
    if(isset($settings['action'])) {
        $action = $settings['action'];
    }

    $params = array(
        'service_path' => $settings['path'],
        'module' => $settings['module'],
        'filename' => $settings['filename'],
        'action' =>  $action,
        'scenario' => $scenario,
        'parse_type' => 'file',
    );

    $response = $file->$action($params);

    //Make sure the file was made
    if($response['file'] != FALSE){
        //@todo return full object on save
        $res = new BehatEditorRun($file->show($params));
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




