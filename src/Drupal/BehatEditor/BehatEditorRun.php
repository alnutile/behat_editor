<?php

/**
 * @file
 * Contains \Drupal\BehatEditor\BehatEditorRun.
 */

namespace Drupal\BehatEditor;

use Drupal\BehatEditor;

/**
 * Class BehatEditorRun
 * Methods needed to run a test.
 *
 * @params file_object
 *      This is created by \Drupal\BehatEditor\File class
 * @package Drupal\BehatEditor
 *
 * @todo use the File class to create the file object during __construct
 */

class BehatEditorRun {

    public $behat_path = '';
    public $absolute_file_path = '';
    public $output_file = '';
    public $file_full_path = '';
    public $module = '';
    public $relative_path = '';
    public $filename = '';
    public $yml_path = '';
    public $filename_no_ext = '';
    public $file_array;
    public $file_object = array();
    public $clean_results;
    public $settings;
    public $tags;
    public $behat_yml;
    public $rid;

    /**
     * File object from FileObject class
     * for now using a function
     * check it comes from the class later on
     */
    public function __construct($file_object) {

        composer_manager_register_autoloader();
        $path = drupal_get_path('module', 'behat_editor');
        $this->yml_path = drupal_realpath($path) . '/behat/behat.yml';
        $this->behat_path = _behat_editor_behat_bin_folder();
        $this->absolute_file_path = '';
        $this->file_full_path = $file_object['absolute_path_with_file'];
        $this->absolute_file_path = $file_object['absolute_path_with_file'];
        $this->relative_path = $file_object['relative_path'];
        $this->filename = $file_object['filename'];
        $this->filename_no_ext = $file_object['filename_no_ext'];
        $this->module = $file_object['module'];
        $this->file_object = $file_object;
        $this->output_file = self::getPath();
    }

    /**
     * Decide what path we should be storing files at
     *
     * @return string
     */
    public function getPath() {
        if(user_access('behat add test') && $this->module != variable_get('behat_editor_default_storage_folder', BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER)) {
            return  self::runFromModuleFolder();
        } else {
            return self::runFromTmpFolder();
        }
    }

    /**
     * If we are running from the ModuleFolder
     * @return string
     * @todo unify this with runFromTmpFolder().
     */
    public function runFromModuleFolder() {
        //Setup folder to store file with test results
        $file_tmp_folder = variable_get('behat_editor_default_folder', BEHAT_EDITOR_DEFAULT_FOLDER);
        $path_results = file_build_uri("/{$file_tmp_folder}/results");
        if (!file_prepare_directory($path_results, FILE_CREATE_DIRECTORY)) {
            drupal_mkdir($path_results);
        }
        $test_id = $this->filename_no_ext;
        $output_file = drupal_realpath($path_results) . '/' . $test_id . '.txt';    //Needed to run bin
        return $output_file;
    }

    /**
     * If we are running from the Tmp foldser
     * @return string
     * @todo unify this with runFromModuleFolder().
     */
    public function runFromTmpFolder() {
        $file_tmp_folder = variable_get('behat_editor_default_storage_folder', BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER);
        $path_results = file_build_uri("/{$file_tmp_folder}/results");
        if (!file_prepare_directory($path_results, FILE_CREATE_DIRECTORY)) {
            drupal_mkdir($path_results);
        }
        $test_id = $this->filename_no_ext;
        $output_file = drupal_realpath($path_results) . '/' . $test_id . '.txt';    //Needed to run bin
        return $output_file;
    }

