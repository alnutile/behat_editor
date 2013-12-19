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
    public $module;
    public $filename;
    public $parse_type;
    public $scenario_array = array();
    public $scenario;
    public $feature;
    public $subpath;
    public $relative_path;
    public $full_path_with_file;
    public $full_path;
    public $relative_path_with_no_file_name;
    public $relative_path_with_file;
    public $file_text;
    public $tags_array = array();
    public $root_folder;
    public $test_folder_and_file;
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

    public function save($params = array()) {
        $this->module = $params['module'];
        $this->filename = $params['filename'];
        $file = new FileModel($params);
        return $file->save();
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