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

class Results {
    public $fields = array();

    public function __construct() {
        $this->fields = array(
            'filename' => '',
            'module' => '',
            'results' => '',
            'duration' => '',
            'created' => '',
            'status' => '',
        );
    }

    public function insert() {
        $insert = db_insert('behat_editor_results')->fields($this->fields)->execute();
        return $insert;
    }

    static public function getResultsForFile($module, $filename) {
        $query = db_select('behat_editor_results', 'b');
        $query->fields('b');
        $query->condition('b.filename', $filename, 'LIKE');
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

    static public function getResultsForRids(array $rids) {
        $query = db_select('behat_editor_results', 'b');
        $query->fields('b');
        $query->condition('b.rid', $rids, 'IN');
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

    static function generateHTMLOutput($results_array) {
        $results_message = array_slice($results_array, -3);
        $results_message_top = array_slice($results_array, 0, -3);
        $output_item_results = theme('item_list', $var = array('title' => 'Summary', 'items' => $results_message));
        $output_item_list = theme('item_list', $var = array('title' => 'All Results', 'items' => $results_message_top));
        return $output_item_results . $output_item_list;
    }


}