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
    public $files_text = '';
    public $tags_array = array();

    public function __construct($params = array()) {}

    public function buildObject($params){
        $this->module = $params['module'];
        $this->filename = $params['filename'];
        $path = drupal_get_path('module', $this->module);
        if(!empty($path)) {
            //trim off the module name from the path
            $path_trimmed = explode('/', $path);
            $path_trimmed = array_slice($path_trimmed, 0, -1);
            $path_trimmed = implode('/', $path_trimmed);

            //@todo move the fine $this items down
            $this->get_file_info();
            $this->full_path =  DRUPAL_ROOT . '/' . $path_trimmed;
            $this->full_path_with_file = DRUPAL_ROOT . '/' . $path_trimmed . $params['relative_path_with_file_name'];

            $file_object = parent::fileObjecBuilder();
            $file_object['absolute_path_with_file'] = $this->full_path_with_file;
            $file_object['absolute_path'] = $this->full_path . $params['relative_path_with_no_file_name'];;
            $file_object['relative_path'] = '/' . $path_trimmed . $params['relative_path_with_file_name'];
            $file_object['filename'] = $this->filename;
            $file_object['subpath'] = FALSE;
            $file_object['scenario'] = $this->files_text;
            $file_object['filename_no_ext'] = substr($this->filename, 0, -8);
            $file_object['tags_array'] = $this->tags_array;
            $file_object['module'] = $this->module;
            return $file_object;
        } elseif($this->module == BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER) {
            //build out path based on behat_tests
        } else {
            //offer alter
            drupal_alter('behat_editor_build_path', $data);
        }


    }

    public function build_paths(){}

    public function save_html_to_file() {}

    public function output_file_text_to_html_array($file_text) {}

    public function get_file_info() {
        $this->file_text = parent::read_file($this->full_path_with_file);
        $this->tags_array = parent::_tags_array($this->files_text, $this->module);
    }

    public function delete_file() {}

    protected function _figure_out_where_to_save_file(){}

    protected function _save_file_to_module_folder() {}

    protected function _linkable_path() { }

    protected function _save_path() {}

    protected function _save_file_to_temp_folder() { }

    protected function _parse_questions() {}

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

    protected function _create_file(){}

    protected function _new_line($new_line) {}

    protected function _spaces($spaces) {}

    public static function fileObjecBuilder() {}
}