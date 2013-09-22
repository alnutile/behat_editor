<?php

/**
 * @todo this is running and saving it should only run
 */
function behat_editor_run($module, $filename) {
    $response = '';
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'helpers/behat_editor_file_parsing');
    if($_REQUEST['method'] == 'view-mode') {
        module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
        module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
        $file_info = _behat_get_file_info($module, $filename);
        $run_test_response = _behat_editor_run_test($file_info['absolute_path_with_file'], $filename);
        $results = array('file' => $file_info['absolute_path_with_file'], 'test' => $run_test_response);
        drupal_json_output($results);
        exit();
    }

    /**
     * Run both module and file folder tests from the aboslute url
     * so this can be consolidated
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
                //@todo better error output here
                //  the response needs to be made to work as the format above
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
        if(user_access('save gherkin test') && $file_object['module'] != variable_get('behat_editor_default_storage_folder', BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER)) {
            $output = _behat_editor_run_tests_from_module_folder($file_object);
            return $output;
        } else {
            $output = _behat_editor_run_tests_from_tmp_folder($file_object);
            return $output;
        }
    }
}


/**
 * @todo maybe all tests should results should go into public://files/
 */
function _behat_editor_run_tests_from_module_folder($file_object) {

    $file_tmp_folder = variable_get('behat_editor_default_folder', BEHAT_EDITOR_DEFAULT_FOLDER);
    $path_results = file_build_uri("/{$file_tmp_folder}/results");
    if (!file_prepare_directory($path_results, FILE_CREATE_DIRECTORY)) {
        drupal_mkdir($path_results);
    }

    //$file_full_path = $file_object['absolute_path'];
    $absolute_file_path = $path_results;
    $path = drupal_get_path('module', 'behat_editor');

    $name = $file_object['filename_no_ext'];
    $file_name = $file_object['filename'];
    $file_path = $file_object['absolute_path_with_file'];

    $test_id = $name;
    $results_uri = $path_results . '/' . $name . '.html';                   //Need to show the file in iframe later
    $results_url = file_create_url($results_uri);                           //
    $output = drupal_realpath($path_results) . '/' . $test_id . '.html';    //Needed to run bin

    $response = exec("cd $path/behat && bin/behat --format=html --tags '~@javascript' --out $output $absolute_file_path && echo $?");
    $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);

    /**
     * @todo DRY UP RESPONSE WORK
     */
    if($response == 0) {
        watchdog('gherkin_generator', "%date Test successful results at %name", $variables = array('%date' => $date, '%name' => $output), $severity = WATCHDOG_NOTICE, $link = $file_path);
        $output = array('message' => t('@date: <br> Test successful!', array('@date' => $date)), 'file' => $results_url, 'error' => FALSE);
    } else {
        watchdog('gherkin_generator', "%date Error Running Test %name", $variables = array('%date' => $date, '%name' => $file_name), $severity = WATCHDOG_ERROR, $link = $file_path);
        $output = array('message' => t('@date: <br> Error running test !name to download ', array('@date' => $date, '@name' => $file_path)), 'file' => $file_path, 'error' => TRUE);
    }

    return $output;
}

function _behat_editor_run_tests_from_tmp_folder($file_object) {
    $absolute_file_path = $file_object['absolute_path_with_file'];
    $path = drupal_get_path('module', 'behat_editor');

    $file_tmp_folder = variable_get('behat_editor_default_storage_folder', BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER);
    $path_results = file_build_uri("/{$file_tmp_folder}/results");
    if (!file_prepare_directory($path_results, FILE_CREATE_DIRECTORY)) {
        drupal_mkdir($path_results);
    }

    $name = $file_object['filename_no_ext'];
    $test_id = $name;
    $results_uri = $path_results . '/' . $name . '.html';                   //Need to show the file in iframe later
    $results_url = file_create_url($results_uri);
    $linkable_path = $results_url;
    $output = drupal_realpath($path_results) . '/' . $test_id . '.html';    //Needed to run bin
    $response = exec("cd $path/behat && bin/behat --format=html --tags '~@javascript' --out $output $absolute_file_path && echo $?");
    $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);

    if($response == 0) {
        watchdog('gherkin_generator', "%date Test successful results at %name", $variables = array('%date' => $date, '%name' => $output), $severity = WATCHDOG_NOTICE, $link = $linkable_path);
        $output = array('message' => t('@date: <br> Test successful!', array('@date' => $date)), 'file' => $results_url, 'error' => FALSE);
    } else {
        watchdog('gherkin_generator', "%date Error Running Test %name", $variables = array('%date' => $date, '%name' => $linkable_path), $severity = WATCHDOG_ERROR, $link = $linkable_path);
        $output = array('message' => t('@date: <br> Error running test !name to download ', array('@date' => $date, '@name' => $linkable_path)), 'file' => $results_url, 'error' => TRUE);
    }

    return $output;
}
