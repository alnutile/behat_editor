<?php
/**
 * Created by PhpStorm.
 * User: alfrednutile
 * Date: 12/18/13
 * Time: 11:28 AM
 */

namespace Drupal\BehatEditor;

use Drupal\BehatEditor;

class FileModel {
    protected $module;
    protected $filename;
    protected $parse_type;
    protected $scenario_array = array();
    protected $scenario;
    protected $feature;
    protected $subpath;
    protected $relative_path;
    protected $full_path_with_file;
    protected $full_path;
    protected $relative_path_with_no_file_name;
    protected $relative_path_with_file;
    protected $file_text;
    protected $tags_array = array();
    protected $root_folder;
    protected $test_folder_and_file;
    protected $file_helpers;
    protected $service_path_full;
    protected $params;
    protected $file_object;
    protected $service_path;
    protected $file_data;

    public function __construct($params = array()) {
        $this->params = $params;
    }

    public function save(){
        $output = array();
        $this->service_path_full = $this->params['service_path'];
        $this->file_object = $this->getFile();
        watchdog('test_save', print_r($this->file_object, 1));
//        $this->scenario = $this->file_object['scenario'];
//        //@todo use params type to decide on html or array process of file
//        $this->scenario_array = $this->_parse_questions();
//        $this->file_text =  $this->_process_text();
//        watchdog('test_file_text', print_r($this->file_text, 1));
//        $output = $this->_save_file_to_absolute_path();
        return $output;
    }

    /**
     * What folder to start in
     * module or public://
     *
     * @param array $params
     * @return array|fileObject
     */
    public function getFile() {
        $this->module = $this->params['module'];
        $this->filename = $this->params['filename'];
        if(isset($this->params['parse_type'])) {
            $this->parse_type = $this->params['parse_type'];
        }
        $path = drupal_get_path('module', $this->module);
        if(!empty($path)) {
            $file_object = $this->buildFileObjectFromModule($path);
            return $file_object;
        } else {
            $this->buildFileObjectFromPublic();
            return $this->file_data;
        }
    }


    protected function _save_file_to_absolute_path(){
        $output = array();
        $response = file_unmanaged_save_data($this->file_text, $this->full_path_with_file, $replace = FILE_EXISTS_REPLACE);
        if($response == FALSE) {
            $message = t('The file could not be saved !file', array('!file' => $this->full_path_with_file . '/' . $this->filename));
            //throw new \RuntimeException($message);
        } else {
            $file_url = l('click here', $this->relative_path, array('attributes' => array('target' => '_blank', 'id' => array('test-file'))));
            $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
            $output = array('message' => t('@date: <br> File created !name to download ', array('@date' => $date, '!name' => $file_url)), 'file' => $file_url, 'error' => '0');
        }
        return $output;
    }

    /**
     * Format the file creation from the array
     *
     * @return string
     */
    protected function _process_text(){
        $file = '';
        foreach($this->scenario_array as $key) {
            $new_line = $this->_new_line($key['new_line']);
            $new_line_above = $this->_new_line($key['new_line_above']);
            $spaces = $this->_spaces($key['spaces']);
            $file = $file . "{$new_line_above}" . "{$spaces}" . $key['string'] . "{$new_line}\r\n";
        }
        return $file;
    }


    /**
     * New line parse
     *
     * @param $new_line
     * @return string
     */
    protected function _new_line($new_line) {
        if($new_line == 1) {
            return "\r\n";
        } else {
            return "";
        }
    }

    /**
     * Spaces needed to output the HTML or file to look
     * right.
     *
     * @param $spaces
     * @return string
     */
    protected function _spaces($spaces) {
        $spaces_return = '';
        for($i = 0; $i <= $spaces; $i++) {
            $spaces_return = $spaces_return . " ";
        }
        return $spaces_return;
    }



