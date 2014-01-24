<?php

namespace Drupal\BehatEditorReports;
use Drupal\BehatEditor;

/**
 * @TODO limit by user and group
 */

class BehatServiceReportsModel {
    public $settings;
    protected $browserPassFailCount = array();
    protected $passFailChart = array();
    protected $passFailPerUrl = array();

    public function __construct($settings = null) {
        if(empty($settings)) {
            $this->settings = new BehatEditor\BehatSettingsBaseUrl();
        } else {
            $this->settings = $settings;
        }
    }

    //Get all results
    // paginate as at 100
    public function get_all($parameters = array()) {
        list($page, $params) = $parameters;

        $browserPassFailCount = array();
        $all_browsers = array();
        $all_users = array();
        $all_urls = array();
        $query = db_select('behat_editor_results', 'b');
        $query->fields('b');
        if(isset($params['browser']) && $params['browser'] != 'all') {
            $query->condition('settings', "%{$params['browser']}%", 'LIKE');
        }
        if(isset($params['user_id']) && $params['user_id'] != 'all') {
            $query->condition('uid', "{$params['user_id']}", '=');
        }
        if(isset($params['pass_fail']) && $params['pass_fail'] != 'all') {
            $query->condition('status', "{$params['pass_fail']}", '=');
        }
        if(isset($params['filename']) && $params['filename'] != 'all') {
            $query->condition('filename', "%{$params['filename']}%", 'LIKE');
        }
        if(isset($params['url']) && $params['url'] != 'all') {
            $url = explode('|', $params['url']);
            $base_url_usid = $url[0];
            $base_url_gsid = $url[1];
            $query->condition('settings', "%{$base_url_usid}%", 'LIKE');
            $query->condition('settings', "%{$base_url_gsid}%", 'LIKE');
        }
        $query->groupBy('filename');
        $query->range(0, 300);
        $query->orderBy('b.created', 'DESC');
        $result = $query->execute();
        $rows = array();
        if ($result) {
            foreach ($result as $record) {
                $record->results = unserialize($record->results);
                $record->settings = unserialize($record->settings);
                $base_url_gsid = (isset($record->settings['base_url_gsid'])) ? $record->settings['base_url_gsid'] : 0;
                $base_url_usid = (isset($record->settings['base_url_usid'])) ? $record->settings['base_url_usid'] : 0;
                //because I serialized the data
                //I need to do an extra check on results
                $url_state = ( ($base_url_gsid == $url[1] && $base_url_usid == $url[0]) || $params['url'] == 'all') ? TRUE : FALSE;
                if( $url_state ) {
                    $browser = (isset($record->settings['browser_version'])) ? $record->settings['browser_version'] : 'undefined';
                    $this->setBrowsersPassFail($browser, $record);
                    $this->setPassFailChart($record);
                    $record->settings['browser_version'] = $this->getBrowser($browser);
                    $all_browsers[$browser] = $record->settings['browser_version'];
                    $user = $this->getUser($record->uid);
                    $url_found = $this->getUrl($user['uid'], $base_url_usid, $base_url_gsid);
                    (empty($url_found['nice_name'])) ? $url_found['nice_name'] = 'undefined' : null;
                    $all_urls[$url_found['nice_name']] = $base_url_usid.'|'.$base_url_gsid;
                    $this->setPassFailPerURL($url_found['nice_name'], $record);
                    $all_users[$user['uid']] = $user['mail'];
                    $record->settings['url'] = $url_found['nice_name'];
                    $tags = $this->getTags($record->results);
                    $rows[] = (array) $record;
                }
            }
        }
        return array(
            'results' => $rows,
            'error' => 0,
            'browsers' => $all_browsers,
            'users' => $all_users,
            'urls' => $all_urls,
            'browser_pass_fail_count' => $this->browserPassFailCount,
            'pass_fail_chart' => $this->passFailChart,
            'pass_fail_per_url' => $this->passFailPerUrl
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

    protected function getUrl($uid, $base_url_usid, $base_url_gsid) {
        if($base_url_usid == 0) {
            $settings = $this->settings->getSettingsBySID(array($base_url_gsid));
        } else {
            $settings = $this->settings->getSettingsBySID(array($base_url_usid));
        }
        return $settings['results'];
    }

    protected function getTags($test) {
        //watchdog('test_tags_coming_in', print_r($test, 1));
        //$step1 = explode('<li>', $test);
        //$step2 = explode('</li>', $step1);
        //watchdog('test_tags', print_r($step1, 1));
    }
}