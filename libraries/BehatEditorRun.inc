<?php

namespace BehatEditor\Libraries;

class BehatEditorRun {
    public $behat_path = '';
    public $absolute_file_path = '';
    public $output_file = '';
    public $file_full_path = '';
    public $module = '';
    public $file_object = array();

    /**
     * File object from FileObject class
     * for now using a function
     * check it comes from the class later one
     */
    public function __construct($file_object) {
        $this->behat_path = drupal_get_path('module', 'behat_editor');
        $this->absolute_file_path = '';
        $this->file_full_path = $file_object['absolute_path_with_file'];
        $this->absolute_file_path = $file_object['absolute_path_with_file'];
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
        $response = exec("cd $this->behat_path/behat && ./bin/behat --no-paths --tags '~@javascript' --out $this->output_file  $this->absolute_file_path && echo $?");
        return array('response' => $response, 'output_file' => $this->output_file);
    }
}