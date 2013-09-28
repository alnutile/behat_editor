<?php
use Drupal\BehatEditorSauceLabs;

function behat_editor_saucelabs_run($module, $filename) {
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'helpers/behat_editor_file_parsing');
    module_load_include('inc', 'behat_editor', 'includes/behat_editor_run');

    composer_manager_register_autoloader();

    if (!class_exists('Drupal\BehatEditorSauceLabs\BehatEditorSauceLabsRun')) {
        $message = t('BehatEditorSauceLabsRun not found.');
        drupal_json_output($results = array('error' => 1, 'message' => $message));
        exit();
    }

    $file_object = _behat_get_file_info($module, $filename);
    $res = new Drupal\BehatEditorSauceLabs\BehatEditorSauceLabsRun($file_object);

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

