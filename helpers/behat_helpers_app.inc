<?php

/**
 * @return modules that have behat_feature folders
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
 * @param $module
 *   the module to check if it has a folder
 * @return array
 *   exists = TRUE/FALSE
 *   writable = TRUE/FALSE
 *   nice_name = Yup
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
            $files_found[$machine_name] = file_scan_directory($path, '/.*\.feature/', $options = array('recurse' => FALSE), $depth = 0);
        } else {
          $path = DRUPAL_ROOT . '/' . drupal_get_path('module', $machine_name) . '/' . BEHAT_EDITOR_FOLDER;
          $files_found[$machine_name] = file_scan_directory($path, '/.*\.feature/', $options = array('recurse' => FALSE), $depth = 0);
        }
    }

    return $files_found;
}

/*
 * @module
 *   the machine name
 * @filename
 *   the full file name
 * @return
 *  array of common file data
 */
function _behat_get_file_info($module, $filename) {
    if ($module == BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER) {
        $sub_folder = BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER;
        $files_folder =  file_build_uri("/{$sub_folder}/");
        $path = drupal_realpath($files_folder);
        $full_path_with_file = $path . '/' . $filename;
    } else {
        $sub_folder = drupal_get_path('module', $module) . '/' . BEHAT_EDITOR_FOLDER;
        $path = DRUPAL_ROOT . '/' . $sub_folder;
        $full_path_with_file = $path . '/' . $filename;
    }

    $file_data = array(
        'module' => $module,
        'filename' => $filename,
        'absolute_path' => $path,
        'absolute_path_with_file' => $full_path_with_file,
        'scenario' => _behat_editor_read_file($full_path_with_file),
        'filename_no_ext' => substr($filename, 0, -8)
    );
    return $file_data;
}


function _behat_editor_read_file($full_path_with_file) {
    $file_open = fopen($full_path_with_file, "r");
    $file_read = fread($file_open, filesize($full_path_with_file));
    return $file_read;
}