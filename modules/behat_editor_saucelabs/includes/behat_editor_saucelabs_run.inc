<?php
use Drupal\BehatEditorSauceLabs;

function behat_editor_saucelabs_run($module, $filename) {
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'includes/behat_editor_run');

    composer_manager_register_autoloader();

    if($_REQUEST['method'] == 'view-mode') {
        //@todo include these in autoloader
        module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
        module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');

        if (!class_exists('Drupal\BehatEditorSauceLabs\BehatEditorSauceLabsRun')) {
            $message = t('BehatEditorSauceLabsRun not found.');
            drupal_json_output($results = array('error' => 1, 'message' => $message));
            exit();
        }

        $file = new Drupal\BehatEditor\File($_REQUEST, $module, $filename, FALSE);
        $file_object = $file->get_file_info();
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
    if($_REQUEST['method'] == 'create-mode') {
        if (!class_exists('Drupal\BehatEditorSauceLabs\BehatEditorSauceLabsRun')) {
            $message = t('BehatEditorSauceLabsRun not found.');
            drupal_json_output($results = array('error' => 1, 'message' => $message));
            exit();
        }
        $file = new Drupal\BehatEditor\File($_REQUEST, $module, $filename, 'file');
        $response = $file->save_html_to_file();
        if($response['file'] != FALSE){
            $file_object = $file->get_file_info();
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
            //Issue Creating File
        } else {
            drupal_json_output(array('message' => $response['message'], 'file' => array('message' => $response), 'error' => 1));
            exit();
        }
    }

}

