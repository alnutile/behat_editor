<?php
/**
 * Created by PhpStorm.
 * User: alfrednutile
 * Date: 11/18/13
 * Time: 8:24 AM
 */

namespace Drupal\BehatEditor;


class Settings {
    public $base_url;
    public $behat_yml_defaults;
    public $settings;


    function __construct(){
        composer_manager_register_autoloader();
    }

    public function setBehatYmlSettings($behat_yml_defaults, $settings) {
        $this->behat_yml_defaults = $behat_yml_defaults;
        $this->settings = $settings;
        /**
         * @todo
         *   It could know less about these classes and just send settings
         */
        self::setFeaturesPath();
        self::setBaseUrl();
        return $this->behat_yml_defaults;
    }

    private function setBaseUrl() {
        if($this->settings) {
            $base_url_get = new BehatSettingsBaseUrl();
            $this->base_url = $base_url_get->getBaseUrlFromSidArray($this->settings);
            $this->behat_yml_defaults['default']['extensions']['Behat\MinkExtension\Extension']['base_url'] = $this->base_url;
        }
    }

    private function setFeaturesPath(){
        $path = drupal_get_path('module', 'behat_editor') . '/behat/features/bootstrap/FeatureContext.php';
        $full_path = drupal_realpath($path);
        $this->behat_yml_defaults['default']['paths']['bootstrap'] = $full_path;
    }
} 