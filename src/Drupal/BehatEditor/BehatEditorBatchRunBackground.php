<?php

/**
 * @file
 * Contains \Drupal\BehatEditor\BehatEditorBatchRun.
 */

namespace Drupal\BehatEditor;

use Drupal\BehatEditor;



class BehatEditorBatchRunBackground {
    public function __construct(){
        composer_manager_register_autoloader();
    }

    function parseOperations() {

    }

    function batchStart() {

    }

    function batchRun() {

    }

    static function batchDone($success, $results, $operations) {

    }
}