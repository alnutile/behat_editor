<?php


namespace Drupal\BehatEditor;

use Symfony\Component\Yaml;
use Drupal\BehatEditor\BehatEditorRunHelpers;

class GenerateBehatYml {
    use BehatEditorRunHelpers;

    public $behat_yml = array();
    private $loader;
    private $settings;
    private $behat_filename;
    CONST BEHAT_YML_PATH = 'behat_yml';

    function __construct($settings) {
        composer_manager_register_autoloader();
        $this->settings = $settings;
        $behat_yml = self::behatYmlDefaults();
        $settings_build = new Settings();
        $this->behat_yml = $settings_build->setBehatYmlSettings($behat_yml, $settings);
        $context1 = 'generate_yml';

        drupal_alter('behat_editor_yml_array', $this->behat_yml, $context1, $settings);
    }

    public function writeBehatYmlFile() {
        $subFoldersArray = self::subFolders();
        $yml = new Yaml\Yaml();
        $yml_string = $yml->dump($this->behat_yml);
        $folder = self::BEHAT_YML_PATH;
        $subFolders = implode('/', $subFoldersArray);
        $path = file_build_uri("/{$folder}/$subFolders/");
        $build_folder = file_prepare_directory($path, FILE_MODIFY_PERMISSIONS | FILE_CREATE_DIRECTORY);
        if($build_folder == FALSE) {
            $message = t('Could not make the folder !folder', array('!folder' => $path));
            throw new \RuntimeException($message);
        }

        $this->behat_filename = REQUEST_TIME . '.yml';
        $response = file_unmanaged_save_data($yml_string, $path . '/' . $this->behat_filename, $replace = FILE_EXISTS_REPLACE);
        if($response == FALSE) {
            $message = t('The behat.yml file could not be saved could not be saved !file', array('!file' => $path . '/' . $this->behat_filename));
            throw new \RuntimeException($message);
        } else {
            return drupal_realpath($path . '/' . $this->behat_filename);
        }
    }

    public function deleteBehatYmlFile(){
        $subFoldersArray = self::subFolders();
        $subFolders = implode('/', $subFoldersArray);
        file_unmanaged_delete_recursive(file_build_uri("/".self::BEHAT_YML_PATH."/$subFolders/{$this->behat_filename}"));
    }

    private function subFolders() {
        $subFolderArray = array('default');
        if ($this->settings) {
            if($this->settings['base_url_gsid']) {
                $subFolderArray[0] = $this->settings['base_url_gsid'];
            } else {
                $subFolderArray[0] = '0';
            }
            if($this->settings['base_url_usid']) {
                $subFolderArray[1] = $this->settings['base_url_usid'];
            } else {
                $subFolderArray[1] = '0';
            }
        }
        return $subFolderArray;
    }

    private function behatYmlDefaults() {
        $behat_yml_parse = array();
        $path = drupal_get_path('module', 'behat_editor');
        //Check for custom file
        //$behat_yml_path_custom = drupal_realpath($path) . '/behat/behat.yml';
        $behat_yml_path_custom = $this->setBehatYmlPath()->getBehatYmlPath();
        if(file_exists($behat_yml_path_custom)) {
            $behat_yml_path = $behat_yml_path_custom;
        } else {
            $behat_yml_path = drupal_realpath($path) . '/behat/behat.yml.example';
        }

        $loader = new Yaml\Yaml();
        $behat_yml_parse = $loader->parse($behat_yml_path);

        if(!$behat_yml_parse) {
            $message = "Could not locate the behat.yml.template file";
            throw new \RuntimeException($message);
        }

        $this->behat_yml = $behat_yml_parse;
        return $this->behat_yml;
    }


}