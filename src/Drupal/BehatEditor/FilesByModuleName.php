<?php

namespace Drupal\BehatEditor;

class FilesByModuleName extends Files{
    public $files = array();
    public $subpath = '';

    public function __construct(array $modules, $subpath = FALSE, $cache = TRUE) {
        $this->subpath = $subpath;
        $this->files = self::_buildModuleFilesArray($modules);
    }

    private function _buildModuleFilesArray($modules) {
        $files_array = self::_buildArrayOfAvailableFiles($modules);
        return $files_array;
    }

    protected function _buildArrayOfAvailableFiles($modules) {
        $files_found = array();

        foreach($modules as $machine_name) {
            if($this->subpath) {
                $subpath = $this->subpath . '/';
            } else {
                $subpath = '';
            }
            if ($machine_name == BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER) {
                $sub_folder = BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER;
                $files_folder =  file_build_uri("/{$sub_folder}/{$subpath}");
                $path = drupal_realpath($files_folder);
                $files_found[$machine_name] = self::_behatEditorScanDirectories($machine_name, $path);
            } else {
                $path = DRUPAL_ROOT . '/' . drupal_get_path('module', $machine_name) . '/' . $subpath . BEHAT_EDITOR_FOLDER;
                $files_found[$machine_name] =  self::_behatEditorScanDirectories($machine_name, $path);
            }
        }
        return $files_found;
    }

    protected function _behatEditorScanDirectories($module, $path) {
        $file_data = array();
        $files = file_scan_directory($path, '/.*\.feature/', $options = array('recurse' => FALSE), $depth = 0);
        foreach($files as $key => $value) {
            $filename = $files[$key]->filename;
            $file = new File(array(), $module, $filename, 'file', $this->subpath);
            $file_data[$key] = $file->get_file_info();
        }
        return $file_data;
    }

}