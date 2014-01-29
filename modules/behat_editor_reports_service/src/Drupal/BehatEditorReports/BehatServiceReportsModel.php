<?php

namespace Drupal\BehatEditorReports;
use Drupal\BehatEditor;
use Drupal\BehatEditor\BehatPermissions;
use Drupal\BehatEditor\BehatSettingsBaseUrl;

/**
 * @TODO limit by user and group
 */

class BehatServiceReportsModel {
    public $settings;
    public $permissions;
    protected $browserPassFailCount = array();
    protected $passFailChart = array();
    protected $passFailPerUrl = array();

    public function __construct(BehatSettingsBaseUrl $settings, BehatPermissions $perms) {
        $this->settings = $settings;
        $this->permissions = $perms;
    }

    public function get_all($parameters = array()) {
        list($params) = $parameters;
        //Users Groups
        $group_perms_array = $this->permissions->getGroupIDs();
        $all_browsers = array();
        $all_users = array();
        $all_urls = array();
        $query = db_select('behat_editor_results', 'b');
        $query->fields('b');
        $query->join('behat_editor_base_url_settings', 'u', 'u.sid = b.base_url_sid');
        $query->fields('u');
        if(isset($params['browser']) && $params['browser'] != 'all') {
            $query->condition('settings', "%{$params['browser']}%", 'LIKE');
        }
        if(isset($params['pass_fail']) && $params['pass_fail'] != 'all') {
            $query->condition('status', "{$params['pass_fail']}", '=');
        }
        if(isset($params['filename']) && $params['filename'] != 'all') {
            $query->condition('filename', "%{$params['filename']}%", 'LIKE');
        }
        if(isset($params['url']) && $params['url'] != 'all') {
            $query->condition('b.base_url_sid', $params['url'], '=');
        }
        //Not going to do uid right now so they can look in their group
        //$query->condition('b.uid', $this->permissions->uid, '=');
        if(isset($params['user_id']) && $params['user_id'] != 'all') {
            $uid = $params['user_id'];
            $query->condition('u.uid', $uid, '=');
        } else {
            $uid = $this->permissions->uid;
            $query->condition(db_or()->condition('u.gid', $group_perms_array, 'IN')->condition('u.uid', $uid, '='));
        }

        $query->groupBy('filename');
        //$query->range(0, 200);
        $query->orderBy('b.created', 'DESC');
        $result = $query->execute();
        $rows = array();
        if ($result) {
            foreach ($result as $record) {
                $record->results = unserialize($record->results);
                $record->settings = unserialize($record->settings);
                $browser = (isset($record->settings['browser_version'])) ? $record->settings['browser_version'] : 'undefined';
                $this->setBrowsersPassFail($browser, $record);
                $this->setPassFailChart($record);
                $record->settings['browser_version'] = $this->getBrowser($browser);
                $all_browsers[$browser] = $record->settings['browser_version'];
                $user = $this->getUser($record->uid);
                $all_urls[$record->sid] = $record->nice_name;
                $this->setPassFailPerURL($record->nice_name, $record);
                $all_users[$user['uid']] = $user['mail'];
                $tags = $this->getTags($record->results);
                $rows[] = (array) $record;
            }
        }
        asort($all_urls, SORT_NATURAL);
        return array(
            'results' => $rows,
            'error' => 0,
            'browsers' => $all_browsers,
            'users' => $all_users,
            'urls' => $all_urls,
            'browser_pass_fail_count' => $this->browserPassFailCount,
            'pass_fail_chart' => $this->passFailChart,
            'pass_fail_per_url' => $this->passFailPerUrl,
            'total_count' => '100',
            'pager' => theme('pager'),

        );
    }

    protected function setPassFailPerURL($url, $record) {
        if($record->status == 0) {
            $fail = $this->passFailPerUrl[$url]['fail'] + 1;
            $this->passFailPerUrl[$url]['fail'] = $fail;
        } else {
            $pass = $this->passFailPerUrl[$url]['pass'] + 1;
            $this->passFailPerUrl[$url]['pass'] = $pass;
        }
    }

    /**
     * browser = key and pass fail is a count?
     */
    protected function setBrowsersPassFail($browser, $record) {
        if($record->status == 0) {
            $fail = $this->browserPassFailCount[$this->getBrowser($browser)]['fail'] + 1;
            $this->browserPassFailCount[$this->getBrowser($browser)]['fail'] = $fail;
        } else {
            $pass = $this->browserPassFailCount[$this->getBrowser($browser)]['pass'] + 1;
            $this->browserPassFailCount[$this->getBrowser($browser)]['pass'] = $pass;
        }
    }

    protected function setPassFailChart($record) {
        if($record->status == 0) {
            $fail = $this->passFailChart['fail'] + 1;
            $this->passFailChart['fail'] = $fail;
        } else {
            $pass = $this->passFailChart['pass'] + 1;
            $this->passFailChart['pass'] = $pass;
        }
    }

    protected function getBrowser($browser) {
        $browser = explode('|', $browser);
        switch ($browser[0]) {
            case 'chrome':
                return "Chrome $browser[1]";
                break;
            case 'internet explorer':
                return "Internet Explorer $browser[1]";
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

    protected function getTags($test) {
        //watchdog('test_tags_coming_in', print_r($test, 1));
        //$step1 = explode('<li>', $test);
        //$step2 = explode('</li>', $step1);
        //watchdog('test_tags', print_r($step1, 1));
    }
}