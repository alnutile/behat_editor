<?php

/**
 * @file
 * Contains \Drupal\BehatEditor\BehatEditorRun.
 */

namespace Drupal\BehatEditor;

/**
 * Class Results
 * Methods to save results
 *
 * @todo maybe make an abstract Results class
 *   to deal with the slight differences in Results and ResultsBatch
 */

class Results {
    public $fields = array();
    public $results_cleaned;
    public $duration;
    protected $settings_getter;

    public function __construct(BehatSettingsBaseUrl $settings_getter = null) {
        global $user;
        if ( $settings_getter === 'null') {
            $this->settings_getter = new BehatSettingsBaseUrl();
        } else {
            $this->settings_getter = $settings_getter;
        }
        $this->fields = array(
            'filename' => '',
            'module' => '',
            'results' => '',
            'duration' => '',
            'created' => '',
            'status' => '',
            'uid' => $user->uid,
            'settings' => array(),
            'base_url_sid' => 0
        );
    }

    public function insert() {
        $this->fields['base_url_sid'] = $this->getSid($this->fields['settings']);
        $insert = db_insert('behat_editor_results')->fields($this->fields)->execute();
        return $insert;
    }

    public function updateByRid($params) {
        $this->fields['base_url_sid'] = $this->getSid($this->fields['settings']);
        $update = db_update('behat_editor_results')
            ->fields($params['fields'])
            ->condition('rid', $params['rid'], '=')
            ->execute();
        return $update;
    }

    static public function getResultsForFile($module, $filename) {
        $query = db_select('behat_editor_results', 'b');
        $query->fields('b');
        $query->condition('b.filename', $filename, 'LIKE');
        $query->condition('b.module', $module, 'LIKE');
        $query->orderBy('b.created', 'DESC');
        $result = $query->execute();
        $rows = array();
        if ($result) {
            foreach ($result as $record) {
                //@todo new records my not need this so do a check
                $record->results = unserialize($record->results);
                $record->settings   = unserialize($record->settings);
                self::getURLResults($record);
                self::getUserResults($record);
                $rows[] = (array) $record;
            }
        }
        return array('results' => $rows, 'error' => 0);
    }

    static public function getLatestResultForFile($module, $filename, $file_object = array(), $allow_alter = TRUE) {
        $query = db_select('behat_editor_results', 'b');
        $query->fields('b');
        $query->condition('b.filename', $filename, 'LIKE');
        $query->condition('b.module', $module, 'LIKE');
        $query->range(0, 1);
        $query->orderBy('b.created', 'DESC');
        $result = $query->execute();
        $rows = array();
        if ($result) {
            foreach ($result as $record) {
                $record->results = unserialize($record->results);
                $record->settings   = unserialize($record->settings);
                self::getURLResults($record);
                self::getUserResults($record);
                $rows[] = (array) $record;
            }
        }
        if($allow_alter) {
            $params = array('filename' => $filename, 'module' => $module, 'file_object' => $file_object);
            drupal_alter('behat_editor_results_per_file', $rows, $params);
        }

        return array('results' => $rows, 'error' => 0);
    }

    //@TODO why is this static?
    static public function getResultsForRids(array $rids) {
        $query = db_select('behat_editor_results', 'b');
        $query->fields('b');
        $query->condition('b.rid', $rids, 'IN');
        $query->orderBy('b.created', 'DESC');
        $result = $query->execute();
        $rows = array();
        if ($result) {
            foreach ($result as $record) {
                $record->results    = unserialize($record->results);
                $record->settings   = unserialize($record->settings);
                self::getURLResults($record);
                self::getUserResults($record);
                $rows[]             = (array) $record;
            }
        }
        return array('results' => $rows, 'error' => 0);
    }

    protected function getURLResults(&$record) {
        $settings_getter = new BehatSettingsBaseUrl();
        $url = $settings_getter->getSettingsBySID(array($record->base_url_sid));
        $record->url        = $url['results'];
    }

    protected function getUserResults(&$record) {
        $user = user_load($record->uid);
        $record->user = $user;
    }

    public function cleanHtml($results){
        $results_imploded = implode("", $results);
        $s = strpos($results_imploded, '<body>') + strlen('<body>');
        $f = '<div class="switchers">';
        $results_html = trim(substr($results_imploded, $s, strpos($results_imploded, $f) - $s)) . "</div>";
        $this->results_cleaned = $results_html;
    }

    public function getDuration() {
        $s = strpos($this->results_cleaned, '<p class="time">') + strlen('<p class="time">');
        $f = 's</p>';
        $duration = trim(substr($this->results_cleaned, $s, strpos($this->results_cleaned, $f) - $s));
        $this->duration = (!empty($duration)) ? $duration : '0';
        //One last check since the duration can sometimes be wrong if there is an error during the test
        if (strlen($this->duration) >= 20) { $this->duration = '0'; }
        return $this->duration;
    }

    static function generateHTMLOutput($results_array) {
        $results_message = array_slice($results_array, -3);
        $results_message_top = array_slice($results_array, 0, -3);
        $output_item_results = theme('item_list', $var = array('title' => 'Summary', 'items' => $results_message));
        $output_item_list = theme('item_list', $var = array('title' => 'All Results', 'items' => $results_message_top));
        return $output_item_results . $output_item_list;
    }

    public function prepareResultsAndInsert($output, $return_var = 0, $settings = array(), $filename, $module){
        //Clean Output
        self::cleanHtml($output);
        $this->fields['filename'] = $filename;
        $this->fields['module'] = $module;
        $this->fields['results'] = serialize($this->results_cleaned);
        $this->fields['duration'] = $this->getDuration();
        $this->fields['created'] = REQUEST_TIME;
        $this->fields['status'] = $return_var;
        $this->fields['base_url_sid'] = $this->getSid($settings);
        $this->fields['settings'] = serialize($settings);

        drupal_alter('behat_editor_save_results', $saveResults);
        return array('rid' => self::insert(), 'clean_results' => $this->results_cleaned, 'duration' => $this->getDuration());
    }

    /**
     * Dealing with old api that stored sid of url in
     * settings
     */
    public function getSid($settings) {
        if(!empty($settings)) {
            $settings = unserialize($settings);
            if(isset($settings['base_url_gsid']) && $settings['base_url_gsid'] > 0) {
                $sid = $settings['base_url_gsid'];
            } else {
                $sid = $settings['base_url_usid'];
            }
        } else {
            $sid = 0;
        }
        if($sid == null || $sid == '' || !is_numeric($sid)) {
            $sid = 0;
        }
        return $sid;
    }

}