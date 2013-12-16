<?php

use Drupal\BehatEditor\File;

/*
 * @file helper functions used by different files
 *
 */

/**
 * Check for all the modules that have behat_features
 * folders
 *
 * @return array
 */
function _behat_editor_check_for_modules() {
    if($cached = cache_get('behat_editor_modules', 'cache')) {
        return $cached->data;
    } else {
        $module_array = array();
        $modules = module_list();
        foreach ($modules as $module) {
            if ($status = _behat_editor_has_folder($module)) {
                $module_array[$module] = $status;
            }
        }
        cache_set('behat_editor_modules', $module_array, 'cache', CACHE_TEMPORARY);
        return $module_array;
    }
}

/**
 * Helper to see if has behat_editor folder
 *
 * @param $module
 *   the module to check if it has a folder
 * @return array
 *   exists = TRUE/FALSE
 *   writable = TRUE/FALSE
 *   nice_name = module nice_name
 */
function _behat_editor_has_folder($module) {
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

/**
 * Helper function to make the array of
 * available files in the folder
 *
 * @param $modules
 *  return value of _behat_editor_has_folder
 * @return array
 *  and array keyed by module names of files folders etc.
 */
function _behat_editor_build_array_of_available_files($modules) {
    $files_found = array();
    foreach($modules as $machine_name => $nice_name) {
        if ($machine_name == BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER) {
            $sub_folder = BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER;
            $files_folder =  file_build_uri("/{$sub_folder}/");
            $path = drupal_realpath($files_folder);
            $files_found[$machine_name] = _behat_editor_scan_directories($machine_name, $path);
        } else {
          $path = DRUPAL_ROOT . '/' . drupal_get_path('module', $machine_name) . '/' . BEHAT_EDITOR_FOLDER;
          $files_found[$machine_name] =  _behat_editor_scan_directories($machine_name, $path);
        }
    }
    drupal_alter('behat_editor_files_found', $files_found);
    dpm($files_found);
    return $files_found;
}

/*
 * Helper function to scan directories
 *
 * @module
 *   the machine name
 * @filename
 *   the full file name
 * @return
 *  array of common file data
 */
function _behat_editor_scan_directories($module, $path) {
    composer_manager_register_autoloader();
    $file_data = array();
    $files = file_scan_directory($path, '/.*\.feature/', $options = array('recurse' => FALSE), $depth = 0);
    foreach($files as $key => $value) {
        $filename = $files[$key]->filename;
        $file = new File(array(), $module, $filename, 'file');
        $file_data[$key] = $file->get_file_info();
    }
    return $file_data;
}