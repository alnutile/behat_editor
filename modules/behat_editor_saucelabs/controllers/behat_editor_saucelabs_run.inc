<?php

function behat_editor_saucelabs_run($module, $filename) {
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'helpers/behat_editor_file_parsing');
    $file_object = _behat_get_file_info($module, $filename);
    $run_test_response = _behat_editor_saucelabs_run_test($file_object);
    $filename = $file_object['filename'];
    $date = format_date(time(), $type = 'medium', $format = '',
        $timezone = NULL, $langcode = NULL);
    $file_message = array(
        'file' => $filename,
        'error' => 0,
        'message' => "$date <br> File $filename tested."
    );
    $results = array('file' => $file_message, 'test' => $run_test_response);
    drupal_json_output($results);
    exit();
}


function _behat_editor_saucelabs_run_test($file_object) {
    module_load_include('inc', 'behat_editor', 'controllers/behat_editor_run');
    $file_full_path = $file_object['absolute_path_with_file'];
    if (!is_file($file_full_path)) {
        //@todo does this stop the process and feedback the error system if
        watchdog('behat_editor_saucelabs', "Error file does not exist", $variables = array(), $severity = WATCHDOG_ERROR, $link = $file_full_path);
        return array('message' => "Error file does not exist", 'file' => $file_full_path);
    } else {
        $absolute_file_path =  $file_object['absolute_path_with_file'];
        $file_name = $file_object['filename'];
        $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
        if(user_access('behat add test') && $file_object['module'] != variable_get('behat_editor_default_storage_folder', BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER)) {
            $output_file = _behat_editor_run_tests_from_module_folder($file_object);
            $response = _behat_editor_saucelabse_exec_test($output_file, $absolute_file_path);
        } else {
            $output_file = _behat_editor_run_tests_from_tmp_folder($file_object);
            $response = _behat_editor_saucelabse_exec_test($output_file, $absolute_file_path);
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

function _behat_editor_saucelabse_exec_test($output_file, $absolute_file_path) {
    $path = drupal_get_path('module', 'behat_editor');
    $response = exec("cd $path/behat && ./bin/behat --no-paths --out $output_file  --profile=Selenium-saucelabs2 $absolute_file_path && echo $?");
    return $response;
}