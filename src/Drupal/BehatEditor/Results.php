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

}