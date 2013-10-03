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

    public function __construct() {
        $this->files = self::_buildModuleFilesArray();
    }

    public function getFilesArray() {
        return $this->files;
    }

    private function _buildModuleFilesArray() {
        $modules = self::_checkForModules();
        $modules = array_merge($modules, self::_hasTestFolderArray());
        $files_array = self::_buildArrayOfAvailableFiles($modules);
        return $files_array;
    }

    private function _checkForModules() {
        if($cached = cache_get('behat_editor_modules', 'cache')) {
            return $cached->data;
        } else {
            $module_array = array();
            $modules = module_list();
            foreach ($modules as $module) {
                if ($status = self::_hasFolder($module)) {
                    $module_array[$module] = $status;
                }
            }
            cache_set('behat_editor_modules', $module_array, 'cache', CACHE_TEMPORARY);
            return $module_array;
        }
    }

    private function _hasFolder($module) {
        $status = array();
        $path = drupal_get_path('module', $module);
        $full_path = $path . '/' . BEHAT_EDITOR_FOLDER;
        if(drupal_realpath($full_path)) {
            $status['exists'] = TRUE;
            $status['writable'] = (is_writeable($full_path)) ? TRUE : FALSE;
            $nice_name = system_rebuild_module_data();
            $status['nice_name'] = $nice_name[$module]->info['name'];

            return $status;
        }
    }

    private function _hasTestFolderArray() {
        return array(
            'behat_tests' => array(
                'exists' => 1,
                'writable' => 1,
                'nice_name' => 'Behat Tmp Folder'
            )
        );
    }

    private function _buildArrayOfAvailableFiles($modules) {
        $files_found = array();
        foreach($modules as $machine_name => $nice_name) {
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

    private function _behatEditorScanDirectories($module, $path) {
            $file_data = array();
            $files = file_scan_directory($path, '/.*\.feature/', $options = array('recurse' => FALSE), $depth = 0);
            foreach($files as $key => $value) {
                $filename = $files[$key]->filename;
                $file = new File(array(), $module, $filename, 'file');
                $file_data[$key] = $file->get_file_info();
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