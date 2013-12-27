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

class BehatEditorRunTests extends \PHPUnit_Framework_TestCase {
    protected $file_object = array();

    public function __construct() {
        composer_manager_register_autoloader();
        self::_setup_new_test_file('/tmp/test.feature');
        $this->file_object = array(
            'relative_path' => '/tmp/',
            'absolute_path_with_file' => '/tmp/test.feature',
            'absolute_path' => '/tmp/',
            'filename_no_ext' => '/tmp/',
            'module' => '/tmp/',
        );
    }

    public function test_behat_command_array() {
        $test_array = new BehatEditorRun($this->file_object);
        $test_return = $test_array->behatCommandArray();
        $this->assertEquals($test_return['file_path'], "/tmp/test.feature");
    }

    static function _setup_new_test_file($full_path) {

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

}

