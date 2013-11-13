<?php

namespace Drupal\BehatEditor;

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

    function __construct(){

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

    abstract function setUp($method, $operations, $type);
    abstract function setBatch();
    abstract function getBatch();
    private function parseOperations($operations) {}

    function setupResults() {
        $results = new ResultsBatch();
        $results->fields['batch_status'] = 1;
        $results->fields['operations'] = serialize($this->form_values);
        $results->fields['method'] = $this->method;
        $rid = $results->insert();
        $this->rid = $rid;
    }

    abstract function batchItemDone();
    abstract function batchDone($success, $results, $operations, $message);
    abstract function setupResultsUpdate();
    abstract function batchRun(array $params);
    private function definePaths() {}
    private function findFiles() {}
    private function copyFiles() {}
}