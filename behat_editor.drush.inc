<?php

use Drupal\BehatEditor\BehatEditorRun,
    Drupal\BehatEditor\File,
    Drupal\BehatEditor;

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
            'path' => 'This is the path to the file. But if the file lives in default/files/behat_tests/features it would be behat_tests/filename.feature. But if it is a subfolder it would be behat_tests/tests/filename.feature showing the full path.',
            'javascript' => '1 if you want to run javascript based tests 0 if not. Javascript means you are running this locally.',
            'tag' => 'Add tag to only run test on. Eg @anonymous',
            'profile' => 'The behat.yml file has profiles. Default etc. You can call one here or blank = default',
            'user settings sid' => 'The user settings sid you would like to run.',
            'group settings sid' => 'The groups settings sid you would like to run or 0',
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

    $items['behat-user-settings'] = array(
        'description' => 'Show a users settings for a users paths',
        'allow-additional-options' => TRUE,
        'aliases' => array('bus'),
        'examples' => array(
            'drush bus 1' => 'This will get the settings for the user with id 1'
        ),
        'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
        'arguments' => array(
            'uid' => 'The users uid',
        ),
    );

    $items['behat-group-settings'] = array(
        'description' => 'Show a groups settings for a groups paths',
        'allow-additional-options' => TRUE,
        'aliases' => array('bug'),
        'examples' => array(
            'drush bug 3' => 'This will get the settings for the group with id 1'
        ),
        'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
        'arguments' => array(
            'gid' => 'The groups gid',
        ),
    );

    $items['behat-test-setup'] = array(
        'description' => 'Setup Tests',
        'allow-additional-options' => TRUE,
        'aliases' => array('bts'),
        'examples' => array(
            'drush bts' => 'This will run the setup steps'
        ),
        'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
        'arguments' => array(),
    );


    return $items;
}

function drush_behat_editor_behat_user_settings() {
    $uid = func_get_arg(0);
    print "You can use the SID column to pass as a settings\n";
    composer_manager_register_autoloader();
    $perms = new \Drupal\BehatEditor\BehatSettingsBaseUrl();
    $results = $perms->getSettingsByUID($uid);
    $header = array(
        'sid',
        'uid',
        'gid',
        'base_url',
        'default',
        'active',
        'nice_name'
    );
    array_unshift($results['results'], $header);
    drush_print_table($results['results'], $header = TRUE, $widths = array(), $handle = NULL);
}

function drush_behat_editor_behat_group_settings() {
    $gid = func_get_arg(0);
    print "You can use the SID column to pass as a settings\n";
    composer_manager_register_autoloader();
    $perms = new \Drupal\BehatEditor\BehatSettingsBaseUrl();
    $results = $perms->getSettingsByGID(array($gid));
    $header = array(
        'sid',
        'uid',
        'gid',
        'base_url',
        'default',
        'active',
        'nice_name'
    );
    array_unshift($results['results'], $header);
    drush_print_table($results['results'], $header = TRUE, $widths = array(), $handle = NULL);
}

function drush_behat_editor_behat_run() {
    $module = func_get_arg(0);
    $path = func_get_arg(1);
    $tag_include = '';
    if(func_num_args() > 2) {
        $javascript = func_get_arg(2);
    } else {
        $javascript = 0;
    }

    if(func_num_args() > 3) {
        $tag_include = func_get_arg(3);
    } else {
        $tag_include = '';
    }

    if(func_num_args() > 4) {
        $profile = func_get_arg(4);
    } else {
        $profile = 'default';
    }

    if(func_num_args() > 5) {
        $usid = func_get_arg(5);
    } else {
        $usid = 0;
    }

    if(func_num_args() > 6) {
        $gsid = func_get_arg(6);
    } else {
        $gsid = 0;
    }

    composer_manager_register_autoloader();
    $file_path = explode("/", $path);
    $file_path_temp = $file_path;
    $filename = array_pop($file_path_temp);
    $file = new BehatEditor\FileController();
    $params = array(
        'service_path' => $file_path,
        'module' => $module,
        'filename' => $filename,
        'action' => 'view'
    );
    $file_object = $file->show($params);
    $run = new BehatEditorRun($file_object);
    print "Running Tests...\n";
    $output = $run->exec($javascript, $settings = array('base_url_usid' => $usid, 'base_url_gsid' => $gsid), $context =  'behat_run', $tag_include, $profile);
    $results = str_replace(array("    ", "---- Feature", "::: Scenario", "::: Background"), array("\n", "\n---- Feature", "\n::: Scenario", "\n::: Background"), drush_html_to_text($output['output_array']));
    return drush_print($results);
}

/**
 * @todo
 * the above drush_behat_editor_behat_run works this one is out of date
 * @return string
 */
function drush_behat_editor_behat_run_folder() {
    print "Running Tests...\n";
    composer_manager_register_autoloader();
    //@todo validate args
    $module = func_get_arg(0);
    if(func_num_args() > 1) {
        $javascript = func_get_arg(1);
    } else {
        $javascript = 0;
    }

    $path = func_get_arg(1);

    $file_path = explode("/", $path);
    $filename = 'drush_folder_run';
    $file = new BehatEditor\FileController();
    $params = array(
        'service_path' => $file_path,
        'module' => $module,
        'filename' => $filename,
        'action' => 'view'
    );
    $fileObject = $file->show($params);
    $run = new BehatEditorRun($fileObject);

    print "Running Tests in folder {$path}\n";
    $output = $run->exec($javascript);
    $results = str_replace(array("    ", "---- Feature", "::: Scenario", "::: Background"), array("\n", "\n---- Feature", "\n::: Scenario", "\n::: Background"), drush_html_to_text($output['output_array']));
    return drush_print($results);
}


//@todo add profile switch as well
function drush_behat_editor_behat_test_setup() {
    composer_manager_register_autoloader();
    $public = file_build_uri('/behat_tests/');
    $public = drupal_realpath($public);
    $full_path = $public . '/tests_of_tests.feature';
    unlink($full_path);
}


function _behat_editor_setup_new_test_file($full_path) {

    $data = <<<HEREDOC
     @example
 Feature: Example Test for WikiPedia

   Scenario: WikiPedia
     Given I am on "http://en.wikipedia.org/wiki/Main_Page"
     Then I should see "WikiPedia"
     Then I should see "Muffins"

HEREDOC;
    $file = fopen($full_path, 'w');
    fwrite($file, $data);
    fclose($file);
}
