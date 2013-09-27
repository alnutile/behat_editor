<?php
use BehatEditor\Libraries\BehatEditorRun;
/**
 * @todo this is running and saving it should only run
 */
function behat_editor_run($module, $filename) {
    $response = '';
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'helpers/behat_editor_file_parsing');
    composer_manager_register_autoloader();
    //@todo pull this in with above ann ot user module_load_include
    module_load_include('inc', 'behat_editor', 'libraries/BehatEditorRun');

    if (!class_exists('BehatEditor\Libraries\BehatEditorRun')) {
        $message = t('BehatEditorRun not found.');

        $results = array('file' => array(), 'test' => array(), 'message' => $message, 'error' => 1);
        drupal_json_output($results);
        exit();
    }

    if($_REQUEST['method'] == 'view-mode') {
        //Run with no alterations to the test file
        module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
        module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
        $file_object = _behat_get_file_info($module, $filename);
        $res = new BehatEditorRun($file_object);
        $run_test_response = $res->exec();
        $response = $run_test_response['response'];
        $absolute_file_path =  $file_object['absolute_path_with_file'];
        $output_file = $run_test_response['output_file'];
        $date = format_date(time(), $type = 'medium', $format = '',
            $timezone = NULL, $langcode = NULL);
        $filename = $file_object['filename'];
        $file_message = array(
            'file' => $filename,
            'error' => 0,
            'message' => "$date <br> File $filename tested."
        );
        if($response == 0) {
            $results = _behat_editor_read_file($output_file);
            $output_item_list = _behat_editor_output_html_item_list($results);
            $output = array('message' => t('@date: <br> Test successful!', array('@date' => $date)), 'file' => $output_item_list, 'error' => FALSE);
            $results = array('file' => $file_message, 'test' => $output, 'error' => 0);
            drupal_json_output($results);
            exit();
        } else {
            watchdog('behat_editor', "%date Error Running Test %name", $variables = array('%date' => $date, '%name' => $output_file), $severity = WATCHDOG_ERROR, $link = $absolute_file_path);
            $output = array('message' => t('@date: <br> Error running test !name to download ', array('@date' => $date, '@name' => $filename)), 'file' => $filename, 'error' => TRUE);
            $results = array('file' => $file_message, 'test' => $output, 'error' => 1);
            drupal_json_output($results);
            exit();
        }

    }

    /**
     * This means the file needs to be created or
     * updated before being Run
     */
    if($_REQUEST['method'] == 'create-mode') {
        //Make sure it is a good request
        if(isset($_REQUEST['scenario'])) {
            $response = _behat_editor_save_html_to_file($_REQUEST, $module, $filename, 'file');
            //Make sure the file was made
            if($response['file'] != FALSE){
                $file_object = _behat_get_file_info($module, $filename);
                $res = new BehatEditorRun($file_object);
                $run_test_response = $res->exec();
                $response = $run_test_response['response'];
                $absolute_file_path =  $file_object['absolute_path_with_file'];
                $output_file = $run_test_response['output_file'];
                $date = format_date(time(), $type = 'medium', $format = '',
                    $timezone = NULL, $langcode = NULL);
                $filename = $file_object['filename'];
                $file_url = l($filename, $file_object['relative_path'], $options = array('attributes' => array('target' => '_blank')));
                $file_message = array(
                    'file' => $filename,
                    'error' => 0,
                    'message' => "$date <br> File $file_url tested."
                );
                //Check the Response from Running the Tests
                if($response == 0) {
                    $results = _behat_editor_read_file($output_file);
                    $output_item_list = _behat_editor_output_html_item_list($results);
                    $output = array('message' => t('@date: <br> Test successful!', array('@date' => $date)), 'file' => $output_item_list, 'error' => FALSE);
                    $results = array('file' => $file_message, 'test' => $output, 'error' => 0);
                    drupal_json_output($results);
                    exit();
                } else {
                    //Test Error
                    watchdog('behat_editor', "%date Error Running Test %name", $variables = array('%date' => $date, '%name' => $output_file), $severity = WATCHDOG_ERROR, $link = $absolute_file_path);
                    $output = array('message' => t('@date: <br> Error running test !name to download ', array('@date' => $date, '@name' => $filename)), 'file' => $filename, 'error' => TRUE);
                    $results = array('file' => $file_message, 'test' => $output, 'error' => 1);
                    drupal_json_output($results);
                    exit();
                }
            //Issue Creating File
            } else {
                drupal_json_output(array('file' => array('message' => $response), 'error' => 1));
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
 * @todo maybe all tests should results should go into public://files/
 */
function _behat_editor_run_tests_from_module_folder($file_object) {
    module_load_include('inc', 'behat_editor', 'behat_helpers_app');
    //Setup folder to store file with test results
    $file_tmp_folder = variable_get('behat_editor_default_folder', BEHAT_EDITOR_DEFAULT_FOLDER);
    $path_results = file_build_uri("/{$file_tmp_folder}/results");
    if (!file_prepare_directory($path_results, FILE_CREATE_DIRECTORY)) {
        drupal_mkdir($path_results);
    }
    $name = $file_object['filename_no_ext'];
    $test_id = $name;
    $output_file = drupal_realpath($path_results) . '/' . $test_id . '.txt';    //Needed to run bin
    return $output_file;
}

function _behat_editor_run_tests_from_tmp_folder($file_object) {
    module_load_include('inc', 'behat_editor', 'behat_helpers_app');
    $file_tmp_folder = variable_get('behat_editor_default_storage_folder', BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER);
    $path_results = file_build_uri("/{$file_tmp_folder}/results");
    if (!file_prepare_directory($path_results, FILE_CREATE_DIRECTORY)) {
        drupal_mkdir($path_results);
    }
    $name = $file_object['filename_no_ext'];
    $test_id = $name;
    $output_file = drupal_realpath($path_results) . '/' . $test_id . '.txt';    //Needed to run bin
    return $output_file;
}

function _behat_editor_output_html_item_list($results){
    $output_file_to_array = _behat_editor_turn_file_to_array($results);
    $output_item_list = theme('item_list', $var = array('title' => 'Test Results', 'items' => $output_file_to_array));
    return $output_item_list;
}

