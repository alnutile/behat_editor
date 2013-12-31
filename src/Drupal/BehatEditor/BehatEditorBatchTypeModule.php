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

class BehatEditorBatchTypeModule extends  BehatEditorBatchType {


    function __construct(){
        parent::__construct();
    }



    function setBatch(){
            $batch = array(
                'operations' => $this->operations,
                'title' => t('Behat Batch by Module and Folder'),
                'file' => drupal_get_path('module', 'behat_editor') . '/includes/behat_editor_module.batch.inc',
                'init_message' => t('Starting Behat Tests'),
                'error_message' => t('An error occurred. Please check the Reports/DB Logs'),
                'finished' => 'bulk_editor_batch_module_done',
                'progress_message' => t('Running tests for @number modules. Will return shortly with results.', array('@number' => count($this->operations))),
            );
            $this->batch = $batch;
    }

    protected function parseOperations($args, $settings) {
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
            $operations[] = array('bulk_editor_batch_run_module', array($module, $subfolder, $this->rid, $settings));
        }
        return $operations;
    }


    function batchRun(array $params) {
        $this->module = $params['module'];
        $this->subfolder = $params['subfolder'];
        $this->definePaths();
        $this->rid = $params['rid'];
        $this->file_object = BehatEditor\FileModel::fileObjecBuilder();
        $this->file_object['module'] = $this->module;
        $this->file_object['filename'] = "behat_batch|{$this->rid}";
        $this->file_object['absolute_path_with_file'] = $this->absolute_path;
        $this->file_object['relative_path'] = $this->path;
        $this->settings['filename'] = $this->file_object['filename'];
        $this->settings['module'] = 'behat_batch';
        $tests = new BehatEditor\BehatEditorRun($this->file_object);
        $results = $tests->exec(1, $this->settings, 'behat_run_batch', "~@disabled");
        $this->test_results = $results;
    }


}