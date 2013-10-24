<?php
use Drupal\BehatEditor\File;
/**
 * @file output the form on the View page
 */

/**
 * View form to see the file in a new editable state
 *
 * @param module
 *   this should be the machine_name
 * @param filename
 *   this should be the filename.feature extension
 * @return array
 *
 */

function behat_editor_view_form($form, $form_state, $module, $filename) {
    behat_editor_add_bootstrap();
    composer_manager_register_autoloader();
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    module_load_include('inc', 'behat_editor', 'tpl/behat_shared_views');
    libraries_load('behat_editor_data_table');
    libraries_load('behat_editor_date_format');

    $file = new File($_REQUEST, $module, $filename, FALSE);
    $file_info = $file->get_file_info();

    $path = drupal_get_path('module', 'behat_editor');
    $form['#theme'] = array('theme_behat_editor_view');

    $form['#attached']['js']['behat_editor_app'] = $path . '/js/behat_editor_app.js';
    $form['#attached']['js']['behat_editor_results'] = $path . '/js/behat_editor_results.js';
    $form['#attached']['js']['behat_editor_run'] = $path . '/js/behat_editor_run.js';
    $form['#attached']['js']['behat_editor_view'] = $path . '/js/behat_editor_view.js';

    $form['#attached']['css']['behat_app_css'] = $path . '/css/behat_editor_app.css';
    $form['#attached']['css']['behat_view_css'] = $path . '/css/behat_editor_view.css';

    $form['header'] = array(
        '#markup' => '<h3>Feature: ' . $filename . '</h3>'
    );

    $file_to_html = theme('behat_editor_file_to_html_view', $vars = array('file_text' => $file_info['scenario']));

    $form['actions'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('col-lg-12', 'well', 'actions'))
    );

    $form['actions']['run_test'] = run_test_array($file_info['filename_no_ext'], $module, 'run');
    $form['actions']['admin_area'] = admin_button_array();
    $form['actions']['edit_test'] = edit_test_array($file_info['filename_no_ext'] . ".feature", $module);


    $form['question'] = array(
        '#prefix' => '<div class="panel panel-default col-md-6">',
        '#suffix' => '</div>',
        '#markup' => theme('behat_editor_view_questions_area', array('questions' => $file_to_html['file_text'], 'actions' => array()))
    );

    $form['results_area'] = array(
        '#type' => 'container',
        '#attributes' => array('id' => 'accordion', 'class' => array('col-md-5', 'panel-group'))
    );

    $form['results_area']['container1'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('panel', 'panel-default')),
    );

    $form['results_area']['container1']['results'] = array(
        '#prefix' => "<div class=\"panel-heading\">
                        <a class=\"accordion-toggle\" data-toggle=\"collapse\" data-parent=\"#accordion\" href=\"#collapseOne\">
                            <h4 class=\"panel-title\">Current Test</h4>
                        </a></div>",
        '#markup' => theme('behat_editor_results_area', $args = array('params' => array())),
    );


    $form['results_area']['container2'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('panel', 'panel-default')),
    );

    $form['results_area']['container2']['past_results'] = array(
        '#prefix' => "<div class=\"panel-heading\">
                            <a class=\"accordion-toggle\" data-toggle=\"collapse\" data-parent=\"#accordion\" href=\"#collapseTwo\">
                                <h4 class=\"panel-title\">Past Results</h4>
                            </a>
                        </div>",
        '#markup' => '<div id="collapseTwo" class="panel-collapse collapse">
                        <div class="panel-body">
                                <table cellpadding="0" cellspacing="0" border="0" class="display" id="past-results-table"></table>
                        </div>'
    );

    $title = "Preview of test";
    $body = "<div class='test'></div>";
    $buttons = array(
        'close' => array('title' => 'Cancel', 'class' => array('btn', 'btn-success'), 'data' => array('dismiss' => "modal"))
    );
    $form['message']['#markup'] = behat_editor_modal($title, $body, $buttons);

    $form['filename'] = array('#type' => 'hidden', '#default_value' => $file_info['filename_no_ext']);
    $form['module'] = array('#type' => 'hidden', '#default_value' => $module);
    return $form;
}
