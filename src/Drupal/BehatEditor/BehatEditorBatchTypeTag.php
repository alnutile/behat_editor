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
    public $tags = array();
    public $total_files;
    public $files_all = array();

    function __construct(){
        parent::__construct();
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

    protected function parseOperations($args, $settings) {
        $operations = array();
        foreach($args as $key => $value) {
            $operations[] = array('bulk_editor_batch_run_tag', array($value, $this->rid, $settings));
        }
        return $operations;
    }

    function getFileCount() {
        return $this->total_files;
    }

    function getAllFilesArray(){
        return $this->files_all;
    }

    function setTotalCount($params) {
        $rid = $params['rid'];
        $test_count = $params['test_count'];
        $results = BehatEditor\ResultsBatch::getResultsByRid($rid);
        $fields = $results['results'];
        $fields['test_count'] = $test_count;
        $update = new BehatEditor\ResultsBatch();
        $update->update($this->rid, $fields);
    }

    function batchRunForFile(array $params) {
        $this->tag = $params['tag'];
        $this->settings = $params['settings'];
        //Later may be more than one tag
        $tag_trimmed = substr($this->tag[0], 1);
        $this->rid = $params['rid'];
        $file_path = explode('/', $params['file_path']);
        $filename = array_pop($file_path);
        $file_object = new BehatEditor\FileModel();
        $this->file_object = $file_object->fileObjecBuilder();
        $this->file_object['module'] = 'behat_batch';
        $this->file_object['filename'] = $filename;
        $this->file_object['absolute_path_with_file'] = $params['file_path'];
        $this->file_object['relative_path'] = implode('/', $file_path);
        $this->settings['filename'] = $this->file_object['filename'];
        $this->settings['module'] = 'behat_batch';
        $this->settings['context'] = 'behat_run_batch';
        $tests = new BehatEditor\BehatEditorRun($this->file_object);
        $results = $tests->exec(1, $this->settings, 'behat_run_batch', "~@disabled");
        $this->test_results = $results;
        return $results;
    }

    //@TODO BPFD-258 makes this no longer needed once that is done
    function batchRun(array $params) {
        $this->tag = $params['tag'];
        $this->settings = $params['settings'];
        //Later may be more than one tag
        $tag_trimmed = substr($this->tag[0], 1);
        $this->rid = $params['rid'];
        $this->temp_uri = file_build_uri("/behat_batch/$this->rid/$tag_trimmed");
        $prepare = file_prepare_directory($this->temp_uri, $options = FILE_CREATE_DIRECTORY);
        if(!$prepare) {
            $message = t('Temp path could not be created !path', array('!path' => $this->temp_uri));
            throw new \RuntimeException($message);
        }
        $this->findFilesAndSetupDirectory();
        $file_object = new BehatEditor\FileModel();
        $this->file_object = $file_object->fileObjecBuilder();
        $this->file_object['module'] = 'behat_batch';
        $this->file_object['filename'] = "behat_batch|{$this->rid}";
        $this->file_object['absolute_path_with_file'] = drupal_realpath($this->temp_uri);
        $this->file_object['relative_path'] = $this->temp_uri;
        $this->settings['filename'] = $this->file_object['filename'];
        $this->settings['module'] = 'behat_batch';
        $this->settings['context'] = 'behat_run_batch';
        $tests = new BehatEditor\BehatEditorRun($this->file_object);
        $results = $tests->exec(1, $this->settings, 'behat_run_batch', "~@disabled");
        $this->test_results = $results;
    }

    function batchSetupFolderCopyFiles(array $params) {
        $this->tags = $params['tags'];
        $this->settings = $params['settings'];
        //Later may be more than one tag
        $this->total_files = 0;
        foreach($this->tags as $tag) {
            $this->tag = $tag;
            $tag_trimmed = substr($this->tag[0], 1);
            $this->rid = $params['rid'];
            $this->temp_uri = file_build_uri("/behat_batch/$this->rid/$tag_trimmed");
            $prepare = file_prepare_directory($this->temp_uri, $options = FILE_CREATE_DIRECTORY);
            if(!$prepare) {
                $message = t('Temp path could not be created !path', array('!path' => $this->temp_uri));
                throw new \RuntimeException($message);
            }
            $this->findFilesAndSetupDirectory();
        }
    }

    protected function wrapUp(&$fields) {
        //@todo make sure to add this back to clean up after tests
        //file_unmanaged_delete_recursive(file_build_uri("/behat_batch/{$this->rid}"));
        parent::wrapUp($fields);
    }

    protected function findFilesAndSetupDirectory() {
        $file = new BehatEditor\FileModel(array());
        $files = $file->getFilesByTag(array($this->tag));
        $this->total_files = count($files);
        foreach($files as $key => $value) {
            $copy = file_unmanaged_copy($value['absolute_path_with_file'], $this->temp_uri, FILE_EXISTS_REPLACE);
            $files_all[$this->tag][] = $copy;
        }
        $this->files_all = $files_all;
    }


}