    /**
     * Turn the file into an array
     *
     * @return array
     */
    protected function _parse_questions(){
        $scenario_array = array();
        $count = 0;
        $direction = $this->parse_type;
        $scenario = array_values($this->scenario);
        foreach($scenario as $value) {
            if($results = $this->string_type(trim($value), $scenario, $count, $direction)) {
                if(array_key_exists('scenario', $results) || array_key_exists('feature', $results) || array_key_exists('background', $results)) {
                    $key = key($results);
                    foreach($results[$key] as $row) {
                        $scenario_array[] = $row;
                    }
                } else {
                    $scenario_array[] = $results;
                }
            }
            $count++;
        }

        return $scenario_array;
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
    protected function buildFileObjectFromModule(array $params){
        $path = drupal_get_path('module', $this->module);
        $service_path_full = $params['service_path'];
        $test_folder_and_test_file_name =  self::getNeededPath($service_path_full);
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


    /**
     * If the path is not rooted in modules then it is files public (@todo private)
     * the url / path params should provide the path from there
     * eg admin/behat/view/behat_tests/tests/test.feature
     * would be in sites/default/files/behat_tests/tests/test.feature
     *
     * @param array $params
     * @return array
     */
    public function buildFileObjectFromPublic() {
        $this->buildPaths();
        $this->get_file_info();
    }

    protected function buildPaths() {
        $test_folder_and_test_file_name = $this->params['service_path'];
        $this->test_folder_and_file = implode('/', $test_folder_and_test_file_name);
        $service_path_full_no_file_name = array_slice($test_folder_and_test_file_name, 0, -1);
        $service_path_full_no_file_name_string = implode('/', $service_path_full_no_file_name);
        $this->root_folder = file_build_uri("/$service_path_full_no_file_name_string/");
        $this->full_path =  drupal_realpath($this->root_folder);
        $this->full_path_with_file =  $this->full_path . '/' . $this->filename;
        $this->relative_path =  file_create_url($this->root_folder . '/' . $this->test_folder_and_file);
    }

    /**
     * Build out the file_object used in most functions.
     *
     * @return array
     */
    public function get_file_info() {
        if(file_exists($this->full_path_with_file) == FALSE) {
            $message = t('The file does not exist !file', array('!file' => $this->full_path_with_file));
            //throw new \RuntimeException($message);
            drupal_set_message($message, 'error');
            //drupal_goto('admin/index');
        } else {
            $file_text = self::read_file();
            $file_data = array(
                'module' => $this->module,
                'filename' => $this->filename,
                'absolute_path' => $this->full_path,
                'absolute_path_with_file' => $this->full_path_with_file,
                'scenario' => $file_text,
                'filename_no_ext' => substr($this->filename, 0, -8),
                'relative_path' => $this->relative_path,
                'subpath' => $this->subpath,
                'tags_array' => self::_tags_array($file_text, $this->module)
            );
            $this->file_data = array_merge( $this->fileObjecBuilder(), $file_data);
        }
    }



    /**
     * Build out a tags array of a file
     *
     * @param $file
     * @param $module_name
     * @return array
     */
    protected function _tags_array($file, $module_name) {
        $file_to_array = self::_turn_file_to_array($file);
        $tags = array();
        foreach($file_to_array as $key => $value) {
            if(strpos($value, '@') !== FALSE && !strpos($value, '"')) {
                foreach(explode(' ', $value) as $tag) {
                    if(!empty($tag)) {
                        $tags[] = trim($tag);
                    }
                }
            }
        }
        return $tags;
    }

    /**
     * Quick helper to turn a file into a simple array
     *
     * @param $file
     * @return array
     */
    protected function _turn_file_to_array($file) {
        $array = explode("\n", $file);
        foreach($array as $key => $value) {
            if(strlen($value) <= 1) {
                unset($array[$key]);
            }
        }
        return $array;
    }

    /**
     * Read file
     *
     * @param $full_path_with_file
     * @return string
     */
    public function read_file() {
        if(filesize($this->full_path_with_file) > 0) {
            $file_open = fopen($this->full_path_with_file, "r");
            $file_read = fread($file_open, filesize($this->full_path_with_file));
            return $file_read;
        }
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

    protected static function getNeededPath($service_path_full) {
        //return array_slice($service_path_full, 3);
        return $service_path_full;
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

        $this->root_folder = $data['subpath'];
        $this->full_path =  $data['absolute_path'];
        $this->full_path_with_file =  $data['absolute_path_with_file'];
        $this->test_folder_and_file = $data['relative_path'];
        $this->relative_path = $data['relative_path'];

        $this->get_file_info();
        $file_object = $this->setFileObject();
        return $file_object;
    }

    protected function fileExistsCheck(){
        if(!file_exists($this->full_path_with_file)) {
            $message = t('The file does not exist !file', array('!file' => $this->filename));
            drupal_set_message($message, 'error');
            //drupal_goto('admin/behat/index');
        }
    }

    public static function fileObjecBuilder() {
        composer_manager_register_autoloader();
        $path = drupal_get_path('module', 'behat_editor');
        $file_object['absolute_path_with_file'] = '';
        $file_object['absolute_path'] = '';
        $file_object['relative_path'] = '';
        $file_object['filename'] = '';
        $file_object['subpath'] = FALSE;
        $file_object['filename_no_ext'] = '';
        $file_object['tags_array'] = '';
        $file_object['module'] = '';
        return $file_object;
    }



} 