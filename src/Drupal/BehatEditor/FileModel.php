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
    protected $modules;
    protected $clone;
    protected $module_path;
    protected $action_path;
    protected $file_object;
    protected $service_path;
    public $file_data;

    public function __construct($params = array()) {
        $this->params = $params;
    }

    public function createFile(){
        $this->module = $this->params['module'];
        $this->clone = $this->params['clone'];
        $this->filename = $this->params['filename'];
        $this->scenario = $this->params['scenario'];
        $this->addParentTag();
        $this->parse_type = $this->params['parse_type'];
        $this->action = $this->params['action'];
        $this->action_path = $this->params['action'];
        $this->service_path_full = $this->params['service_path'];
        $this->full_path_with_file = $this->set_absolute_path() . '/' . $this->filename;
        $this->full_path = $this->set_absolute_path();
        $this->relative_path = file_create_url($this->root_folder . '/' . $this->filename);
        $this->scenario_array = $this->_parse_questions();
        $this->file_text =  $this->_process_text();
        $this->_save_file_to_absolute_path();
        //Want to return a full file object
        $this->get_file_info();
        return $this->file_data;
    }



    public function save(){
        $output = array();
        $this->service_path_full = $this->params['service_path'];
        $this->getFile();
        $this->scenario = $this->params['scenario'];
        $this->parse_type = $this->params['parse_type'];
        //@todo use params type to decide on html or array process of file
        $this->scenario_array = $this->_parse_questions();
        $this->file_text =  $this->_process_text();
        $output = $this->_save_file_to_absolute_path();
        return $output;
    }

    protected function set_absolute_path() {
        $trim_file_name_from_path = array_slice($this->service_path_full, 0, -1);
        $path = implode('/', $trim_file_name_from_path);
        $this->root_folder = file_build_uri($path);
        return drupal_realpath($this->root_folder);
    }

    public function deleteFile(){
        $path = drupal_get_path('module', $this->params['module']);
        if(!empty($path)) {
//            drupal_set_message(t('File could not be deleted it is part of a module'), 'error');
//            drupal_goto('admin/behat/index');
            $message = t('The file could not be saved !file because it is a module', array('!file' => $this->full_path_with_file . '/' . $this->filename));
            $output = array('message' => $message, 'file' => null, 'data' => null, 'error' => '1');
            return $output;
        }
        $this->service_path_full = $this->params['service_path'];
        $this->getFile();
        $response = file_unmanaged_delete($this->full_path_with_file);

        if($response == FALSE) {
            $message = t('The file could not be saved !file', array('!file' => $this->full_path_with_file . '/' . $this->filename));
            $output = array('message' => $message, 'file' => null, 'data' => null, 'error' => '1');
            //throw new \RuntimeException($message);
        } else {
            $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
            $output = array('message' => t('@date: <br> File deleted !name ', array('@date' => $date, '!name' => $this->filename)), 'file' => null, 'data' => $this->file_data, 'error' => '0');
        }

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
            $this->buildFileObjectFromModule($path);
            return $this->file_data;
        } else {
            $this->buildFileObjectFromPublic();
            return $this->file_data;
        }
    }

    public function getAllFiles(){
        //@todo add cache back to this
        //Modules first
        $files_array = $this->_buildModuleFilesArray();
        //public://behat_tests next
        $files_array_others = $this->_buildArrayOfAvailableFilesInPublicFolders();
        return array_merge($files_array_others, $files_array);
    }


    public function getAllModuleFiles(){
        //@todo add cache back to this
        //Modules first
        $files_array = $this->_buildModuleFilesArray();
        //public://behat_tests next
        return $files_array;
    }

    public function getFilesByTag(array $tag) {
        $files_found = array();
        $files_pre = $this->getAllFiles();
        foreach($files_pre as $key => $value) {
            foreach($value as $key2 => $value2) {
                //Some tags had ending string so had to
                if(isset($value2['tags_array'])) {
                    foreach($value2['tags_array'] as $tag_key => $tag_value) {
                        if(in_array(trim($tag_value), $tag)) {
                            $files_found[$key2] = $value2;
                        }
                    }
                }
            }
        }
        return $files_found;
    }

    protected function _buildModuleFilesArray() {
        if(empty($this->modules)) {
            $this->modules = $this->_checkForModules();
        }
        $files_array = $this->_buildArrayOfAvailableFilesInModulessFolders();
        return $files_array;
    }

    protected static function _hasTestFolderArray() {
        return array(
            'behat_tests' => array(
                'exists' => 1,
                'writable' => 1,
                'nice_name' => 'Behat Tmp Folder'
            )
        );
    }

    protected function _checkForModules() {
        //@todo turn cache back on
//        if( $this->cache !== FALSE ) {
//            if($cached = cache_get('behat_editor_modules', 'cache')) {
//                return $cached->data;
//            } else {
//                $module_array = $this->getModuleFolders();
//                if($this->cache != FALSE) {
//                    cache_set('behat_editor_modules', $module_array, 'cache', CACHE_TEMPORARY);
//                }
//            }
//        } else {
//            $module_array = $this->getModuleFolders();
//        }

        $module_array = $this->getModuleFolders();
        return $module_array;

    }

    protected static function getModuleFolders() {
        $module_array = array();
        $modules = module_list();
        foreach ($modules as $module) {
            $path = drupal_get_path('module', $module);
            if ($status = self::_hasFolder($module, $path)) {
                $module_array[$module] = $status;
            }
        }
        return $module_array;
    }

    private static function _hasFolder($module, $path, $subpath = FALSE) {
        $status = array();
        $full_path = $path . '/' . BEHAT_EDITOR_FOLDER;
        if($subpath) {
            $full_path = $full_path . '/' . $subpath;
        }
        if(drupal_realpath($full_path)) {
            $status['exists'] = TRUE;
            $status['writable'] = (is_writeable($full_path)) ? TRUE : FALSE;
            $nice_name = system_rebuild_module_data();
            $status['nice_name'] = $nice_name[$module]->info['name'];

            return $status;
        }
    }

    protected function _buildArrayOfAvailableFilesInPublicFolders() {
        $files_found = array();
        $file_data = array();
        $machine_name = 'behat_tests';
        $sub_folder = BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER;
        $service_path = "behat_tests";

        $files_folder =  file_build_uri("/{$sub_folder}/");
        $path = drupal_realpath($files_folder);
        $files = file_scan_directory($path, '/.*\.feature/', $options = array('recurse' => TRUE), $depth = 0);
        foreach($files as $file_key => $file_value) {

            $array_key =$file_value->uri;
            $filename = $file_value->filename;
            $file_uri_array = explode('/', $file_value->uri);
            $service_path = array_slice($file_uri_array, array_search('behat_tests', $file_uri_array));
            $params = array(
                'filename' => $filename,
                'module' => $machine_name,
                'parse_type' => 'file',
                'service_path' => $service_path
            );
            $this->params = $params;
            $file_data[$array_key] = $this->getFile();
        }
        $files_found[$machine_name] = $file_data;
        drupal_alter('behat_editor_files_found', $files_found, $context1 = 'public');
        return $files_found;
    }

    protected function _buildArrayOfAvailableFilesInModulessFolders() {
        $files_found = array();
        $file_data = array();
        foreach($this->modules as $machine_name => $nice_name) {
            //@todo allow subfolders
            $module_path =  drupal_get_path('module', $machine_name);
            $path = DRUPAL_ROOT . '/' . $module_path . '/' . BEHAT_EDITOR_FOLDER;
            $files = file_scan_directory($path, '/.*\.feature/', $options = array('recurse' => TRUE), $depth = 0);
            foreach($files as $file_key => $file_value) {
                $array_key =$file_value->uri;
                $filename = $file_value->filename;
                $full_service_path_string = '/' . $module_path . '/' . BEHAT_EDITOR_FOLDER . '/' . $filename;
                $exploded_path = explode('/', $full_service_path_string);
                $full_service_path_array = array_slice($exploded_path, 5);

                $params = array(
                    'filename' => $filename,
                    'module' => $machine_name,
                    'parse_type' => 'file',
                    'service_path' => $full_service_path_array /* @todo this can be a subfolder issue */
                );
                $this->params = $params;
                $file_data[$array_key] = $this->getFile();
            }
            $files_found[$machine_name] = $file_data;
        }
        drupal_alter('behat_editor_files_found', $file_data, $context1 = 'module');
        return $files_found;
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
            $output = array('message' => t('@date: <br> File created !name to download ', array('@date' => $date, '!name' => $file_url)), 'file' => $file_url, 'data' => $this->getFile(), 'error' => '0');
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
            if($results = $this->_string_type(trim($value), $scenario, $count, $direction)) {
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
     * @param $string
     * @param $scenario
     * @param $count
     * @param $direction
     * @return mixed
     */
    protected function _string_type($string, $scenario, $count, $direction){
        $compare = $this->_string_types();
        foreach($compare as $key) {
            if ($results = self::$key($string, $scenario, $count, $direction)) {
                return $results;
            }
        }
    }


    /**
     * Different functions used to parse the file
     * Later we can add scenario_outline, background etc.
     *
     * @return array
     */
    protected function _string_types() {
        $options = array('behat_editor_string_feature', 'behat_editor_string_scenario', 'behat_editor_string_background', 'behat_editor_string_steps');
        return $options;
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
    protected function buildFileObjectFromModule($path){
        $service_path_array = $this->params['service_path'];
        $service_path_array_minus_module_name = implode('/', array_slice($service_path_array, 1));
        $service_path_full_string = implode('/', $service_path_array);
        $this->action_path = '/'. $service_path_full_string;
        $test_folder_no_filename = array_slice($service_path_array, 1, -1);
        $this->module_path = $path;
        $this->full_path =  DRUPAL_ROOT . '/' . $this->module_path . '/' . implode('/', $test_folder_no_filename);
        $this->full_path_with_file =  DRUPAL_ROOT . '/' . $this->module_path . '/' . $service_path_array_minus_module_name;
        //Final Steps to read file and tags
        $this->test_folder_and_file = implode('/', $service_path_array);
        $this->get_file_info();
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
        $this->action_path = '/' . $this->test_folder_and_file;
        $service_path_full_no_file_name = array_slice($test_folder_and_test_file_name, 0, -1);
        $service_path_full_no_file_name_string = implode('/', $service_path_full_no_file_name);
        $this->root_folder = file_build_uri("/$service_path_full_no_file_name_string/");
        $this->full_path =  drupal_realpath($this->root_folder);
        $this->full_path_with_file =  $this->full_path . '/' . $this->filename;
        $this->relative_path =  file_create_url($this->root_folder . '/' . $this->filename);
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
            //drupal_goto('admin/behat/index');
        } else {
            $file_text = self::read_file();
            $file_data = array(
                'module' => $this->module,
                'filename' => $this->filename,
                'absolute_path' => $this->full_path,
                'absolute_path_with_file' => $this->full_path_with_file,
                'scenario' => $file_text,
                'service_path' => $this->service_path_full,
                'filename_no_ext' => substr($this->filename, 0, -8),
                'relative_path' => $this->relative_path,
                'subpath' => $this->subpath,
                'tags_array' => self::_tags_array($file_text, $this->module),
                'action_path' => $this->action_path
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
    public function _tags_array($file, $module_name) {
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
        $file_object['service_path'] = $this->service_path_full;
        $file_object['scenario'] = $this->file_text;
        $file_object['filename_no_ext'] = substr($this->filename, 0, -8);
        $file_object['tags_array'] = $this->tags_array;
        $file_object['module'] = $this->module;
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

    /**
     * Search for the Feature text
     *
     * @param $string
     * @param $scenario
     * @param $count
     * @param $direction
     * @return array
     */
    protected function behat_editor_string_feature($string, $scenario, $count, $direction) {
        $help =  _behat_editor_make_help_link('tags.html');

        $results = array();
        $first_word = self::_pop_first_word($string);
        $options = array('Feature:');
        if(in_array($first_word, $options)) {
            switch($direction) {
                case 'file':
                    $tags = array();
                    $tags[0] = self::_string_tags($scenario, $count - 1, 0, $direction);
                    $feature_line[1] = array(
                        'string' => $string,
                        'spaces' => 0,
                        'new_line' => 0,
                        'new_line_above' =>  0,
                    );
                    $results['feature'] = $tags + $feature_line;
                    return $results;
                case 'html_view':
                    $tags = array();
                    $tags[0] = self::_string_tags($scenario, $count - 1, 0, $direction);
                    $feature_line[1] = array(
                        'data' => $string,
                        'class' => array('feature', "spaces-none")
                    );
                    $results['feature'] = $tags + $feature_line;
                    return $results;
                case 'html_edit':
                    $tags = array();
                    $tags = self::_string_tags($scenario, $count - 1, 0, $direction);
                    //@todo remove number key should be automatic
                    $features_tags[0] = array(
                        'data' => "<strong>Feature Tags: $help</strong>",
                        'class' => array('ignore'),
                    );

                    $features_tag_input[1] = array(
                        'data' => array('features_tag_value' => array('#id' => 'features-tagit-values', '#type' => 'hidden', '#name' => 'features_tag_value', '#value'=>$tags)),
                        'class' => array('tag hidden'),
                        'id' => 'features-tags'
                    );

                    $features_tag_it[2] = array(
                        'data' => '<ul id="features-tagit-input"></ul><div class="help-block">Start each tag with @. Just separate by comma for more than one tags. Tags can not have spaces.</div>',
                        'class' => array('ignore'),
                    );

                    $feature_line[3] = array(
                        'data' => $string,
                        'class' => array('feature')
                    );
                    $results['feature'] = $features_tags + $features_tag_input + $features_tag_it + $feature_line;
                    return $results;
            }
        }
    }

    /**
     * Search for the Scenario Text
     *
     * @param $string
     * @param $scenario
     * @param $count
     * @param $direction
     * @return array
     */
    protected function behat_editor_string_scenario($string, $scenario, $count, $direction) {
        $results = array();
        $first_word = self::_pop_first_word($string);
        $options = array('Scenario:');
        drupal_alter('behat_editor_string_feature', $options);
        if(in_array($first_word, $options)) {
            switch($direction) {
                case 'file':
                    $tags = array();
                    $tags[0] = self::_string_tags($scenario, $count - 1, 2, $direction);
                    $scenario_line[1] = array(
                        'string' => $string,
                        'spaces' => 2,
                        'new_line' => 0,
                        'new_line_above' =>  0,
                    );
                    $results['scenario'] = $tags + $scenario_line;
                    return $results;
                case 'html_view':
                    $tags = array();
                    $tags[0] = self::_string_tags($scenario, $count - 1, 2, $direction);
                    $scenario_line[1] = array(
                        'data' => $string,
                        'class' => array("spaces-two")
                    );
                    $results['scenario'] = $tags + $scenario_line;
                    return $results;
                case 'html_edit':
                    $tags = array();
                    $tags = self::_string_tags($scenario, $count - 1, 2, $direction);
                    $uid = rand(100000000, 900000000);
                    $scenario_tag_input[0] = array(
                        'data' => array("scenario-tags-$uid" => array('#class' => 'section-tag', '#id' => "scenario-values-$uid", '#type' => 'hidden', '#value' => $tags)),
                        //'data' => array('features_tag_value' => array('#id' => "scenario-values-$uid", '#type' => 'hidden', '#name' => 'features_tag_value', '#value'=>$tags)),
                        'class' => array('tag')
                    );
                    $scenario_tag_it[2] = array(
                        'data' => '<i class="glyphicon glyphicon-move pull-left"></i><ul id="scenario-input-' . $uid . '" class="tagit" data-scenario-id="'.$uid.'"></ul>',
                        'class' => array('ignore'),
                    );
                    $scenario_line[3] = array(
                        'data' => self::_question_wrapper($string),
                        'class' => array('name'),
                        'data-scenario-tag-box' => "scenario-values-$uid"
                    );
                    $results['scenario'] = $scenario_tag_input + $scenario_tag_it + $scenario_line;
                    return $results;
            }

        }
    }


    /**
     * Search for the Background Text
     * http://docs.behat.org/guides/1.gherkin.html#backgrounds
     *
     * @param $string
     * @param $scenario
     * @param $count
     * @param $direction
     * @return array
     */
    protected function behat_editor_string_background($string, $scenario, $count, $direction) {
        $results = array();
        $first_word = self::_pop_first_word($string);
        $options = array('Background:');
        drupal_alter('behat_editor_string_feature', $options);
        if(in_array($first_word, $options)) {
            switch($direction) {
                case 'file':
                    $scenario_line[1] = array(
                        'string' => $string,
                        'spaces' => 2,
                        'new_line' => 0,
                        'new_line_above' =>  0,
                    );
                    $results['background'] = $scenario_line;
                    return $results;
                case 'html_view':
                    $scenario_line[1] = array(
                        'data' => $string,
                        'class' => array("spaces-two")
                    );
                    //$results['background'] = $tags + $scenario_line;
                    $results['background'] = $scenario_line;
                    return $results;
                case 'html_edit':
                    $scenario_line[0] = array(
                        'data' => self::_question_wrapper($string),
                        'class' => array('name'),
                    );
                    $results['background'] = $scenario_line;
                    return $results;
            }

        }
    }

    /**
     * Search for Tags in Scenario
     *
     * @param $scenario_array
     * @return array
     */
    protected function _parse_tags($scenario_array) {
        $tags = array();
        foreach($scenario_array as $key => $value) {
            if(strpos('@', $value)) {
                $string = str_replace(',', '', $value);
                $tags[] = explode(' ', $string);
            }
        }
        return $tags;
    }

    /**
     * Search for tags when
     * searching for Features and Scenario lines
     *
     * @param $scenario
     * @param $count
     * @param int $spaces
     * @param $direction
     * @return array|mixed
     */
    protected function _string_tags($scenario, $count, $spaces = 0, $direction) {

        if(array_key_exists($count, $scenario)) {
            $string = $scenario[$count];
            $options = array('@');
            foreach($options as $key => $value) {
                if(strpos($string, $value) !== false) {
                    switch($direction) {
                        case 'file':
                            $string = str_replace(',', ' ', $string);
                            return array(
                                'string' => $string,
                                'spaces' => $spaces,
                                'new_line' => 0,
                                'new_line_above' => ($count > 1) ? 1 : 0,
                            );
                        case 'html_view':
                            $results = array(
                                'data' => $string,
                                'class' => array('tag', "spaces-$spaces")
                            );
                            return $results;
                        case 'html_edit':
                            return str_replace(' ', ', ', $string);
                    }
                }
            }
        }
    }

    /**
     * Parse Steps
     *
     * @param $string
     * @param $parent
     * @param $count
     * @param $direction
     * @return array
     */
    protected function behat_editor_string_steps($string, $parent, $count, $direction) {
        $first_word = self::_pop_first_word($string);
        $options = array('Given', 'When', 'Then', 'And', 'But');
        drupal_alter('behat_editor_string_steps', $options);
        if(in_array($first_word, $options)) {
            switch($direction) {
                case 'file':
                    return array(
                        'string' => $string,
                        'spaces' => 4,
                        'new_line' => 0,
                        'new_line_above' => 0
                    );
                case 'html_view':
                    return  array(
                        'data' => $string,
                        'class' => array('steps', "spaces-four")
                    );
                case 'html_edit':
                    return  array(
                        'data' => self::_question_wrapper($string),
                        'class' => array('steps', "spaces-four")
                    );
            }
        }
    }

    /**
     * Get the first word from a line
     *
     * @param $string
     * @return mixed
     */
    protected function _pop_first_word($string){
        $first_word = explode(' ', $string);
        return array_shift($first_word);
    }

    /**
     * Wrap the editable questions in the close string
     *
     * @param $string
     * @return string
     */
    protected function _question_wrapper($string) {
        return '<i class="glyphicon glyphicon-move pull-left"></i>' . $string . '<i class="remove glyphicon glyphicon-remove-circle"></i>';
    }


    /**
     * Make HTML array from a file
     *
     * @param $file_text
     * @return array
     */
    public function output_file_text_to_html_array($params) {
        if(!isset($this->parse_type) && isset($params['parse_type'])) {
            $this->parse_type = $params['parse_type'];
        }
        $this->scenario = self::_turn_file_to_array($params['file_text']);
        $this->scenario_array = self::_parse_questions();
        return $this->scenario_array;
    }

    protected function addParentTag() {
        if(is_array($this->clone)) {
            if(strpos($this->scenario[0], '@') !== FALSE) {
                $parent = $this->_build_parent_tag($this->clone);
                $this->scenario[0] = $this->scenario[0] . ' ' . $parent;
            }
        }
    }

    protected function _build_parent_tag($clone) {
        $clone = array_slice($clone, 4);
        $clone = implode(':', $clone);
        $parent = "@parent:$clone";
        $this->_setAllowedTags($parent);
        return $parent;
    }

    private function _setAllowedTags($parent) {
        if ( module_exists('behat_editor_limit_tags')) {
            $tags = variable_get('behat_editor_limit_tags_allowed_tags', array());
            array_push($tags, $parent);
            variable_set('behat_editor_limit_tags_allowed_tags', $tags);
        }
    }


} 