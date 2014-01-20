<?php

namespace Drupal\BehatEditorReports;
use Drupal\BehatEditor;

/**
 * @TODO limit by user and group
 */

class BehatServiceReportsModel {
    public $settings;

    public function __construct($settings = null) {
        if(empty($settings)) {
            $this->settings = new BehatEditor\BehatSettingsBaseUrl();
        } else {
            $this->settings = $settings;
        }
    }

    //Get all results
    // paginate as at 100
    public function get_all() {
        $all_browsers = array();
        $all_users = array();
        $all_urls = array();
        $query = db_select('behat_editor_results', 'b');
        $query->fields('b');
        $query->groupBy('filename');
        $query->range(0, 100);
        $query->orderBy('b.created', 'DESC');
        $result = $query->execute();
        $rows = array();
        if ($result) {
            foreach ($result as $record) {
                $record->results = unserialize($record->results);
                $record->settings = unserialize($record->settings);
                $browser = $record->settings['browser_version'];
                $record->settings['browser_version'] = $this->getBrowser($browser);
                $all_browsers[$record->settings['browser_version']] = $record->settings['browser_version'];
                $user = $this->getUser($record->uid);
                $url_found = $this->getUrl($user['uid'], $record->settings['base_url_usid'], $record->settings['$base_url_gsid']);
                $all_urls[$url_found['sid']] = array('nice_name' => $url_found['nice_name'], 'sid' => $url_found['sid']);
                $all_users[$user['uid']] = array('uid' => $user['uid'], 'mail' => $user['mail']);
                $record->settings['url'] = $url_found['nice_name'];
                $tags = $this->getTags($record->results);
                $rows[] = (array) $record;
            }
        }
        return array('results' => $rows, 'error' => 0, 'browsers' => $all_browsers, 'users' => $all_users, 'urls' => $all_urls);
    }




    protected function getBrowser($browser) {
        $browser = explode('|', $browser);
        watchdog('test_browser', print_r($browser, 1));
        switch ($browser[0]) {
            case 'chrome':
                return "Chrome";
                break;
            case 'internet explorer':
                return "Internet Explorer";
                break;
            case '':
                return 'headless';
                break;
            default:
                return $browser[0];
        }
    }

    protected function getUser($uid) {
        if($uid == 0) {
            return array('uid' => 0, 'mail' => 'unknown');
        } else {
            $result = db_select('users', 'u')
                ->fields('u', array('uid', 'mail'))
                ->condition('u.uid', $uid, '=')
                ->execute()
                ->fetchAssoc();
            return $result;
        }
    }

    protected function getUrl($uid, $base_url_usid, $base_url_gsid) {
        if($base_url_usid == 0) {
            $settings = $this->settings->getSettingsBySID(array($base_url_gsid));
        } else {
            $settings = $this->settings->getSettingsBySID(array($base_url_usid));
        }
        return $settings['results'];
    }

    protected function getTags($test) {
        watchdog('test_tags_coming_in', print_r($test, 1));
        $step1 = explode('<li>', $test);
        $step2 = explode('</li>', $step1);
        watchdog('test_tags', print_r($step1, 1));
    }
}