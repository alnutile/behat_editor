<?php
/**
 * @file
 * Contains \Drupal\BehatEditor\File.
 */

namespace Drupal\BehatEditor;

/**
 * Class File
 * Methods needed to run a process the test file.
 *
 */
class FileBuilder extends File {
    public $module = '';
    public $filename = '';
    public $parse_type = '';
    public $scenario_array = array();
    public $scenario = '';
    public $feature = '';
    public $subpath = '';
    public $relative_path = '';
    public $full_path_with_file = '';
    public $full_path = '';
    public $relative_path_with_no_file_name = '';
    public $relative_path_with_file = '';
    public $file_text = '';
    public $tags_array = array();
    public $root_folder = '';
    public $test_folder_and_file = '';

    const BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER = 'behat_features';

    public function __construct($params = array()) {}

    /**
     * @param $params
     *   module name = string
     *   service_path = array()
     *   filename = string
     * @return fileObject
     */
    public function buildObject($params){
        $this->module = $params['module'];
        $this->filename = $params['filename'];
        if(isset($params['parse_type'])) {
            $this->parse_type = $params['parse_type'];
        }
        $path = drupal_get_path('module', $this->module);
        if(!empty($path)) {
            $file_object = $this->buildFileObjectFromModule($params, $path);
            return $file_object;
        } elseif($this->module == BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER) {
            $path = BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER;
            $file_object = $this->buildFileObjectFromBehatTests($params, $path);
            return $file_object;
        } else {
            //offer alter
            drupal_alter('behat_editor_build_path', $data, $params);
            if(is_array($data) || is_array($params)) {
                if(empty($data)) {
                    $data = $params;
                }
                $file_object = $this->buildFileObjectFromHook($data);
                return $file_object;
            }
        }
    }

    /**
     * @param array $params
     *   get the service path / arg
     *   filename
     *   module name
     * @param $path
     *   path of the module
     * @return fileObject
     */
    protected function buildFileObjectFromHook(array $data){
        watchdog('test_run_on_hook', print_r($data, 1));

        $this->root_folder = $data['subpath'];
        $this->full_path =  $data['absolute_path'];
        $this->full_path_with_file =  $data['absolute_path_with_file'];
        $this->test_folder_and_file = $data['relative_path'];
        $this->relative_path = $data['relative_path'];

        $this->get_file_info();
        $file_object = $this->setFileObject();
        return $file_object;
    }

    /**
     * @param array $params
     *   get the service path / arg
     *   filename
     *   module name
     * @param $path
     *   path of the module
     * @return fileObject
     */
    protected function buildFileObjectFromModule(array $params, $path){
        $service_path_full = $params['service_path'];
        $test_folder_and_test_file_name = array_slice($service_path_full, 4);
        $test_folder = array_slice($test_folder_and_test_file_name, 0, -1);
        $this->root_folder = $path;
        $this->full_path =  DRUPAL_ROOT . '/' . $this->root_folder . '/' . implode('/', $test_folder);
        $this->full_path_with_file =  $this->full_path . '/' . $this->filename;
        //Final Steps to read file and tags
        $this->test_folder_and_file = implode('/', $test_folder_and_test_file_name);
        $this->get_file_info();
        $file_object = $this->setFileObject();
        return $file_object;
    }

    public function buildFileObjectFromBehatTests(array $params, $path) {
        $service_path_full = $params['service_path'];
        $test_folder_and_test_file_name = array_slice($service_path_full, 4);
        $this->test_folder_and_file = implode('/', $test_folder_and_test_file_name);
        $test_folder = array_slice($test_folder_and_test_file_name, 0, -1);
        $this->root_folder = file_build_uri("/$path/");
        $this->full_path =  drupal_realpath($this->root_folder);
        $this->full_path_with_file =  $this->full_path . '/' . $this->filename;
        $this->relative_path =  file_create_url($this->root_folder . '/' . $this->test_folder_and_file);
        $this->get_file_info();
        $file_object = $this->setFileObject();
        return $file_object;
    }

    /**
     * Replaces fileObjectBuilder
     */
    protected function setFileObject() {
        $file_object = array();
        $file_object['absolute_path_with_file'] = $this->full_path_with_file;
        $file_object['absolute_path'] = $this->full_path;
        $file_object['relative_path'] = $this->relative_path;
        $file_object['filename'] = $this->filename;
        $file_object['subpath'] = FALSE;
        $file_object['scenario'] = $this->file_text;
        $file_object['filename_no_ext'] = substr($this->filename, 0, -8);
        $file_object['tags_array'] = $this->tags_array;
        $file_object['module'] = $this->module;
        return $file_object;
    }

    public function build_paths(){}

    public function save_html_to_file($scenario = array()) {
        $this->scenario = $scenario;
        $this->scenario_array = parent::_parse_questions();
        $this->file_text =  parent::_create_file();
        $output = $this->_figure_out_where_to_save_file();
        return $output;
    }

    public function output_file_text_to_html_array($file_text) {}

    public function get_file_info() {
        $this->file_text = $this->read_file($this->full_path_with_file);
        $this->tags_array = $this->_tags_array($this->file_text, $this->module);
    }

    public function delete_file() {}

    protected function _save_file_to_module_folder() {}

    protected function _linkable_path() { }

    protected function _save_path() {}

    protected function _save_file_to_temp_folder() { }

    protected function _turn_file_to_array($file) {}

    protected function _string_type($string, $scenario, $count, $direction){}

    protected function _string_types() {}

    protected function behat_editor_string_feature($string, $scenario, $count, $direction) {}

    protected function behat_editor_string_scenario($string, $scenario, $count, $direction) {}

    protected function behat_editor_string_background($string, $scenario, $count, $direction) {}

    protected function _parse_tags($scenario_array) {}

    protected function _string_tags($scenario, $count, $spaces = 0, $direction) {}

    protected function behat_editor_string_steps($string, $parent, $count, $direction) {}

    protected function _pop_first_word($string){}

    protected function _question_wrapper($string) {}

    protected function _new_line($new_line) {}

    protected function _spaces($spaces) {}

    public static function fileObjecBuilder() {}
}