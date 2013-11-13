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

class BehatEditorBatchTypeTag extends  BehatEditorBatchType{
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
    public $temp_uri;

    function __construct(){
        composer_manager_register_autoloader();
    }

    function setUp($method, $args, $type) {
        $this->method = $method;
        $this->form_values = $args;
        $this->type = $type;
        self::setupResults();
        $this->operations = self::parseOperations($args);
        self::setupResultsUpdate();
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

    function getBatch() {
        return $this->batch;
    }

    private function parseOperations($args) {
        $operations = array();
        foreach($args as $key => $value) {
            $operations[] = array('bulk_editor_batch_run_tag', array($value, $this->rid));
        }
        return $operations;
    }


    function setupResultsUpdate(){
        $results = BehatEditor\ResultsBatch::getResultsByRid($this->rid);
        $fields = $results['results'];
        $fields['rid'] = $this->rid;
        $fields['test_count'] = count($this->operations);
        $update = new BehatEditor\ResultsBatch();
        $update->update($this->rid, $fields);
    }

    function batchRun(array $params) {
        $this->tag = $params['tag'];
        $tag_trimmed = substr($this->tag, 1);
        $this->rid = $params['rid'];
        //Find all files with that tag
        // copy them into a folder
        // behat_tests/{rid}/{tag}
        $this->temp_uri = file_build_uri("/behat_batch/$this->rid/$tag_trimmed");
        $prepare = file_prepare_directory($this->temp_uri, $options = FILE_CREATE_DIRECTORY);
        if(!$prepare) {
            $message = t('Temp path could not be created !path', array('!path' => $this->temp_uri));
            throw new \RuntimeException($message);
        }
        self::findFilesAndSetupDirectory();

        //self::definePaths();

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

    public function definePaths() {
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

    function batchItemDone() {
        $results_of_test = $this->test_results;
        $resultsUpdate = BehatEditor\ResultsBatch::getResultsByRid($this->rid);
        $fields = $resultsUpdate['results'];
        $rids = (is_array(unserialize($fields['results']))) ? unserialize($fields['results']) : array();
        $fields['results'] = serialize(drupal_map_assoc(array($results_of_test['rid'])) + $rids);
        $fields['count_at'] = $fields['count_at'] + 1;
        $fields['results_count'] = $fields['results_count'] + 1;
        $fields['pass_fail'] = ( $fields['pass_fail'] != 1 ) ? $results_of_test['response'] : 1; //leave as fail
        $pass_fail = BehatEditor\ResultsBatch::getResultsPassFail($results_of_test['response']);
        drupal_set_message(t("Ran batch test for tag @tag a result of \"@result\"", array('@tag' => $this->tag, '@result' => $pass_fail)));
        // Only change is not already Fail
        // since it is a FAIL if one test fails
        if($fields['results_count'] == $fields['test_count']) { $fields['batch_status'] = 2; }

        $updateResults = new BehatEditor\ResultsBatch();
        $updateResults->update($this->rid, $fields);

        return $updateResults;
    }



    function batchDone($success, $results, $operations, $message) {

        return t("@message", array('@message' => $message));
    }
}