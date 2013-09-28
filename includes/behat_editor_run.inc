<?php
use Drupal\BehatEditor;
/**
 * @todo this is running and saving it should only run
 */
function behat_editor_run($module, $filename) {
    composer_manager_register_autoloader();

    if($_REQUEST['method'] == 'view-mode') {
        //@todo include these in autoloader
        module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
        module_load_include('inc', 'behat_editor', 'helpers/behat_editor_file_parsing');
        module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');

        $file_object = _behat_get_file_info($module, $filename);
        $res = new Drupal\BehatEditor\BehatEditorRun($file_object);
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
        module_load_include('inc', 'behat_editor', 'helpers/behat_editor_file_parsing');
        module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
        //Make sure it is a good request
        if(isset($_REQUEST['scenario'])) {
            $response = _behat_editor_save_html_to_file($_REQUEST, $module, $filename, 'file');
            //Make sure the file was made
            if($response['file'] != FALSE){
                $file_object = _behat_get_file_info($module, $filename);
                $res = new Drupal\BehatEditor\BehatEditorRun($file_object);
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


