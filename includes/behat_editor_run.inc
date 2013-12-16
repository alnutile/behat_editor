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
    $file = new Drupal\BehatEditor\FileBuilder();
    $path = explode('/', $settings['path']);
    unset($path[0]); //comes in w/ empty item
    $params = array(
        'service_path' => $path,
        'module' => $settings['module'],
        'filename' => $settings['filename'],
        'action' => 'run'
    );
    watchdog('test_params_coming_in', print_r($params, 1));

    $file_object = $file->buildObject($params);
    watchdog('test_fileobject_built', print_r($file_object, 1));
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
function behat_editor_create_and_run_api($action, $module, $filename, $scenario, $settings = array()) {
    composer_manager_register_autoloader();
    $filename = "$filename.feature";
    //Make sure it is a good request
    $file = new File(array('scenario' => $scenario), $module, $filename, 'file');
    $response = $file->save_html_to_file();
    //Make sure the file was made
    if($response['file'] != FALSE){
        $file_object = $file->get_file_info();
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



