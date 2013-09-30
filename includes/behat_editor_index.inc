<?php

function behat_editor_index() {
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
    drupal_add_library('behat_editor', 'data_table');
    $path = drupal_get_path('module', 'behat_editor');
    drupal_add_js($path . '/js/behat_editor_index.js');

    $build['actions'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('span11', 'well'))
    );

    $build['actions']['add_test'] = array(
        '#type' => 'link',
        '#title' => t('Add Test'),
        '#href' => "admin/behat/add",
        '#attributes' => array('class' => array('btn', 'btn-success', 'add')),
    );

    $modules = _behat_editor_check_for_modules();
    $modules = array_merge($modules, _behat_editor_test_folder_array());
    $files_array = _behat_editor_build_array_of_available_files($modules);

    $behat_table =_behat_editor_produce_table_array($files_array);
    $build['behat_table'] = array(
        '#markup' => $behat_table
    );

    return $build;
}


function _behat_editor_test_folder_array() {
    return array(
        'behat_tests' => array(
            'exists' => 1,
            'writable' => 1,
            'nice_name' => 'Behat Tmp Folder'
        )
    );
}