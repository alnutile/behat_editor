<?php
/**
 * @file
 * Contains \Drupal\BehatEditor\File.
 */

namespace Drupal\BehatEditor;

use Drupal\BehatEditor;

/**
 * Class File
 * Methods needed to run a process the test file.
 *
 */
class FileController {
    protected  $module;
    protected $filename;
    protected $parse_type;
    protected $scenario_array = array();
    protected $scenario;
    protected $feature;
    protected $action;
    protected $subpath;
    protected $service_path;
    protected $relative_path;
    protected $full_path_with_file;
    protected $full_path;
    protected $relative_path_with_no_file_name;
    protected $relative_path_with_file;
    protected $file_text;
    protected $tags_array = array();
    protected $root_folder;
    protected $test_folder_and_file;
    public $file_object;

    const BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER = 'behat_features';

    /**
     * @param array $params
     *  filename
     *  path starting at module or root folder ege behat_tests
     *  module eg root folder
     *
     */
    public function __construct($params = array()) {}

    public function build_paths(){}

    public function create($params = array()) {
        $file = new FileModel($params);
        $file_output = $file->createFile();
        if(is_array($file_output)) {
            $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
            $output = array('message' => t('@date: <br> File !name was created !link to download it ', array('@date' => $date, '!name' => $file_output['filename'], '!link' => l('click here', $file_output['relative_path']))), 'file' => $file_output['relative_path'], 'data' => $file_output, 'error' => '0');

        } else {
            $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
            $output = array('message' => t('@date: <br> File could not be created please check the logs ', array('@date' => $date)), 'file' => null, 'data' => array(), 'error' => '1');
        }
        return $output;
    }

    public function save($params = array()) {
        $this->module = $params['module'];
        $this->filename = $params['filename'];
        $file = new FileModel($params);
        return $file->save();
    }

    public function index() {
        $params = array();
        $files = new FileModel($params);
        $files_array = $files->getAllFiles();
        return $files_array;
    }

    public function show($params = array()) {
        $file = new FileModel($params);

        return $file->getFile();
    }

    public function delete($params = array()) {
        $this->module = $params['module'];
        $this->filename = $params['filename'];
        $file = new FileModel($params);
        $output = array();
        $response = $file->deleteFile();
        if($response == FALSE) {
            watchdog('behat_editor', "File could not be deleted...", $variables = array(), $severity = WATCHDOG_ERROR, $link = NULL);
            $output = array('message' => "Error file could not be deleted", 'file' => $response, 'error' => '1');
        } else {
            $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
            watchdog('behat_editor', "%date File deleted %name", $variables = array('%date' => $date, '%name' => $this->filename), $severity = WATCHDOG_NOTICE, $link = $this->filename);
            $output =  array('message' => t('@date: <br> File deleted !name to download ', array('@date' => $date, '!name' => $this->filename)), 'file' => $this->filename, 'error' => '0');
        }
        return $output;
    }

    protected function _save_file_to_absolute_path(){
        $this->full_path_with_file = $this->file_object['absolute_path_with_file'];
        $this->filename = $this->file_object['filename'];
        $this->relative_path = $this->file_object['relative_path'];
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
}