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

    $path = drupal_get_path('module', 'behat_editor');
    $build['#attached']['js']['behat_editor_sids'] = $path . '/js/behat_editor_sids.js';


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
        '#multiple' => FALSE,
        '#empty' => t('No Tags'),
    );

    BehatEditor\BehatSettingsFormHelper::behatSettingsFields($build);

    $options = array(
        'gui' => t('Run in Web browser'),
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
    if (!isset($form_state['values']['dashboard'])) {
        form_set_error('', t('No items selected.'));
    }
}

/**
 * Submit callback for the dashboard form.
 */
function bulk_editor_run_tag_based_submit($form, &$form_state) {
    $os = $form_state['values']['os'];
    $browser = $form_state['values']['browser'];
    $settings = array(
        'base_url_usid' => $form_state['values']['users'],
        'base_url_gsid' => $form_state['values']['group'],
        'os_version' => $os,
        'browser_version' => $browser,
    );
    $operation = $form_state['values']['operation'];
    $tags = $form_state['values']['dashboard'];

    if($operation == 'gui') {
        $batch = _behat_editor_batch_tags_setup(array('tags' => array($tags), 'settings' => $settings));
        batch_set($batch);
    }
}

function _behat_editor_batch_tags_setup($params) {
    $operations = array();
    $setup_batch = new BehatEditor\BehatEditorBatchTypeTag();
    $setup_batch->setupRid('tags', $params['tags'], 'non_background', $params['settings']);
    $files = new BehatEditor\BehatEditorBatchTypeTag();
    $rid = $setup_batch->getCurrentBatchRid();
    $params = array('tags' => $params['tags'], 'settings' => $params['settings'], 'rid' => $rid);
    $files->batchSetupFolderCopyFiles($params);
    $count = $files->getFileCount();
    $files_all = $files->getAllFilesArray();
    $setup_batch->setTotalCount(array('rid' => $rid, 'test_count' => $count));
    foreach($params['tags'] as $tag) {
        $current = 1;
        foreach($files_all[$tag] as $file) {
            $file = drupal_realpath($file);
            $operations[] = array('_behat_editor_batch_tags_run',
                array(
                    'file_path' => $file,
                    'tag' => $tag,
                    'rid' => $rid,
                    'settings' => $params['settings'],
                    t("Working on File @file test #@current", array('@file' => $file, '@current' => $current)),
                    'total_found' => $count,
                    'current' => $current,
                )
            );
            $current++;
        }
    }
    $batch = array(
        'operations' => $operations,
        'finished' => '_behat_editor_batch_tags_run_finish',
        'init_message' => t('Starting the tests'),
        'error_message' => t('An error occurred. Please check the Reports/DB Logs'),
        'file' => drupal_get_path('module', 'behat_editor') . '/includes/behat_editor_tag.batch.inc',
        'progress_message' => t('Total Files found @number', array('@number' => $count)),
    );

    return $batch;
}

function _behat_editor_batch_tags_run($file_path, $tag, $rid, $settings, $message, $total, $current, &$context ) {
    if (empty($context['sandbox'])) {
        $context['sandbox'] = array();
        $context['sandbox']['progress'] = 1;

        // Save node count for the termination message.
        $context['sandbox']['max'] = $total;
    }

    $context['sandbox'];

    $run = new BehatEditor\BehatEditorBatchTypeTag();
    $results = $run->batchRunForFile(array('tag' => array($tag), 'rid' => $rid, 'settings' => $settings, 'file_path' => $file_path));
    $run->batchItemDone(array('item' => $tag));

    //wrap up the batch api info
    $context['sandbox']['progress']= $current;
    $context['results'][] = $results;
    $context['message'] = t("File Tests @name with message @message", array('@name' => $file_path, '@message' => $message));
}

function _behat_editor_batch_tags_run_finish($success, $results, $operations) {
    if ($success) {
        drupal_set_message(t('@count files were tested', array('@count' => count($results))));

        foreach($results as $key => $value) {
            //$filename = explode('/', $value);
            //$filename = array_pop($filename);
            drupal_set_message(t('files !files was tested', array('!files' => $filename)));
        }
    }
    else {
        $error_operation = reset($operations);
        drupal_set_message(t('An error occurred while processing @operation with arguments : @args', array('@operation' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE))));
    }
}

function bulk_editor_batch_run_tag($tag, $rid, $settings){
    composer_manager_register_autoloader();

    $run = new BehatEditor\BehatEditorBatchTypeTag();
    $run->batchRun(array('tag' => array($tag), 'rid' => $rid, 'settings' => $settings));
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
    $file = new BehatEditor\FileModel();
    $files = $file->getAllFiles();
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

