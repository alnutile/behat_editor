<?php namespace Drupal\BehatEditor;

trait BehatEditorRunHelpers {
    public $yml_path;


    public function setBehatYmlPath($path = null)
    {
        if($path == null) {
            if(variable_get('behat_editor_yml', FALSE) === FALSE) {
                $path = drupal_get_path('module', 'behat_editor');
                $this->yml_path = drupal_realpath($path) . '/behat/behat.yml';
            } else {
                $rel_path = variable_get('behat_editor_yml');
                $this->yml_path = drupal_realpath($rel_path);
            }
        }
        return $this;
    }

    public function getBehatYmlPath()
    {
        if(!$this->yml_path) {
            $this->setBehatYmlPath(null);
        }
        return $this->yml_path;
    }
}