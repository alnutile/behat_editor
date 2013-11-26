<?php

namespace Drupal\BehatEditor;
use Drupal\BehatEditor;

class BehatEditorRunModuleFolderBasedTests {
    public $file_object;
    public $module;
    public $subfolder;
    public $path;
    public $temp;
    public $absolute_path;
    public $rid; //id from batch results table

    public function __construct($module, $subfolder, $rid){
        composer_manager_register_autoloader();
        $this->file_object = BehatEditor\File::fileObjecBuilder();
        $this->module = $module;
        $this->subfolder = $subfolder;
        $this->absolute_path;
        $this->temp;
        $this->path;
        self::definePaths();
    }

    public function runTests() {
        //@todo DRY this up
        $this->file_object['module'] = $this->module;
        $this->file_object['absolute_path_with_file'] = $this->absolute_path;
        $this->file_object['relative_path'] = $this->path;
        $tests = new BehatEditor\BehatEditorRun($this->file_object);
        $results = $tests->exec(1);
        $context['message'] = "Running $this->module";
        $context['results'] = "test";
        return array('context' => $context, 'results' => $results);
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

}