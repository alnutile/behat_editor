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
    protected $done_method;
    protected $operations;
    protected $method;
    protected $batch;
    protected $rid;
    protected $temp;
    protected $subfolder;
    protected $test_results;
    protected $form_values;
    protected $file_object;
    protected $module;
    protected $absolute_path;
    protected $path;
    protected $type;
    protected $tag;
    public $message;
    protected $temp_uri;
    protected $pass_fail;

    function __construct(){
        composer_manager_register_autoloader();

    }

    /**
     * Kicks off the process to decide wthat type to use
     *   Module or Tag and later GitHub etc.
     * @param $method
     * @return BehatEditorBatchTypeModule|BehatEditorBatchTypeTag
     */
    static function type($method) {
        if($method == 'module') {
            $batchType = new BehatEditorBatchTypeModule();
            return $batchType;
        } else {
            $batchType = new BehatEditorBatchTypeTag();
            return $batchType;
        }
    }

    function setUp($method, $args, $type) {
        $this->method = $method;
        $this->form_values = $args;
        $this->type = $type;
        $this->setupResults();
        $this->operations = $this->parseOperations($args);
        $this->setupResultsUpdate();
        $this->setBatch();
    }


    protected function setBatch() {}

    function getBatch() {
        return $this->batch;
    }

    protected function parseOperations($operations) {}


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
        //@bug cause a dblog error to pass $this->message to the two next lines will
        //  need to dry this up though
        drupal_set_message(t("Ran batch test for @item with a result of \"@result\"", array('@item' => $params['item'], '@result' => $this->pass_fail)));
        watchdog("behat_editor_batch", t("Ran batch test for @item with a result of \"@result\"", array('@item' => $params['item'], '@result' => $this->pass_fail)), WATCHDOG_INFO);

        $this->wrapUp($fields);
        $updateResults = new BehatEditor\ResultsBatch();
        $updateResults->update($this->rid, $fields);
        return $this->message;
    }

    /**
     * Wrap up tests
     *   Set Batch Results to 2 for DONE
     *   Clean up any files if any
     * @param $fields
     */
    protected function wrapUp(&$fields) {
        if($fields['results_count'] == $fields['test_count']) {
            $fields['batch_status'] = 2;
        }
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

}