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
        );
    }

    public function insert() {
        $insert = db_insert('behat_editor_results')->fields($this->fields);
        $insert->execute();
        return $insert;
    }

    static public function getResultsForFile($module, $filename) {
        $query = db_select('behat_editor_results', 'b');
        $query->fields('b', array('rid', 'filename', 'module', 'results', 'duration', 'created'));
        $query->condition('b.filename', $filename, 'LIKE');
        $query->condition('b.module', $module, 'LIKE');
        $result = $query->execute();
        if ($result) {
            foreach ($result as $record) {
                $date = format_date($record->created, $type = 'custom', $format = 'Y-m-d H:i');
                $rows[] = array($record->filename, $record->module, $record->duration, $date);
            }
        }
        return array('results' => $rows, 'error' => 0);
    }

}