    /**
     * Used to exec the behat command
     *
     * @param bool $javascript
     *   Javascript true will open up a browser locally
     *   if the user is running selenium
     * @param array $settings
     *   This can include the user settings id, group settings id
     *   and more.
     * @return array
     */
    public function exec($javascript = FALSE, $settings = array(), $context1 = 'behat_run', $tag_include = FALSE, $profile = 'default') {
        if($javascript == TRUE) {
            $tags_exclude = '';
        } else {
            $tags_exclude = "--tags '~@javascript'";
        }

        if($tag_include) {
            $tag_include = "--tags '" . $tag_include . "'";
        } else {
            $tag_include = '';
        }

        $this->tags = "$tag_include $tags_exclude";

        $command['profile'] = "--profile=$profile";

        $this->settings = $settings;
        $command = self::behatCommandArray();

        //@todo move this into a shared method for exec and execDrush
        $this->settings['context'] = $context1;
        $behat_yml_path = new GenerateBehatYml($this->settings);
        $this->behat_yml = $behat_yml_path->writeBehatYmlFile();

        $saved_settings['behat_yml'] = $behat_yml_path->behat_yml;
        $saved_settings['sid'] = $this->settings;
        $command['config'] = "--config=\"$this->behat_yml\"";
        drupal_alter('behat_editor_command', $command, $context1);
        $command = implode(' ', $command);

        exec($command, $output, $return_var);
        watchdog('behat_command', print_r($command, 1), array(), WATCHDOG_NOTICE);
        $behat_yml_path->deleteBehatYmlFile();

        $results = new Results();
        $output = $results->prepareResultsAndInsert($output, $return_var, $settings, $this->filename, $this->module);
        $this->clean_results = $output['clean_results'];
        $this->rid = $output['rid'];

        return array('response' => $return_var, 'output_file' => $this->clean_results, 'output_array' =>  $this->clean_results, 'rid' => $this->rid);
    }

    private function buildCommandAndYml(array $params) {
        $command = self::behatCommandArray($params['tags']);
        $behat_yml_path = new GenerateBehatYml($params['settings']);
        $behat_yml = $behat_yml_path->writeBehatYmlFile();
        $saved_settings['behat_yml'] = $behat_yml_path->behat_yml;
        $saved_settings['sid'] = $params['settings'];
        $command['config'] = "--config=\"$behat_yml\"";
        $context1 = 'behat_run';
        drupal_alter('behat_editor_command', $command, $context1);
        $command = implode(' ', $command);
        exec($command, $output, $return_var);
        $this->file_array = $output;
        $behat_yml_path->deleteBehatYmlFile();

    }


    /**
     * Return the output on a Pass test
     *
     * @return array
     */
    public function generateReturnPassOutput() {
        $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
        $file_url = l($this->filename, $this->relative_path, $options = array('attributes' => array('target' => '_blank', 'id' => 'test-file')));
        $file_message = array(
            'file' => $this->filename,
            'error' => 0,
            'message' => "$date <br> File $file_url tested."
        );
        $report = self::generateHTMLOutput();
        $output = array('message' => t('@date: <br> Test successful!', array('@date' => $date)), 'file' => $this->filename, 'test_output' => $report, 'error' => FALSE);
        $results = array('file' => $file_message, 'test' => $output, 'error' => 0);
        return $results;
    }

    /**
     * Return the html output on a Test
     *
     * @return array
     *
     * @todo move results and reporting into a separate class
     */
    public function generateHTMLOutput() {
        //$results_message = array_slice(explode("\n", $this->clean_results), -3);
        //$results_message_top = array_slice(explode("\n", $this->clean_results), 0, -3);
        //$output_item_results = theme('item_list', $var = array('title' => 'Summary', 'items' => $results_message));
        $output_item_list = theme('item_list', $var = array('title' => 'Results', 'items' => explode("\n", $this->clean_results)));
        return $output_item_list;
    }

    /**
     * Return the output on a Fail test
     *
     * @return array
     */
    public function generateReturnFailOutput() {
        $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
        $file_url = l($this->filename, $this->relative_path, $options = array('attributes' => array('target' => '_blank', 'id' => 'test-file')));
        $file_message = array(
            'file' => $this->filename,
            'error' => 0,
            'message' => "$date <br> File $file_url tested."
        );
        $message =  t('@date: <br> Error running test @name ', array('@date' => $date, '@name' => $this->filename));
        watchdog('behat_editor', "%date Error Running Test %name", $variables = array('%date' => $date, '%name' => $this->filename), $severity = WATCHDOG_ERROR, $link = $this->absolute_file_path);
        $output = array('message' => $message, 'file' => $this->filename, 'error' => TRUE);
        $results = array('file' => $file_message, 'test' => $output, 'error' => 1, 'message' => $message);
        return $results;
    }

    //@todo remove tag arg in behatCommandArray
    public function behatCommandArray() {
        return array(
            'pre_command' => "cd $this->behat_path &&",
            'run' => "./bin/behat",
            'config' => "--config=\"$this->yml_path\"",
            'path' => '--no-paths',
            'tags' => "$this->tags",
            'format' => '--format=html',
            'profile' => "--profile=default",
            'misc' => '',
            'file_path' => "$this->absolute_file_path"
        );
    }

}