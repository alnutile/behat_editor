<?php
use BehatEditor\SauceLabs\SauceLabsBehatEditor;
use BehatEditor\SauceLabs\BehatEditorSauceLabsRun;

function behat_editor_saucelabs_run($module, $filename) {
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'helpers/behat_editor_file_parsing');
    module_load_include('inc', 'behat_editor', 'includes/behat_editor_run');

    composer_manager_register_autoloader();
    //@todo pull this in with above ann ot user module_load_include
    module_load_include('inc', 'behat_editor_saucelabs', 'libraries/BehatEditorSauceLabsRun');

    if (!class_exists('BehatEditor\SauceLabs\BehatEditorSauceLabsRun')) {
        $message = t('BehatEditorSauceLabsRun not found.');
        drupal_json_output($results = array('error' => 1, 'message' => $message));
        exit();
    }

    $file_object = _behat_get_file_info($module, $filename);
    $res = new BehatEditorSauceLabsRun($file_object);

    $run_test_response = $res->exec();

    $filename = $file_object['filename'];
    $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
    $file_message = array(
        'file' => $filename,
        'error' => 0,
        'message' => "$date <br> File $filename tested."
    );
    $response = $run_test_response['response'];
    $absolute_file_path =  $file_object['absolute_path_with_file'];
    $output_file = $run_test_response['output_file'];
    if($response == 0) {
        $results = _behat_editor_read_file($output_file);
        $output_item_list = _behat_editor_output_html_item_list($results);
        watchdog('behat_editor', "%date Test successful results at %name", $variables = array('%date' => $date, '%name' => $output_file), $severity = WATCHDOG_NOTICE, $link = $absolute_file_path);
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

