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
        'batch_status' => array('data' => t('Status')),
        'pass_fail' => array('data' => t('Pass / Fail')),
    );

    $all = new BehatEditor\Files(array(), FALSE, $cache = FALSE);
    //So I can pull out the nice name
    $modules = BehatEditor\Files::getModuleFolders();
    $modules = array_merge($modules, BehatEditor\Files::_hasTestFolderArray());

    $rows = array();
    foreach($all->getFilesArray() as $key => $value) {
        $done = '';
        foreach($value as $possible_row) {
            $module = $key;
            $last_run = 'n/a';
            $status = 'n/a';
            $pass_fail = 'n/a';
            $folder = BEHAT_EDITOR_FOLDER;
            $array_key = $key;
            if($possible_row['subpath'] !== FALSE) {
                $subfolder = $possible_row['subpath'];
                $array_key = $array_key . '|' . $subfolder;
            } else {
                $subfolder = '';
            }
            if(!isset($rows[$array_key])) {
                $results = BehatEditor\ResultsBatch::getResultsForByModule($module, $folder, $subfolder);
                if(!empty($results['results'])) {
                        $results = $results['results'][0];
                        $last_run = format_date($results['created'], 'medium');
                        $status = BehatEditor\ResultsBatch::getResultsLabel($results['batch_status']);
                        $pass_fail = BehatEditor\ResultsBatch::getResultsPassFail($results['status']);
                }
                $row['machinename'] = $array_key;
                $row['nicename'] = $modules[$possible_row['module']]['nice_name'];
                $row['folder'] = $folder;
                $row['subfolder'] = $subfolder;
                $row['last_run'] = $last_run;
                $row['batch_status'] = $status;
                $row['pass_fail'] = $pass_fail;
                $rows["$array_key"] = $row;
            }
        }
    }

    $build['dashboard'] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $rows,
        '#empty' => t('No Tests Files'),
    );

    $options = array(
        'run' => t('Run'),
        'stop' => t('Stop'),
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
        '#validate' => array('bulk_editor_run_module_based_validate'),
        '#submit' => array('bulk_editor_run_module_based_submit'),
    );

    return $build;
}

/**
 * Validate callback for the dashboard form.
 */
function bulk_editor_run_module_based_validate($form, &$form_state) {
    // Error if there are no items to select.
    if (!is_array($form_state['values']['dashboard']) || !count(array_filter($form_state['values']['dashboard']))) {
        form_set_error('', t('No items selected.'));
    }
}


function bulk_editor_batch_run($module, $subfolder){
    composer_manager_register_autoloader();
    watchdog("behat_editor_batch", t("Module @mod, Folder @fol", array('@mod' => $module, '@fol' => $subfolder)));
    drupal_set_message("Ran batch test for $module $subfolder");
    $file_object = BehatEditor\File::fileObjecBuilder();
    //@todo DRY this up
    if($module == BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER) {
        $temp = BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER;
        $path = file_build_uri("/{$temp}");
        if($subfolder !== FALSE && $subfolder != 0) {
            $path = $path . '/' . $subfolder;
        }
        $absolute_path = drupal_realpath($path);
    } else {
        $path = drupal_get_path('module', $module);
        if($subfolder !== FALSE && $subfolder != 0) {
            $path = $path . '/' . $subfolder;
        }
        $absolute_path = drupal_realpath($path);
    }
    $file_object['module'] = $module;
    $file_object['absolute_path_with_file'] = $absolute_path;
    $file_object['relative_path'] = $path;
    watchdog("test_absolute_path_with_file", print_r($absolute_path, 1));

    $tests = new BehatEditor\BehatEditorRun($file_object);
    $tests->exec(1);
    $context['message'] = "Running $module";
}


function bulk_editor_batch_done($success, $results, $operations){
    watchdog("test_batch_done_1", print_r($operations, 1));
    $context['message'] = "Ending" . print_r($operations, 1);
}


/**
 * Submit callback for the dashboard form.
 */
function bulk_editor_run_module_based_submit($form, &$form_state) {

    $operation = $form_state['values']['operation'];
    $machine_name_and_subfolder = array_filter($form_state['values']['dashboard']);
    $operations = array();
    foreach($machine_name_and_subfolder as $key) {
        if(strpos($key, '|')) {
            $array = explode('|', $key);
            $module = $array[0];
            $subfolder = $array[1];
        } else {
            $module = $key;
            $subfolder = FALSE;
        }
        $operations[] = array('bulk_editor_batch_run', array($module, $subfolder));
    }

    if (count($operations) > 0) {
        $batch = array(
            'operations' => $operations,
            'title' => t('Behat Batch by Module and Folder'),
            'file' => drupal_get_path('module', 'behat_editor') . '/behat_editor.batch.inc',
            'init_message' => t('Starting Behat Tests'),
            'progress_message' => t(''),
            'error_message' => t('An error occurred. Please check the Reports/DB Logs'),
            'finished' => 'bulk_editor_batch_done',
        );

        batch_set($batch);
    }

//
//    // Rollback in reverse order.
//    if (in_array($operation, array('rollback', 'rollback_and_import'))) {
//        $machine_names = array_reverse($machine_names);
//        foreach ($machine_names as $machine_name) {
//            $operations[] = array('migrate_ui_batch', array('rollback', $machine_name, $limit, $force));
//        }
//        // Reset order of machines names in preparation for final operation.
//        $machine_names = array_reverse($machine_names);
//        $operation = ($operation == 'rollback_and_import') ? 'import' : NULL;
//    }
//
//    // Perform non-rollback operation, if one exists.
//    if ($operation) {
//        foreach ($machine_names as $machine_name) {
//            $migration = Migration::getInstance($machine_name);
//            switch ($operation) {
//                case 'import':
//                    // Update (if necessary) once, before starting
//                    if ($update && method_exists($migration, 'prepareUpdate')) {
//                        $migration->prepareUpdate();
//                    }
//                    $operations[] = array('migrate_ui_batch', array($operation, $machine_name, $limit, $force));
//                    break;
//                case 'stop':
//                    $migration->stopProcess();
//                    break;
//                case 'reset':
//                    $migration->resetStatus();
//                    break;
//            }
//        }
//    }
//
//    // Only rollback and import operations will need to go through Batch API.
//    if (count($operations) > 0) {
//        $batch = array(
//            'operations' => $operations,
//            'title' => t('Migration processing'),
//            'file' => drupal_get_path('module', 'migrate_ui') . '/migrate_ui.pages.inc',
//            'init_message' => t('Starting migration process'),
//            'progress_message' => t(''),
//            'error_message' => t('An error occurred. Some or all of the migrate processing has failed.'),
//            'finished' => 'migrate_ui_batch_finish',
//        );
//        batch_set($batch);
//    }
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

    $build['fieldset'] = array(
        '#type' => 'fieldset',
        '#title' => "Comming soon..."
    );

    return $build;
}