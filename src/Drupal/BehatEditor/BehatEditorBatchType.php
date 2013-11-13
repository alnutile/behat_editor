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

abstract class BehatEditorBatchType {
    public $done_method;
    public $operations;
    public $method;
    public $batch;
    public $rid;
    public $temp;
    public $subfolder;
    public $test_results;
    public $form_values;
    public $file_object;
    public $module;
    public $absolute_path;
    public $path;
    public $type;
    public $tag;
    public $message;
    public $temp_uri;
    public $pass_fail;

    function __construct(){
        composer_manager_register_autoloader();

    }

    static function type($method) {
        if($method == 'module') {
            $batchType = new BehatEditorBatchTypeModule();
            return $batchType;
        } else {
            $batchType = new BehatEditorBatchTypeTag();
            return $batchType;
        }
    }

    function setUp($method, $args, $type) {}

    protected function setBatch() {}

    function getBatch() {
        return $this->batch;
    }

    private function parseOperations($operations) {}


    function setupResults() {
        $results = new ResultsBatch();
        $results->fields['batch_status'] = 1;
        $results->fields['operations'] = serialize($this->form_values);
        $results->fields['method'] = $this->method;
        $rid = $results->insert();
        $this->rid = $rid;
    }



    function batchItemDone(array $params) {
        $results_of_test = $this->test_results;
        $resultsUpdate = BehatEditor\ResultsBatch::getResultsByRid($this->rid);
        $fields = $resultsUpdate['results'];
        $rids = (is_array(unserialize($fields['results']))) ? unserialize($fields['results']) : array();
        $fields['results'] = serialize(drupal_map_assoc(array($results_of_test['rid'])) + $rids);
        $fields['count_at'] = $fields['count_at'] + 1;
        $fields['results_count'] = $fields['results_count'] + 1;
        $fields['pass_fail'] = ( $fields['pass_fail'] != 1 ) ? $results_of_test['response'] : 1; //leave as fail
        $this->pass_fail = BehatEditor\ResultsBatch::getResultsPassFail($results_of_test['response']);

        $this->message = t("Ran batch test for @item with a result of \"@result\"", array('@item' => $params['item'], '@result' => $this->pass_fail));
        drupal_set_message($this->message);
        watchdog("behat_editor_batch", $this->message, WATCHDOG_INFO);

        // Only change is not already Fail
        // since it is a FAIL if one test fails
        if($fields['results_count'] == $fields['test_count']) { $fields['batch_status'] = 2; }

        $updateResults = new BehatEditor\ResultsBatch();
        $updateResults->update($this->rid, $fields);

        return $this->message;
    }


    function batchDone($success, $results, $operations, $message) {

        return t("@message", array('@message' => $message));
    }

    function setupResultsUpdate(){
        $results = BehatEditor\ResultsBatch::getResultsByRid($this->rid);
        $fields = $results['results'];
        $fields['rid'] = $this->rid;
        $fields['test_count'] = count($this->operations);
        $update = new BehatEditor\ResultsBatch();
        $update->update($this->rid, $fields);
    }


    abstract function batchRun(array $params);

    protected  function definePaths() {
        if($this->module == BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER) {
            $this->temp = BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER;
            $this->path = file_build_uri("/{$this->temp}");
            if($this->subfolder !== FALSE && $this->subfolder !== 0) {
                $this->path = $this->path . '/' . $this->subfolder;
            }
            $this->absolute_path = drupal_realpath($this->path);
        } else {
            $this->path = drupal_get_path('module', $this->module) . '/' . BEHAT_EDITOR_FOLDER;
            if($this->subfolder !== FALSE && $this->subfolder !== 0) {
                $this->path = $this->path . '/' . $this->subfolder;
            }
            $this->absolute_path = realpath($this->path);
        }
    }


    private function findFiles() {}
    private function copyFiles() {}
}