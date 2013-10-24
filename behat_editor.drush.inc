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
        ),
    );


    $items['behat-libraries'] = array(
        'callback' => 'drush_behat_libraries',
        'description' => dt("Download all the needed libraries"),
        'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
        'arguments' => array(
            'path' => dt("If you do not want to use the default library path")
        ),
        'aliases' => array('bl')
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
    $results = implode("\n\r", $output);
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

    $get = new Files();
    $files_array = $get->getFilesArray();
    //find those for this module
    foreach($files_array as $key => $value) {
        foreach($value as $file_path => $values) {
            if($module == $values['module']) {
                $modules_tests[$file_path] = $values;
            }
        }
    }
    foreach($modules_tests as $key) {
        $run = new BehatEditorRun($key);
        print "Running Test {$key['filename']}...\n";
        $output = $run->execDrush($javascript);
        $results = implode("\n\r", $output);
        drush_print($results);
    }

    return "All Tests ran for module $module";
}

/**
 * Thanks to https://drupal.org/node/1231378
 */
function drush_behat_libraries(){
    if (!drush_shell_exec('type unzip')) {
        return drush_set_error(dt('Missing dependency: unzip. Install it before using this command.'));
    }

    $args = func_get_args();
    if ($args[0]) {
        $path = $args[0];
    }
    else {
        $path = 'sites/all/libraries';
    }

    // Create the path if it does not exist.
    if (!is_dir($path)) {
        drush_op('mkdir', $path);
        drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
    }

    // Set the directory to the download location.
    $olddir = getcwd();
    chdir($path);

    $libraries = array(
        'tag_it' => array('url' => 'https://codeload.github.com/aehlke/tag-it/zip/master'),
        'data_table' => array('url' => 'http://datatables.net/releases/DataTables-1.9.4.zip'),
        'date_format' => array('url' => 'https://codeload.github.com/alnutile/jacob_wright_date_format/zip/master')
    );

    foreach($libraries as $key => $value) {
        $dirname = 'behat_editor_' . $key;
        //Check if installed
        if(!is_dir($dirname)) {
            //Make folder
            drush_op('mkdir', $dirname);
            chdir($dirname);
            //Get zips
            if (!drush_shell_exec('wget ' . $libraries[$key]['url'] . ' -O file.zip')) {
                drush_shell_exec('curl ' . $libraries[$key]['url'] . ' > file.zip' );
            }
            //Unzip and remove zip
            if (is_file('file.zip')) {
                drush_shell_exec('unzip -qqj -o file.zip');
                drush_op('unlink', 'file.zip');
            }
            //Down go back to the root library folder
            chdir('../');

        }


        print $libraries[$key]['url'];
        print $key;
    }


}