<?php

namespace Drupal\BehatEditor;

use Drupal\BehatEditor;

/**
 * Class Files
 * @package Drupal\BehatEditor
 *
 * Find all tests and modules with tests
 * and build out the info needed by other modules
 *
 */
class Files {
    public $files = '';
    public $subpath = '';
    public $modules = array();
    public $cache = TRUE;

    public function __construct(array $modules = array(), $subpath = FALSE, $cache = TRUE) {
        $this->subpath = FALSE;
        $this->cache = $cache;
        $this->modules = $modules;
        $this->files = self::_buildModuleFilesArray();
    }

    public function getFilesArray() {
        return $this->files;
    }

    private function _buildModuleFilesArray() {
        if(empty($this->modules)) {
            $modules = self::_checkForModules();
            $this->modules = array_merge($modules, self::_hasTestFolderArray());
        }
        $files_array = self::_buildArrayOfAvailableFiles();
        return $files_array;
    }

    private function _checkForModules() {
        if($cached = cache_get('behat_editor_modules', 'cache')) {
            return $cached->data;
        } else {
            $module_array = self::getModuleFolders();
            //cache_set('behat_editor_modules', $module_array, 'cache', CACHE_TEMPORARY);
            return $module_array;
        }
    }

    public static function getModuleFolders() {
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

    public static function _hasTestFolderArray() {
        return array(
            'behat_tests' => array(
                'exists' => 1,
                'writable' => 1,
                'nice_name' => 'Behat Tmp Folder'
            )
        );
    }

    protected function _buildArrayOfAvailableFiles() {
        $files_found = array();
        foreach($this->modules as $machine_name => $nice_name) {
            if ($machine_name == BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER) {
                $sub_folder = BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER;
                $files_folder =  file_build_uri("/{$sub_folder}/");
                $path = drupal_realpath($files_folder);
                $files_found[$machine_name] = self::_behatEditorScanDirectories($machine_name, $path);
            } else {
                $path = DRUPAL_ROOT . '/' . drupal_get_path('module', $machine_name) . '/' . BEHAT_EDITOR_FOLDER;
                $files_found[$machine_name] =  self::_behatEditorScanDirectories($machine_name, $path);
            }
        }
        return $files_found;
    }

    protected function _behatEditorScanDirectories($module, $path) {
        $file_data = array();
        $files = file_scan_directory($path, '/.*\.feature/', $options = array('recurse' => TRUE), $depth = 0);
        foreach($files as $key => $value) {
            $subpath = $this->subpath;
            $array_key = $key;
            $found_uri = array_slice(explode('/', $files[$key]->uri), 0, -1); //remove file name
            $base_uri = explode('/', $path);
            if(count($found_uri) > count($base_uri)) {
                $subpath = array_slice($found_uri, count($base_uri), 1);
                $subpath = $subpath[0];
                $array_key = $array_key . $subpath;
            }
            $filename = $files[$key]->filename;
            $file = new File(array(), $module, $filename, 'file', $subpath);
            $file_data[$array_key] = $file->get_file_info();
        }
        return $file_data;
    }
/*
 *
 *  $modules = _behat_editor_check_for_modules();
    $modules = array_merge($modules, _behat_editor_test_folder_array());
    $files_array = _behat_editor_build_array_of_available_files($modules);
 */

}