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
 * @todo maybe make an abstract Results class
 *   to deal with the slight differences in Results and ResultsBatch
 */

class Results {
    public $fields = array();
    public $results_cleaned;
    public $duration;

    public function __construct() {
        global $user;
        $this->fields = array(
            'filename' => '',
            'module' => '',
            'results' => '',
            'duration' => '',
            'created' => '',
            'status' => '',
            'uid' => $user->uid,
            'settings' => array(),
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

    public function cleanHtml($results){
        $results_imploded = implode("", $results);
        $s = strpos($results_imploded, '<body>') + strlen('<body>');
        $f = '<div class="switchers">';
        $results_html = trim(substr($results_imploded, $s, strpos($results_imploded, $f) - $s)) . "</div>";
        $this->results_cleaned = $results_html;
    }

    public function getDuration() {
        $s = strpos($this->results_cleaned, '<p class="time">') + strlen('<p class="time">');
        $f = 's</p>';
        $duration = trim(substr($this->results_cleaned, $s, strpos($this->results_cleaned, $f) - $s));
        $this->duration = (!empty($duration)) ? $duration : '0';
        return $this->duration;
    }

    static function generateHTMLOutput($results_array) {
        $results_message = array_slice($results_array, -3);
        $results_message_top = array_slice($results_array, 0, -3);
        $output_item_results = theme('item_list', $var = array('title' => 'Summary', 'items' => $results_message));
        $output_item_list = theme('item_list', $var = array('title' => 'All Results', 'items' => $results_message_top));
        return $output_item_results . $output_item_list;
    }


}