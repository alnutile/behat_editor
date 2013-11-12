<?php

namespace Drupal\BehatEditor;

use Drupal\BehatEditor;

/**
 * Define the type of batch job eg
 * Module / Folder
 * Tag
 * Github
 * etc
 */

class BehatEditorBatchTypeModule extends  BehatEditorBatchType{
    public $done_method;
    public $operations;
    public $method;
    public $batch;
    public $rid;
    public $subfolder;
    public $test_results;
    public $form_values;

    function __construct(){
        composer_manager_register_autoloader();
    }

    function setUp($method, $args) {
        $this->method = $method;
        $this->form_values = $args;
        self::setupResults();
        $this->operations = self::parseOperations($args);
        self::setupResultsUpdate();
        self::setBatch();
    }

    function setBatch(){
            $batch = array(
                'operations' => $this->operations,
                'title' => t('Behat Batch by Module and Folder'),
                'file' => drupal_get_path('module', 'behat_editor') . '/behat_editor.batch.inc',
                'init_message' => t('Starting Behat Tests'),
                'error_message' => t('An error occurred. Please check the Reports/DB Logs'),
                'finished' => 'bulk_editor_batch_module_done',
                'progress_message' => t('Running tests for @number modules. Will return shortly with results.', array('@number' => count($this->operations))),
            );
            $this->batch = $batch;
    }

    function getBatch() {
        return $this->batch;
    }

    private function parseOperations($args) {
        $operations = array();
        foreach($args as $key) {
            if(strpos($key, '|')) {
                $array = explode('|', $key);
                $module = $array[0];
                $subfolder = $array[1];
            } else {
                $module = $key;
                $subfolder = FALSE;
            }
            $operations[] = array('bulk_editor_batch_run_module', array($module, $subfolder, $this->rid));
        }
        return $operations;
    }

    function setupResults() {
        $results = new ResultsBatch();
        $results->fields['batch_status'] = 1;
        $results->fields['operations'] = serialize($this->form_values);
        $results->fields['method'] = $this->method;
        $rid = $results->insert();
        $this->rid = $rid;
    }

    function setupResultsUpdate(){
        $results = BehatEditor\ResultsBatch::getResultsByRid($this->rid);
        $fields = $results['results'];
        $fields['rid'] = $this->rid;
        $fields['test_count'] = count($this->operations);
        $update = new BehatEditor\ResultsBatch();
        $update->update($this->rid, $fields);
    }

    function batchRun($module, $subfolder, $rid) {
        $this->rid = $rid;
        $this->module = $module;
        $this->subfolder = $subfolder;
        $run = new BehatEditor\BehatEditorRunModuleFolderBasedTests($module, $subfolder, $rid);
        $this->test_results = $run->runTests();
    }

    function batchItemDone() {
        $results_of_test = $this->test_results['results'];
        $resultsUpdate = BehatEditor\ResultsBatch::getResultsByRid($this->rid);
        $fields = $resultsUpdate['results'];
        $rids = (is_array(unserialize($fields['results']))) ? unserialize($fields['results']) : array();
        $fields['results'] = serialize(drupal_map_assoc(array($results_of_test['rid'])) + $rids);
        $fields['count_at'] = $fields['count_at'] + 1;
        $fields['results_count'] = $fields['results_count'] + 1;
        $fields['pass_fail'] = ( $fields['pass_fail'] != 1 ) ? $results_of_test['response'] : 1; //leave as fail
        $pass_fail = BehatEditor\ResultsBatch::getResultsPassFail($results_of_test['response']);
        drupal_set_message(t("Ran batch test for @module @folder with a result of \"@result\"", array('@module' => $this->module, '@folder' => $this->subfolder, '@result' => $pass_fail)));
        //Only change is not already Fail since it is a FAIL if one test fails
        if($fields['results_count'] == $fields['test_count']) { $fields['batch_status'] = 2; }

        $updateResults = new BehatEditor\ResultsBatch();
        $updateResults->update($this->rid, $fields);

        return $updateResults;
    }

    function batchDone($success, $results, $operations, $message) {

        return t("@message", array('@message' => $message));
    }
}