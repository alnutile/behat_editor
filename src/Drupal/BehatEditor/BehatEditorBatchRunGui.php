<?php

/**
 * @file
 * Contains \Drupal\BehatEditor\BehatEditorBatchRun.
 */

namespace Drupal\BehatEditor;

use Drupal\BehatEditor;


class BehatEditorBatchRunGui {
    public $operations;
    public $rid;
    public $test_results;

    public function __construct(){
        composer_manager_register_autoloader();

    }

    function batchSubmit($method, $args) {
        $batch = BehatEditorBatchType::type($method);
        $batch->setUp($method, $args);
        return $batch;
    }

}
