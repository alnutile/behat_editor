<?php
/**
 * http://thomaslattimore.com/blog/using-phpunit-drupal-7
 */

namespace Drupal\BehatEditor;


// We assume that this script is being executed from the root of the Drupal
// installation. e.g. ~$ `phpunit TddTests sites/all/modules/tdd/TddTests.php`.
// These constants and variables are needed for the bootstrap process.
define('DRUPAL_ROOT', getcwd());
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';

// Bootstrap Drupal.
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

class FileModelTests extends \PHPUnit_Framework_TestCase {
    protected $file_object = array();
    protected $filename;
    protected $fullpath;
    protected $file_data;

    public function __construct() {
        composer_manager_register_autoloader();
        $this->filename = 'phpunit.feature';
        $this->params = array(
            'module' => "behat_tests",
            'filename' => "phpunit.feature",
            'scenario' => explode("\n", self::file_data()),
            'parse_type' => 'file',
            'action' => 'view',
            'service_path' => array('behat_tests', 'phpunit.feature')
        );
    }

    public function test_createFile() {
        $test_file = new FileModel($this->params);
        $test_file->createFile();
        $this->file_data = $test_file->file_data;
        $this->assertFileExists($test_file->file_data['absolute_path_with_file']);
        $test = fopen($test_file->file_data['absolute_path_with_file'], r);
        $test_read = fread($test, filesize($test_file->file_data['absolute_path_with_file']));
        $this->assertTrue((strpos($test_read, 'WikiPedia') != FALSE), $message = "Content not in file");
    }

    public function test_save(){
        $this->params['scenario'] = explode("\n", self::file_data_updated());
        $test_save = new FileModel($this->params);
        $test_output = $test_save->save();
        $this->file_data = $test_output['data'];
        $this->assertFileExists($this->file_data['absolute_path_with_file']);
        $test = fopen($this->file_data['absolute_path_with_file'], r);
        $test_read = fread($test, filesize($this->file_data['absolute_path_with_file']));
        $this->assertTrue((strpos($test_read, 'Updated') != FALSE), $message = "Content not in file");
    }

    public function test_deleteFile() {
        $this->params['filename'] = 'test_delete.feature';
        $this->params['service_path'] = array('behat_tests', 'test_delete.feature');
        $test = new FileModel($this->params);
        $output = $test->save(); //setup first
        $this->fullpath = $output['data']['absolute_path_with_file'];

        $this->params['module'] = 'behat_editor'; //test no delete first
        $test = new FileModel($this->params);
        $output = $test->deleteFile();
        $this->assertTrue(($output['error'] == 1));
        $this->assertFileExists($this->fullpath);

        $this->params['filename'] = 'test_delete.feature';
        $this->params['service_path'] = array('behat_tests', 'test_delete.feature');
        $this->params['module'] = 'behat_tests';
        $test = new FileModel($this->params);
        $output = $test->deleteFile();
        $this->assertTrue(($output['error'] != 1));
        $this->assertFileNotExists($this->fullpath);
    }

    public function test_getFile() {
        $file = new FileModel($this->params);
        $file->save();
        $output = $file->getFile();
        $this->assertTrue((strpos($output['scenario'], 'WikiPedia') != FALSE));
    }

    public function test_getAllFiles() {
        $files = new FileModel($this->params);
        $test_output = $files->save();
        $this->file_data = $test_output['data'];
        $this->fullpath = $this->file_data['absolute_path_with_file'];
        $all_files = $files->getAllFiles();
        $this->assertArrayHasKey($this->fullpath, $all_files["behat_tests"]);

    }

    static function _setup_new_test_file($full_path) {
        $data = self::file_data();
        $file = fopen($full_path, 'w');
        fwrite($file, $data);
        fclose($file);
    }

    function tearDown() {
       unlink($this->file_data['absolute_path_with_file']);
    }

    static function file_data() {
        $data = <<<HEREDOC
     @example
 Feature: Example Test for WikiPedia

   Scenario: WikiPedia
     Given I am on "http://en.wikipedia.org/wiki/Main_Page"
     Then I should see "WikiPedia"
     Then I should see "Muffins"

HEREDOC;
        return $data;
    }

    static function file_data_updated() {
        $data = <<<HEREDOC
     @example
 Feature: Example Test for WikiPedia

   Scenario: Updated
     Given I am on "http://en.wikipedia.org/wiki/Main_Page"
     Then I should see "WikiPedia"
     Then I should see "Muffins"

HEREDOC;
        return $data;
    }

}

