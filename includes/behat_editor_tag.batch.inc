<?php

use Drupal\BehatEditor;

/**
 * @file
 *   Interface to do tag related batch work
 */


function behat_editor_bulk_tags() {
    $output = drupal_get_form('behat_editor_bulk_form_tags');
    return $output;
}

function behat_editor_bulk_form_tags($form, &$form_state) {
    composer_manager_register_autoloader();
    $build = array();

    $build['overview'] = array(
        '#prefix' => '<div>',
        '#markup' => t("Choose which tags to run the tests for."),
        '#suffix' => '</div>',
    );
    composer_manager_register_autoloader();

    $build['overview'] = array(
        '#prefix' => '<div>',
        '#markup' => t("Choose which tag(s) to run the tests from."),
        '#suffix' => '</div>',
    );

    $header = array(
        'machinename' => array('data' => t('Tag Name')),
        'last_run' => array('data' => t('Last Run')),
        'batch_status' => array('data' => t('Status')),
        'pass_fail' => array('data' => t('Pass / Fail')),
        'report' => array(t('Report Link')),
    );

    $rows = behat_editor_build_rows_tags();

    $build['dashboard'] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $rows,
        '#empty' => t('No Tags'),
    );

    $options = array(
        'gui' => t('Run in Webbrowser'),
//        'stop' => t('Stop'),
//        'background' => t('Run in background'),
    );

    $build['operations']['operation'] = array(
        '#type' => 'select',
        '#title' => t('Operation'),
        '#title_display' => 'invisible',
        '#options' => $options,
    );

    $build['operations']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Execute'),
        '#validate' => array('bulk_editor_run_tag_based_validate'),
        '#submit' => array('bulk_editor_run_tag_based_submit'),
    );

    return $build;
}


/**
 * Validate callback for the dashboard form.
 * @todo compare to allowed tags if module enabled
 */
function bulk_editor_run_tag_based_validate($form, &$form_state) {
    // Error if there are no items to select.
    if (!is_array($form_state['values']['dashboard']) || !count(array_filter($form_state['values']['dashboard']))) {
        form_set_error('', t('No items selected.'));
    }
}

/**
 * Submit callback for the dashboard form.
 */
function bulk_editor_run_tag_based_submit($form, &$form_state) {

    $operation = $form_state['values']['operation'];
    $tags = array_filter($form_state['values']['dashboard']);

    if($operation == 'gui') {
        $start = new BehatEditor\BehatEditorBatchRunGui();
        $batch = $start->batchSubmit('tags', $tags, 'non_background');
        batch_set($batch->getBatch());
    }
}

function bulk_editor_batch_run_tag($tag, $rid){
    composer_manager_register_autoloader();

    $run = new BehatEditor\BehatEditorBatchTypeTag();
    $run->batchRun(array('tag' => array($tag), 'rid' => $rid));
    $run->batchItemDone(array('item' => $tag));
    $context['message'] = $run->message;
}

function bulk_editor_batch_tag_done($success, $results, $operations){
    composer_manager_register_autoloader();
    $message = "All tests have been completed";
    $done = new BehatEditor\BehatEditorBatchTypeModule();
    $message = $done->batchDone($success, $results, $operations, $message);
    drupal_set_message($message);
}


/** HELPERS */

function behat_editor_build_rows_tags() {
    $tags = behat_editor_tags_get();
    $rows = array();

    foreach($tags as $key => $value) {
        $last_run = 'n/a';
        $status = 'n/a';
        $pass_fail = 'n/a';
        $view = 'n/a';
        $results = BehatEditor\ResultsBatch::getResultsForByTag($value);
        if(!empty($results['results'])) {
            $results = $results['results'][0];
            $args = implode('/', $results['results']);
            $view = l('view', 'admin/behat/report/' . $args, array('attributes' => array('target' => '_blank')));
            $last_run = format_date($results['created'], 'medium');
            $status = BehatEditor\ResultsBatch::getBatchRunningStatus($results['batch_status']);
            $pass_fail = BehatEditor\ResultsBatch::getResultsPassFail($results['pass_fail']);
        }
        $row['machinename'] = $value;
        $row['last_run'] = $last_run;
        $row['batch_status'] = $status;
        $row['pass_fail'] = $pass_fail;
        $row['report'] = $view;
        $rows["$value"] = $row;
    }
    return $rows;
}

function behat_editor_tags_get() {
    $file = new BehatEditor\Files();
    $files = $file->getFilesArray();
    $tags = array();
    foreach($files as $key => $value) {
        foreach($value as $key2 => $value2) {
            if(isset($value2['tags_array'])) {
                foreach($value2['tags_array'] as $tag) {
                    $tags[$tag] = $tag;
                }
            }
        }
    }
    return $tags;
}

