<?php

/**
 * @param $module to get the path
 * @param $filename the filename to get that as well
 * @return content
 */

function behat_editor_view($module, $filename) {
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
    $file_info = _behat_get_file_info($module, $filename);
    $path = drupal_get_path('module', 'behat_editor');
    $build['#attached']['js']['behat_editor_app'] = $path . '/js/behat_editor_app.js';
    $build['#attached']['js']['behat_editor_run'] = $path . '/js/behat_editor_view.js';

    $build['#attached']['css']['behat_app_css'] = $path . '/css/behat_editor_app.css';
    $build['#attached']['css']['behat_view_css'] = $path . '/css/behat_editor_view.css';

    $build['header'] = array(
        '#markup' => '<h3>Feature: ' . $filename . '</h3>'
    );

    $file_to_html = theme('behat_editor_file_to_html_view', $vars = array('file_text' => $file_info['scenario']));

    $build['actions'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('span11', 'well'))
    );

    $build['actions']['run_test'] = array(
        '#type' => 'link',
        '#title' => t('Run Test'),
        '#name' => 'run_test',
        '#href' => "admin/behat/run/$module/$filename",
        '#attributes' => array('class' => array('btn', 'btn-success', 'run')),
    );

    $build['actions']['edit_test'] = array(
        '#type' => 'link',
        '#name' => 'edit_test',
        '#title' => t('Edit Test'),
        '#href' => "admin/behat/edit/$module/$filename",
        '#attributes' => array('class' => array('btn', 'btn-info', 'edit')),
    );

    $build['question'] = array(
        '#prefix' => '<div class="well span5">',
        '#suffix' => '</div>',
        '#markup' => theme('behat_editor_view_questions_area', array('questions' => $file_to_html['file_text'], 'actions' => array()))
    );

    $build['results'] = array(
        '#markup' => theme('behat_editor_results_area', $args = array('params' => array()))
    );

    return $build;
}