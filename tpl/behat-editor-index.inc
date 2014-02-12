<?php

/**
 * @file functions to show the Admin / Index page
 */

use Drupal\BehatEditor;

/**
 * Build the output for the Index page of files.
 * You can see the related js file as well in the js folder
 *
 * @todo turn this into a form so it will be easier to hook and do
 * other bulk operations
 * @return mixed
 */

function behat_editor_index() {
    composer_manager_register_autoloader();
    module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
    libraries_load('behat_editor_data_table');
    $path = drupal_get_path('module', 'behat_editor');
    drupal_add_js($path . '/js/behat_editor_app.js');
    drupal_add_js($path . '/js/behat_editor_index.js');
    drupal_add_css($path . '/css/behat_editor_app.css');

    behat_editor_add_bootstrap();

    $build['messages_top'] = array(
        '#markup' => "<div id='messages-behat'></div>",
        '#weight' => -100
    );

    $build['actions'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('col-lg-12', 'well')),
        '#weight' => -100
    );

    $build['actions']['add_test'] = array(
        '#type' => 'link',
        '#title' => t('Add Test'),
        '#href' => "admin/behat/add",
        '#attributes' => array('role' => 'button', 'class' => array('btn', 'btn-success', 'btn-md', 'add')),
    );

    if ( module_exists('behat_editor_limit_tags') ) {
        $build['actions']['manage_tags'] = array(
            '#type' => 'link',
            '#title' => t('Manage Tags'),
            '#href' => "admin/config/behat_editor/allowed_tags",
            '#attributes' => array('role' => 'button', 'class' => array('btn', 'btn-default', 'btn-md',  'manage_tags')),
        );
    }

    $help =  _behat_editor_make_help_link('index.html');
    $build['actions']['help'] = array(
        '#prefix' => '<span class="text-success">',
        '#markup' => t('There are icons like this one !help for you to get help through the site', array('!help' => $help)),
        '#suffix' => '</span>'
    );


    $build['file_form_fieldset'] = drupal_get_form('behat_editor_file_uploader');

    //@todo convert to using the json index output
    //  see ticket #85 on github

    $modules = new BehatEditor\FileController();
    //Was getFilesArray for Files
    $files_array = $modules->index();

    $build['intro'] = array(
        '#prefix' => "<div class='alert alert-warning'><i class='glyphicon glyphicon-question-sign'></i>",
        '#suffix' => "</div>",
        '#markup' => t("Type Pass or Fail into search to filter by those states"),
        '#weight' => 9
    );

    $behat_table =_behat_editor_produce_table_array($files_array);
    $build['behat_table'] = array(
        '#markup' => $behat_table,
        '#weight' => 10,
    );

    $title =  "Confirm Delete";
    $body = "The file <span class='filename'>...</span> will be deleted.";
    $buttons = array(
        'save' => array('title' => "Confirm", 'class' => array('btn', 'btn-danger', 'confirm-delete')),
        'delete' => array('title' => 'Cancel', 'class' => array('btn', 'btn-success'), 'data' => array('dismiss' => "modal"))
    );
    $build['message']['#markup'] = behat_editor_modal($title, $body, $buttons);
    behat_editor_results_history_modal($build);
    _behat_editor_load_help_area($build);

    return $build;
}
