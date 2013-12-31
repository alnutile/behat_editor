<?php
use Drupal\BehatEditor;

/**
 * @file File for viewing the Tests
 * You can see the related js file as well in the js folder
 * /
 */

/**
 * @todo Work in the new layout for a show file using params et so subfolder work with the API
 * @param $module
 * @param $filename
 */

function behat_editor_view_service($module, $filename) {
    composer_manager_register_autoloader();

    $file = new BehatEditor\FileController();
    $service_path = explode("/", $module . "/" . $filename);
    $params = array(
        'service_path' => $service_path,
        'module' => $module,
        'filename' => $filename,
        'action' => 'view'
    );
    $file_info = $file->show($params);
    drupal_alter('behat_editor_view', $file_info);

    exit();
}