<?php


/**
 * @files returned from _behat_editor_build_array_of_available_files
 *
 * return table
 */

function _behat_editor_produce_table_array($files_array) {
    $rows = array();
    foreach($files_array as $key => $value) {
        foreach($value as $row) {
            $file_name = $row->filename;
            $module_name = $key;
            $rows[] = array(
                'data' =>
                array
                (
                    $file_name,
                    l('view', 'admin/behat/view/' . $module_name . '/' . $file_name),
                    l('edit', 'admin/behat/edit/' . $module_name . '/' . $file_name),
                    l('delete', 'admin/behat/delete/' . $module_name . '/' . $file_name)
                )
            );
        }
    };

    $header = array(
        'Name',
        'View',
        'Edit',
        'Delete',
    );

    $behat_table = theme('table', array(
        'header' => $header,
        'rows' => $rows,
        'attributes' => array('class' => array('table-hover'))
    ));

    return $behat_table;
}


function theme_behat_editor_file_to_html_view($vars) {
    $build['file_text'] = array(
        '#markup' => $vars['file_text']
    );
    return $build;
}


function template_preprocess_behat_editor_file_to_html_view(&$variables) {
    module_load_include('inc', 'behat_editor', 'helpers/behat_editor_file_parsing');
    $file_text = $variables['file_text'];
    $file_to_array = _behat_editor_turn_file_to_array($file_text);
    $array_to_html_array = _behat_editor_parse_questions($file_to_array, $direction = 'html_view');
    $make_it_html = theme('item_list', $vars = array('items' => $array_to_html_array));
    $variables['file_text'] = $make_it_html;
}