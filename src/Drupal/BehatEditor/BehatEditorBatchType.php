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
            $done = 'bulk_editor_batch_tag_done';
        }
    }

    abstract function setUp($method, $operations);
    abstract function setBatch();
    abstract function getBatch();
    private function parseOperations($operations) {}
    abstract function setupResults();
    abstract function batchItemDone();
    abstract function batchDone($success, $results, $operations, $message);
    abstract function setupResultsUpdate();
    abstract function batchRun($module, $subfolder, $rid);
    private function definePaths() {

    }
}