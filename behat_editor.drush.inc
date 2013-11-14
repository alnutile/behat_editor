<?php

use Drupal\BehatEditor\BehatEditorRun,
    Drupal\BehatEditor\File,
    Drupal\BehatEditor\Files;

/**
 * @file
 * Drush hook implementations for BehatEditor
 */

/**
 * Implements hook_drush_command().
 */
function behat_editor_drush_command() {
    $items = array();

    $items['behat-run'] = array(
        'description' => 'Pass a module name and file to run as a test pass 1 for javascript TRUE',
        'allow-additional-options' => TRUE,
        'aliases' => array('br'),
        'examples' => array(
            'drush br module_name view.feature' => 'This will use the path of the module_name to run that test'
        ),
        'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
        'arguments' => array(
            'module' => 'The module name that the test file is in',
            'filename' => 'The file name to test. Should be in present folder',
            'javascript' => '1 if you want to run javascript based tests 0 if not. Javascript means you are running this locally.',
            'tag' => 'Add tag to only run test on. Eg @anonymous',
            'profile' => 'The behat.yml file has profiles. Default etc. You can call one here or blank = default',
        ),
    );

    $items['behat-run-folder'] = array(
        'description' => 'Pass a module name to run all tests in it\'s behat_features folder and decide to turn on Javascript',
        'allow-additional-options' => TRUE,
        'aliases' => array('brf'),
        'examples' => array(
            'drush brf module_name 1' => 'This will use the path of the module_name to run that test and 1 means you want to include Javascript'
        ),
        'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
        'arguments' => array(
            'module' => 'The module name that the test files are in',
            'javascript' => '1 if you want to run javascript based tests 0 if not. Javascript means you are running this locally.',
            'subfolder' => '(Optional) if this is a folder in the behat_features folder',
        ),
    );

    return $items;
}


function drush_behat_editor_behat_run() {
    $module = func_get_arg(0);
    $filename = func_get_arg(1);
    if(func_num_args() > 2) {
        $javascript = func_get_arg(2);
    } else {
        $javascript = 0;
    }

    if(func_num_args() > 3) {
        $tag = func_get_arg(3);
    } else {
        $tag = '';
    }

    if(func_num_args() > 4) {
        $profile = func_get_arg(4);
    } else {
        $profile = 'default';
    }

    composer_manager_register_autoloader();
    //@todo get composer to load this
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');


    $file = new File(array(), $module, $filename, FALSE);
    $file_object = $file->get_file_info();

    $run = new BehatEditorRun($file_object);
    print "Running Tests...\n";
    $output = $run->execDrush($javascript, $tag, $profile);
    $results = implode("\n\r", $output['output_array']);
    return drush_print($results);
}

//@todo add profile switch as well
function drush_behat_editor_behat_run_folder() {
    print "Running Tests...\n";
    module_load_include('inc', 'behat_editor', 'helpers/behat_helpers_app');
    composer_manager_register_autoloader();
    //@todo validate args
    $module = func_get_arg(0);
    if(func_num_args() > 1) {
        $javascript = func_get_arg(1);
    } else {
        $javascript = 0;
    }


    $fileObject = File::fileObjecBuilder();
    $path = drupal_get_path('module', $module);
    $file_tests_folder = variable_get('behat_editor_default_folder', BEHAT_EDITOR_DEFAULT_FOLDER);
    $path = drupal_realpath($path) . '/' . $file_tests_folder;
    if(func_num_args() > 2) {
        $path = $path . '/' . func_get_arg(2);
    }
    $fileObject->absolute_file_path = $path;
    $run = new BehatEditorRun($fileObject);
    print "Running Tests in folder  {$fileObject['filename']}...\n";
    $output = $run->exec($javascript);
    $results = implode("\n\r", $output['output_array']);
    drush_print($results);


    return "All Tests ran for module $module";
}
