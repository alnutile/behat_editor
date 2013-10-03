<?php

namespace Drupal\BehatEditorSauceLabs;
use Drupal\BehatEditor\BehatEditorRun;

class BehatEditorSauceLabsRun extends BehatEditorRun {

    public function __construct($file_object) {
        parent::__construct($file_object);
        $path = drupal_get_path('module', 'behat_editor_saucelabs');
        $this->yml_path = drupal_realpath($path) . '/behat/sauce.yml';
    }

    public function exec() {
        exec("cd $this->behat_path && ./bin/behat --config=\"$this->yml_path\" --no-paths  --profile=Selenium-saucelabs2  $this->absolute_file_path", $output);
        $this->file_array = $output;
        $response = is_array($output) ? 0 : 1;
        return array('response' => $response, 'output_file' => $this->output_file, 'output_array' => $output);
    }

}