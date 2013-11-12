<?php

/**
 * @file
 * Contains \Drupal\BehatEditor\BehatEditorBatchRun.
 */

namespace Drupal\BehatEditor;

use Drupal\BehatEditor;

/**
 * Class BehatEditorBatchRun
 * Methods needed to run a test from a batch standpoint.
 *
 * @params file_object
 *      This is created by \Drupal\BehatEditor\File class
 * @package Drupal\BehatEditor
 *
 * @todo merge this into BehatEditorRun or an Interface / Abstract for both of them
 */

abstract class BehatEditorBatchRun {

    public function __construct(){
        composer_manager_register_autoloader();
    }

    /**
     * @param $type
     *   gui or background
     * @param $method
     * @param $args
     *   array of tags or modules and subfolders
     *   array('tag' => array('tag1', 'tag2')
     *   array('module' => array('form values')
     *
     */
    static public function runType($type, $method, array $args) {
        if($type == 'gui') {
            $run = new BehatEditorBatchRunGui();
            $run->batchSubmit($method, $args);
            return $run;
        }
    }

    abstract function parseOperations();

    abstract function batchStart();

    abstract function batchRun($module, $subfolder, $rid);

    abstract function setupResults();

    abstract function setupResultsUpdate($operations);

    static function batchDone($success, $results, $operations, $message) {

    }

    abstract  function batchItemDone();
}
