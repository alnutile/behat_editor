<?php

/**
 * @file
 * Contains \Drupal\BehatEditor\BehatEditorRun.
 */

namespace Drupal\BehatEditor;

/**
 * Class Results
 * Methods to save results
 *
 *
 */

class ResultsBatch {
    public $fields = array();

    public function __construct() {
        $this->fields = self::fields();
    }

    public function insert() {
        $insert = db_insert('behat_editor_batch_results')->fields($this->fields)->execute();
        return $insert;
    }

    public function update($rid, $fields) {
        $update = db_update('behat_editor_batch_results')
            ->fields($fields)
            ->condition('rid', $rid, '=')
            ->execute();
        return $update;
    }


    static public function getResultsByRid($rid) {
        $query = db_select('behat_editor_batch_results', 'b');
        $query->fields('b');
        $query->condition('b.rid', $rid);
        $result = $query->execute();

        return array('results' => $result->fetchAssoc(), 'error' => 0);
    }

    /**
     * @todo this query needs to be optimized
     * @param $array_key
     * @return array
     */
    static public function getResultsForByModule($array_key) {
        $query = db_select('behat_editor_batch_results', 'b');
        $query->fields('b');
        $query->condition('b.operations', "%\"$array_key\"%", 'LIKE');
        $query->orderBy('b.created', 'DESC');
        $query->range(0, 1);
        $result = $query->execute();
        $rows = array();
        if ($result) {
            foreach ($result as $record) {
                $record->results = unserialize($record->results);
                $rows[] = (array) $record;
            }
        }
        return array('results' => $rows, 'error' => 0);
    }

    static public function getResultsForByTag($module, $folder, $tags) {
        $query = db_select('behat_editor_batch_results', 'b');
        $query->fields('b');
        $query->condition('b.folder', $folder, 'LIKE');
        $query->condition('b.module', $module, 'LIKE');
        $query->orderBy('b.created', 'DESC');
        $result = $query->execute();
        $rows = array();
        if ($result) {
            foreach ($result as $record) {
                $record->results = unserialize($record->results);
                $rows[] = (array) $record;
            }
        }
        return array('results' => $rows, 'error' => 0);
    }

    /**
     * @todo make globals
     *   pending, running, pass, fail
     * @param $number
     * @return string
     */
    static public function getBatchRunningStatus($number) {
        switch($number) {
            case 0:
                return "Pending";
            case 1:
                return "Running";
            case 2:
                return "Done";

        }
    }

    static public function getResultsPassFail($number) {
        switch($number) {
            case 0:
                return "Pass";
            case 1:
                return "Fail";
            case 2:
                return "Pending";

        }
    }

    private function fields() {
        global $user;
        $fields['bid'] = 0;
        $fields['jid'] = 0;
        $fields['uid'] = $user->uid;
        $fields['created'] = REQUEST_TIME;
        $fields['type'] = 'non_background';
        $fields['method'] = NULL;
        $fields['operations'] = NULL;
        $fields['results'] = NULL;
        $fields['batch_status'] = 3;
        $fields['test_count'] = 0;
        $fields['count_at'] = 0;
        $fields['results_count'] = 0;
        $fields['pass_fail'] = 3;
        $fields['repo'] = NULL;
        return $fields;
    }

}