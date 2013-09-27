<?php

namespace BehatEditor\SauceLabs;
use BehatEditor\Libraries\BehatEditorRun;

//@todo pull this in with above ann ot user module_load_include
// should be using composer_manager
module_load_include('inc', 'behat_editor', 'libraries/BehatEditorRun');

class BehatEditorSauceLabsRun extends BehatEditorRun {


    public function __construct($file_object) {
        parent::__construct($file_object);
    }

    public function exec() {
        $response = exec("cd $this->behat_path && ./bin/behat --config=\"$this->yml_path\" --no-paths --out $this->output_file  --profile=Selenium-saucelabs2 $this->absolute_file_path && echo $?");
        return array('response' => $response, 'output_file' => $this->output_file);
    }
}