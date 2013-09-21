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

    $build['header'] = array(
        '#markup' => '<h3>Feature: ' . $filename . '</h3>'
    );

    $file_to_html = theme('behat_editor_file_to_html_view', $vars = array('file_text' => $file_info['scenario']));

    $build['question'] = array(
        '#prefix' => '<div class="well span6">',
        '#markup' => render($file_to_html['file_text']),
        '#suffix' => '</div>',
    );

    $build['results'] = array(
        '#prefix' => '<div class="span5"><h4>Results</h4>',
        '#markup' => theme('behat_editor_results_area'),
        '#suffix' => '</div>',
    );

    return $build;
}