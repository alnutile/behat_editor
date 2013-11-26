<?php
use Drupal\BehatEditor\File,
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
function behat_editor_run_api($action, $module, $filename, $settings) {
    composer_manager_register_autoloader();
    $filename = "$filename.feature";
    //@todo include these in autoloader
    //  or remove them if they are no longer needed
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');

    $file = new File(array(), $module, $filename, FALSE);
    $file_object = $file->get_file_info();
    $res = new BehatEditorRun($file_object);
    $run_test_response = $res->exec(FALSE, $settings);
    $response = $run_test_response['response'];
    //Check the Response from Running the Tests
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



