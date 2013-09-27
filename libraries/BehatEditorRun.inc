<?php

namespace BehatEditor\Libraries;

class BehatEditorRun {
    public $behat_path = '';
    public $absolute_file_path = '';
    public $output_file = '';
    public $file_full_path = '';
    public $module = '';
    public $relative_path = '';
    public $filename = '';
    public $yml_path = '';
    public $file_object = array();

    /**
     * File object from FileObject class
     * for now using a function
     * check it comes from the class later one
     */
    public function __construct($file_object) {
        $path = drupal_get_path('module', 'behat_editor');
        $this->yml_path = drupal_realpath($path) . '/behat/behat.yml';
        $this->behat_path = _behat_editor_behat_bin_folder();
        $this->absolute_file_path = '';
        $this->file_full_path = $file_object['absolute_path_with_file'];
        $this->absolute_file_path = $file_object['absolute_path_with_file'];
        $this->relative_path = $file_object['relative_path'];
        $this->filename = $file_object['filename'];
        $this->module = $file_object['module'];
        $this->file_object = $file_object;
        $this->output_file = self::getPath();
    }

    public function getPath() {
        if(user_access('behat add test') && $this->module != variable_get('behat_editor_default_storage_folder', BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER)) {
            return  _behat_editor_run_tests_from_module_folder($this->file_object);
        } else {
            return _behat_editor_run_tests_from_tmp_folder($this->file_object);
        }
    }

    public function exec() {
        $response = exec("cd $this->behat_path && ./bin/behat --config=\"$this->yml_path\" --no-paths --tags '~@javascript' --out $this->output_file  $this->absolute_file_path && echo $?");
        return array('response' => $response, 'output_file' => $this->output_file);
    }

    public function generateReturnPassOutput() {
        $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
        $file_url = l($this->filename, $this->relative_path, $options = array('attributes' => array('target' => '_blank', 'id' => 'test-file')));
        $file_message = array(
            'file' => $this->filename,
            'error' => 0,
            'message' => "$date <br> File $file_url tested."
        );
        $results = _behat_editor_read_file($this->output_file);
        $output_item_list = _behat_editor_output_html_item_list($results);
        $output = array('message' => t('@date: <br> Test successful!', array('@date' => $date)), 'file' => $output_item_list, 'error' => FALSE);
        $results = array('file' => $file_message, 'test' => $output, 'error' => 0);
        return $results;
    }

    public function generateReturnFailOutput() {
        $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
        $file_url = l($this->filename, $this->relative_path, $options = array('attributes' => array('target' => '_blank', 'id' => 'test-file')));
        $file_message = array(
            'file' => $this->filename,
            'error' => 0,
            'message' => "$date <br> File $file_url tested."
        );
        watchdog('behat_editor', "%date Error Running Test %name", $variables = array('%date' => $date, '%name' => $this->output_file), $severity = WATCHDOG_ERROR, $link = $this->absolute_file_path);
        $output = array('message' => t('@date: <br> Error running test !name to download ', array('@date' => $date, '@name' => $this->filename)), 'file' => $this->filename, 'error' => TRUE);
        $results = array('file' => $file_message, 'test' => $output, 'error' => 1);
        return $results;
    }
}