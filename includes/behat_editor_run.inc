<?php
use Drupal\BehatEditor\File,
    Drupal\BehatEditor\BehatEditorRun;

/**
 * @file functions need to run a test
 */

/**
 * Takes the request and either runs the test
 * or saves the file and runs the tests.
 * You can see the related js file as well in the js folder
 *
 *
 * @param module
 *   this should be the machine_name
 * @param filename
 *   this should be the filename.feature extension
 *
 * @todo remove this function replaced by services functions
 */
function behat_editor_run($module, $filename) {
    composer_manager_register_autoloader();

    if($_REQUEST['method'] == 'view-mode') {
        //@todo include these in autoloader
        module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
        module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');

        $file = new File($_REQUEST, $module, $filename, FALSE);
        $file_object = $file->get_file_info();
        $res = new BehatEditorRun($file_object);
        $run_test_response = $res->exec();
        $response = $run_test_response['response'];

        //Check the Response from Running the Tests
        if($response == 0) {
            $results = $res->generateReturnPassOutput();
            drupal_json_output($results);
            exit();
        } else {
            $results = $res->generateReturnFailOutput();
            drupal_json_output($results);
            exit();
        }
    }

    /**
     * This means the file needs to be created or
     * updated before being Run
     */
    if($_REQUEST['method'] == 'create-mode') {
        //@todo include these in autoloader
        module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
        module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
        //Make sure it is a good request
        if(isset($_REQUEST['scenario'])) {
            $file = new File($_REQUEST, $module, $filename, 'file');
            $response = $file->save_html_to_file();
            //Make sure the file was made
            if($response['file'] != FALSE){
                $file_object = $file->get_file_info();
                $res = new BehatEditorRun($file_object);
                $run_test_response = $res->exec();
                $response = $run_test_response['response'];
                //Check the Response from Running the Tests
                if($response == 0) {
                    $results = $res->generateReturnPassOutput();
                    drupal_json_output($results);
                    exit();
                } else {
                    $results = $res->generateReturnFailOutput();
                    drupal_json_output($results);
                    exit();
                }
            //Issue Creating File
            } else {
                drupal_json_output(array('message' => $response['message'], 'file' => array('message' => $response), 'error' => 1));
                exit();
            }
        } else {
            //Request was did not have a Scenario
            watchdog('behat_editor', "Bad Request: Scenario could not be parsed", $variables = array(), $severity = WATCHDOG_ERROR, $link = NULL);
            $response = array('message' => t('Bad Request: Scenario could not be parsed'), 'file' => FALSE, 'error' => 1);
            drupal_json_output($response);
            exit();
        }
    }
}

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
function behat_editor_run_api($action, $module, $filename) {
    watchdog('test_run_as', print_r($action, 1));
    composer_manager_register_autoloader();
    $filename = "$filename.feature";
    //@todo include these in autoloader
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');

    $file = new File(array(), $module, $filename, FALSE);
    $file_object = $file->get_file_info();
    $res = new BehatEditorRun($file_object);
    $run_test_response = $res->exec();
    $response = $run_test_response['response'];

    //Check the Response from Running the Tests
    if($response == 0) {
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
function behat_editor_create_and_run_api($action, $module, $filename, $query) {
    composer_manager_register_autoloader();
    $filename = "$filename.feature";
    //@todo include these in autoloader
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
    //Make sure it is a good request
    $file = new File(array('scenario' => $query), $module, $filename, 'file');
    $response = $file->save_html_to_file();
    //Make sure the file was made
    if($response['file'] != FALSE){
        $file_object = $file->get_file_info();
        $res = new BehatEditorRun($file_object);
        $run_test_response = $res->exec();
        $response = $run_test_response['response'];
        //Check the Response from Running the Tests
        if($response == 0) {
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



