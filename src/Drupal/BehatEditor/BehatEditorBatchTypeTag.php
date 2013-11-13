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

class BehatEditorBatchTypeTag extends  BehatEditorBatchType {


    function __construct(){
        parent::__construct();
    }

    function setUp($method, $args, $type) {
        $this->method = $method;
        $this->form_values = $args;
        $this->type = $type;
        parent::setupResults();
        $this->operations = self::parseOperations($args);
        parent::setupResultsUpdate();
        self::setBatch();
    }

    function setBatch(){
        $batch = array(
            'operations' => $this->operations,
            'title' => t('Behat Batch by Tags'),
            'file' => drupal_get_path('module', 'behat_editor') . '/includes/behat_editor_tag.batch.inc',
            'init_message' => t('Starting Behat Tests'),
            'error_message' => t('An error occurred. Please check the Reports/DB Logs'),
            'finished' => 'bulk_editor_batch_tag_done',
            'progress_message' => t('Running tests for @number modules. Will return shortly with results.', array('@number' => count($this->operations))),
        );
        $this->batch = $batch;
    }

    private function parseOperations($args) {
        $operations = array();
        foreach($args as $key => $value) {
            $operations[] = array('bulk_editor_batch_run_tag', array($value, $this->rid));
        }
        return $operations;
    }


    function batchRun(array $params) {
        $this->tag = $params['tag'];
        //Later may be more than one tag
        $tag_trimmed = substr($this->tag[0], 1);
        $this->rid = $params['rid'];
        $this->temp_uri = file_build_uri("/behat_batch/$this->rid/$tag_trimmed");
        $prepare = file_prepare_directory($this->temp_uri, $options = FILE_CREATE_DIRECTORY);
        if(!$prepare) {
            $message = t('Temp path could not be created !path', array('!path' => $this->temp_uri));
            throw new \RuntimeException($message);
        }
        self::findFilesAndSetupDirectory();

        $this->file_object = BehatEditor\File::fileObjecBuilder();
        $this->file_object['module'] = 'behat_batch';
        $this->file_object['absolute_path_with_file'] = drupal_realpath($this->temp_uri);
        $this->file_object['relative_path'] = $this->temp_uri;
        $tests = new BehatEditor\BehatEditorRun($this->file_object);
        $results = $tests->exec(1);
        $this->test_results = $results;
    }

    private function findFilesAndSetupDirectory() {
        $file = new BehatEditor\Files();
        $files = $file->getFilesByTag($this->tag);
        foreach($files as $key => $value) {
            $copy = file_unmanaged_copy($value['absolute_path_with_file'], $this->temp_uri, FILE_EXISTS_REPLACE);
        }
    }


}