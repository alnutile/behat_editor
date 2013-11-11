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
        $this->fields = array(
            'bid' => '',
            'jid' => '',
            'uid' => '',
            'created' => '',
            'duration' => '',
            'module' => '',
            'folder' => '',
            'subfolder' => '',
            'tag' => '',
            'results' => '',
            'batch_status' => '',
            'status' => '',
        );
    }

    public function insert() {
        $insert = db_insert('behat_editor_batch_results')->fields($this->fields)->execute();
        return $insert;
    }

    static public function getResultsForByModule($module, $folder, $subfolder = FALSE) {
        $query = db_select('behat_editor_batch_results', 'b');
        $query->fields('b');
        $query->condition('b.folder', $folder, 'LIKE');
        $query->condition('b.module', $module, 'LIKE');
        if($subfolder === 0  && $subfolder !== FALSE) {
            $query->condition('b.subfolder', $subfolder, 'IS NULL');
        } else {
            $query->condition('b.subfolder', $subfolder, 'LIKE');
        }

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

    static public function getResultsLabel($number) {
        switch($number) {
            case 0:
                return "N/A";
            case 1:
                return "Done";
            case 2:
                return "Running";

        }
    }

    static public function getResultsPassFail($number) {
        switch($number) {
            case 0:
                return "Pass";
            case 1:
                return "Fail";
            case 2:
                return "N/A";

        }
    }

}