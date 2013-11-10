<?php

use Drupal\BehatEditor;

function behat_editor_bulk() {
    $output = drupal_get_form('behat_editor_bulk_form_modules');
    return $output;
}

function behat_editor_bulk_form_modules($form, &$form_state) {
    composer_manager_register_autoloader();
    //Test Subfolder
    $test = new BehatEditor\Files(drupal_map_assoc(array('behat_tests')), 'bulk1');

    $build = array();

    $build['overview'] = array(
        '#prefix' => '<div>',
        '#markup' => t("Choose which modules to run the tests from."),
        '#suffix' => '</div>',
    );

    $header = array(
        'machinename' => array('data' => t('Module Name')),
        'nicename' => array('data' => t('Nice Name')),
        'folder' => array('data' => t('Folder')),
        'subfolder' => array('data' => t('SubFolder')),
        'last_run' => array('data' => t('Last Run')),
        'status' => array('data' => t('Status')),
        'pass_fail' => array('data' => t('Pass / Fail')),
    );

    $all = new BehatEditor\Files(array(), FALSE, $cache = FALSE);
    //So I can pull out the nice name
    $modules = BehatEditor\Files::getModuleFolders();
    $modules = array_merge($modules, BehatEditor\Files::_hasTestFolderArray());

    $rows = array();
    foreach($all->getFilesArray() as $key => $value) {
        foreach($value as $possible_row) {
            $folder = BEHAT_EDITOR_FOLDER;
            $array_key = $key;
            if($possible_row['subpath'] !== FALSE) {
                $subfolder = $possible_row['subpath'];
                $array_key = $array_key . '|' . $subfolder;
            } else {
                $subfolder = '';
            }

            $row['machinename'] = $key;
            $row['nicename'] = $modules[$possible_row['module']]['nice_name'];
            $row['folder'] = $folder;
            $row['subfolder'] = $subfolder;
            $row['last_run'] = 'coming soon..';
            $row['status'] = 'coming soon..';
            $row['pass_fail'] = 'coming soon..';
            $rows["$array_key"] = $row;
        }
    }

    dpm($rows);

    $build['dashboard'] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $rows,
        '#empty' => t('No Tests Files'),
    );

    return $build;
}


function behat_editor_bulk_tags() {
    $output = drupal_get_form('behat_editor_bulk_form_tags');
    return $output;
}


function behat_editor_bulk_form_tags($form, &$form_state) {
    $build = array();

    $build['overview'] = array(
        '#prefix' => '<div>',
        '#markup' => t("Choose which tags to run the tests for."),
        '#suffix' => '</div>',
    );
    composer_manager_register_autoloader();
    $test = new BehatEditor\Files(drupal_map_assoc(array('behat_tests')), 'bulk1');
    dpm($test->getFilesArray());

    $build['fieldset'] = array(
        '#type' => 'fieldset',
        '#title' => "Comming soon..."
    );

    return $build;
}