<?php

/**
 * @todo this is running and saving it should only run
 */
function behat_editor_run($module, $filename) {
    $response = '';
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
            $response = _gherkin_generator_save_html_to_file($_REQUEST);
            if($response['file'] != FALSE){
                $run_test_response = _gherkin_generator_run_test($response['file'], $_REQUEST['filename']);
                //@todo respond to error!
                $results = array('file' => $response, 'test' => $run_test_response);
                drupal_json_output($results);
                exit();
            } else {
                //@todo better error output here
                drupal_json_output(array());
                exit();
            }
        } else {
            //Send back the Error
            watchdog('gherkin_generator', "Bad Request", $variables = array(), $severity = WATCHDOG_ERROR, $link = NULL);
            $response = array('message' => t('Bad Request'), 'file' => FALSE);
            drupal_json_output($response);
            exit();
        }
    }
}

function _behat_editor_run_test($file_path, $name) {
    if (!is_file($file_path)) {
        //@todo does this stop the process and feedback the error system if
        //  file does not exist?
        watchdog('gherkin_generator', "Error file does not exist", $variables = array(), $severity = WATCHDOG_ERROR, $link = $file_path);
        return array('message' => "Error file does not exist", 'file' => $file_path);
    } else {
        if(user_access('save gherkin test')) {
            $output = _behat_editor_run_tests_from_module_folder($file_path, $name);
            return $output;
        } else {
            $output = _gherkin_generator_run_tests_from_tmp_folder($file_path, $name);
            return $output;
        }
    }
}


/**
 * @todo maybe all tests should results should go into public://files/
 */
function _behat_editor_run_tests_from_module_folder($file_path, $name) {
    $path = drupal_get_path('module', 'behat_editor');
    $absolute_file_path = $file_path;

    $path_results = file_build_uri("/behat_tests/results");
    if (!file_prepare_directory($path_results, FILE_CREATE_DIRECTORY)) {
        drupal_mkdir($path_results);
    }

    $test_id = $name;
    $results_uri = $path_results . '/' . $name . '.html';                   //Need to show the file in iframe later
    $results_url = file_create_url($results_uri);                           //
    $output = drupal_realpath($path_results) . '/' . $test_id . '.html';    //Needed to run bin

    $response = exec("cd $path/behat && bin/behat --format=html --out $output $absolute_file_path && echo $?");
    $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);

    /**
     * @todo DRY UP RESPONSE WORK
     */
    if($response == 0) {
        watchdog('gherkin_generator', "%date Test successful results at %name", $variables = array('%date' => $date, '%name' => $output), $severity = WATCHDOG_NOTICE, $link = $file_path);
        $output = array('message' => t('@date: <br> Test successful!', array('@date' => $date)), 'file' => $results_url, 'error' => FALSE);
    } else {
        watchdog('gherkin_generator', "%date Error Running Test %name", $variables = array('%date' => $date, '%name' => $file_path), $severity = WATCHDOG_ERROR, $link = $file_path);
        $output = array('message' => t('@date: <br> Error running test !name to download ', array('@date' => $date, '@name' => $file_path)), 'file' => $file, 'error' => TRUE);
    }

    return $output;
}

function _gherkin_generator_run_tests_from_tmp_folder($file_path, $name) {
    watchdog('test_tmp_name', print_r($name, 1));
    watchdog('real_path', drupal_realpath($file_path));
    $absolute_file_path = drupal_realpath($file_path);
    $path = drupal_get_path('module', 'gherkin_generator');
    $path_results = file_build_uri("/gherkin_tests/results");
    if (!file_prepare_directory($path_results, FILE_CREATE_DIRECTORY)) {
        drupal_mkdir($path_results);
    }

    $test_id = $name;
    $results_uri = $path_results . '/' . $name . '.html';                   //Need to show the file in iframe later
    $results_url = file_create_url($results_uri);                           //
    $output = drupal_realpath($path_results) . '/' . $test_id . '.html';    //Needed to run bin

    $response = exec("cd $path/behat && bin/behat --format=html --out $output $absolute_file_path && echo $?");
    $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);

    if($response == 0) {
        watchdog('gherkin_generator', "%date Test successful results at %name", $variables = array('%date' => $date, '%name' => $output), $severity = WATCHDOG_NOTICE, $link = $file_path);
        $output = array('message' => t('@date: <br> Test successful!', array('@date' => $date)), 'file' => $results_url, 'error' => FALSE);
    } else {
        watchdog('gherkin_generator', "%date Error Running Test %name", $variables = array('%date' => $date, '%name' => $file_path), $severity = WATCHDOG_ERROR, $link = $file_path);
        $output = array('message' => t('@date: <br> Error running test !name to download ', array('@date' => $date, '@name' => $file_path)), 'file' => $results_url, 'error' => TRUE);
    }

    return $output;
}
