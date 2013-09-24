<?php

/**
 * @todo this is running and saving it should only run
 */
function behat_editor_run($module, $filename) {
    $response = '';
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'helpers/behat_editor_file_parsing');
    if($_REQUEST['method'] == 'view-mode') {
        //Run with no alterations to the test file
        module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
        module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
        $file_object = _behat_get_file_info($module, $filename);
        $run_test_response = _behat_editor_run_test($file_object);
        $date = format_date(time(), $type = 'medium', $format = '',
                $timezone = NULL, $langcode = NULL);
        $filename = $file_object['filename'];
        $file_message = array(
            'file' => $filename,
            'error' => 0,
            'message' => "$date <br> File $filename tested."
        );
        $results = array('file' => $file_message, 'test' => $run_test_response);
        drupal_json_output($results);
        exit();
    }

    /**
     * This means the file needs to be created or
     * updated before being Run
     */
    if($_REQUEST['method'] == 'create-mode') {
        if(isset($_REQUEST['scenario'])) {
            $response = _behat_editor_save_html_to_file($_REQUEST, $module, $filename, 'file');
            if($response['file'] != FALSE){
                $file_object = _behat_get_file_info($module, $filename);
                $run_test_response = _behat_editor_run_test($file_object);
                //@todo respond to error!
                $results = array('file' => $response, 'test' => $run_test_response);
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
}

function _behat_editor_run_test($file_object) {
    $file_full_path = $file_object['absolute_path_with_file'];
    if (!is_file($file_full_path)) {
        //@todo does this stop the process and feedback the error system if
        //  file does not exist?
        watchdog('behat_editor', "Error file does not exist", $variables = array(), $severity = WATCHDOG_ERROR, $link = $file_full_path);
        return array('message' => "Error file does not exist", 'file' => $file_full_path);
    } else {
        if(user_access('behat add test') && $file_object['module'] != variable_get('behat_editor_default_storage_folder', BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER)) {
            $output_file = _behat_editor_run_tests_from_module_folder($file_object);
            $absolute_file_path =  $file_object['absolute_path_with_file'];
            $file_name = $file_object['filename'];
            $response = _behat_editor_exec_test($output_file, $absolute_file_path);
            $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
        } else {
            $output_file = _behat_editor_run_tests_from_tmp_folder($file_object);
            $absolute_file_path =  $file_object['absolute_path_with_file'];
            $file_name = $file_object['filename'];
            $response = _behat_editor_exec_test($output_file, $absolute_file_path);
            $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
        }
        if($response == 0) {
            //HTML and iFrame of the test led to cache issues in chrome so going with Text
            $results = _behat_editor_read_file($output_file);
            $output_item_list = _behat_editor_output_html_item_list($results);
            watchdog('behat_editor', "%date Test successful results at %name", $variables = array('%date' => $date, '%name' => $output_file), $severity = WATCHDOG_NOTICE, $link = $absolute_file_path);
            $output = array('message' => t('@date: <br> Test successful!', array('@date' => $date)), 'file' => $output_item_list, 'error' => FALSE);
            return $output;
        } else {
            watchdog('behat_editor', "%date Error Running Test %name", $variables = array('%date' => $date, '%name' => $output_file), $severity = WATCHDOG_ERROR, $link = $absolute_file_path);
            $output = array('message' => t('@date: <br> Error running test !name to download ', array('@date' => $date, '@name' => $file_name)), 'file' => $file_name, 'error' => TRUE);
            return $output;
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


function _behat_editor_exec_test($output, $absolute_file_path){
    $path = drupal_get_path('module', 'behat_editor');
    $response = exec("cd $path/behat && bin/behat --no-paths --tags '~@javascript' --out $output $absolute_file_path && echo $?");
    return $response;
}

function _behat_editor_output_html_item_list($results){
    $output_file_to_array = _behat_editor_turn_file_to_array($results);
    $output_item_list = theme('item_list', $var = array('title' => 'Test Results', 'items' => $output_file_to_array));
    return $output_item_list;
}

