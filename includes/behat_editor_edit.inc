<?php

/**
 * @param $module to get the path
 * @param $filename the filename to get that as well
 * @return content
 *
 * @todo my "controller" seems to have a ton of view related work?
 */

function behat_editor_edit_form($form, $form_state, $module, $filename) {
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
    drupal_add_library('system', 'ui.sortable');
    drupal_add_library('system', 'ui.autocomplete');
    drupal_add_library('behat_editor', 'tag_it') ;
    $file_info = _behat_get_file_info($module, $filename);
    //Product the HTML from the File in a format it will have all the markup needed to
    //  build / edit a test
    $file_to_html = theme('behat_editor_file_to_html_edit', $vars = array('file_text' => $file_info['scenario']));


    $path = drupal_get_path('module', 'behat_editor');
    $form['#attached']['js']['behat_editor_tags'] = $path . '/js/jquery.behat_editor.apply_tags.js';
    $form['#attached']['js']['behat_editor_close'] = $path . '/js/jquery.behat_editor.close_button.js';
    $form['#attached']['js']['behat_editor_app'] = $path . '/js/behat_editor_app.js';
    $form['#attached']['js']['behat_editor_add'] = $path . '/js/behat_editor_edit.js';
    $form['#attached']['js']['behat_editor_save'] = $path . '/js/behat_editor_save.js';
    $form['#attached']['js']['behat_editor_sbuilder'] = $path . '/js/behat_editor_scenario_builder.js';

    $form['#attached']['css']['behat_app_css'] = $path . '/css/behat_editor_app.css';
    $form['#attached']['css']['behat_add_css'] = $path . '/css/behat_editor_edit.css';
    $form['#attributes'] = array('class' => array('scenario-builder'));
    $form['header'] = array(
        '#markup' => '<h3>Editing: ' . $filename . '</h3>'
    );

    $form['actions'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('span11', 'well'))
    );

    $form['actions']['run_test'] = array(
        '#type' => 'link',
        '#title' => t('Run Test'),
        '#name' => 'run_test',
        '#href' => "admin/behat/run/$module/$filename",
        '#attributes' => array('class' => array('btn', 'btn-success', 'run')),
    );

    $form['actions']['save_test'] = array(
        '#type' => 'link',
        '#title' => t('Save Test'),
        '#name' => 'save_test',
        '#href' => "admin/behat/save/$module/$filename",
        '#attributes' => array('class' => array('btn', 'btn-info', 'save')),
    );

    $form['actions']['admin_area'] = array(
        '#type' => 'link',
        '#title' => t('Admin'),
        '#name' => 'admin_area',
        '#href' => "admin/behat/index",
        '#attributes' => array('class' => array('btn', 'btn-warning', 'back')),
    );

    $form['left-side'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('span5'))
    );

    $form['left-side']['intro'] = array(
        '#theme' => 'behat_editor_questions_area_intro',
        '#prefix' => '<div class="well">',
        '#suffix' => '</div>',
    );

    $form['results'] = array(
        '#markup' => theme('behat_editor_results_area', $args = array('params' => array()))
    );

    $form['left-side']['existing_test'] = array(
        '#prefix' => '<div class="well">',
        '#suffix' => '</div>',
        '#markup' => theme('behat_editor_view_questions_area', array('questions' => $file_to_html['file_text'], 'actions' => array()))
    );

    $form['left-side']['questions'] = _behat_editor_questions_form();


    return $form;
}

