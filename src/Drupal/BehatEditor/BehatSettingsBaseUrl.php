<?php
/**
 * @file
 *   Get settings for user and groups
 */
namespace Drupal\BehatEditor;


class BehatSettingsBaseUrl {
    public $userObject;
    public $uid;
    public $groups;

    function __construct(){
        $this->fields = self::fields();
    }

    function getUsersSettings($uid) {
        $this->uid = $uid;
    }

    function getGroupSettingsForUser($uid, array $groups) {

    }

    static function userObject() {

    }

    static function getFields() {
        return self::fields();
    }

    static public function getSettingsByUID($uid) {
        $query = db_select('behat_editor_base_url_settings', 'b');
        $query->fields('b');
        $query->condition('b.uid', "$uid", '=');
        $query->condition('b.gid', 0, '=');
        $query->orderBy('b.nice_name', 'ASC');
        $result = $query->execute();
        $rows = array();
        if ($result) {
            foreach ($result as $record) {
                $rows[] = (array) $record;
            }
        }
        return array('results' => $rows, 'error' => 0);
    }

    static public function getSettingsByGID(array $gids) {
        $rows = array();
        if(!empty($gids)){
            $query = db_select('behat_editor_base_url_settings', 'b');
            $query->fields('b');
            $query->condition('b.gid', $gids, 'IN');
            $query->condition('b.gid', 0, '!=');
            $query->orderBy('b.nice_name', 'ASC');
            $result = $query->execute();
            $rows = array();
            if ($result) {
                foreach ($result as $record) {
                    $rows[] = (array) $record;
                }
            }
        }
        return array('results' => $rows, 'error' => 0);
    }

    static public function getSettingsBySID($sid) {
        $query = db_select('behat_editor_base_url_settings', 'b');
        $query->fields('b');
        $query->condition('b.sid', $sid, '=');
        $result = $query->execute()->fetchAssoc();
        return array('results' => $result, 'error' => 0);
    }

    public function insert() {
        $insert = db_insert('behat_editor_base_url_settings')->fields($this->fields)->execute();
        if($this->fields['default_url'] == 1) {
            self::resetDefault($insert);
        }
        return $insert;
    }

    public function update($sid) {
        $update = db_update('behat_editor_base_url_settings')
            ->fields($this->fields)
            ->condition('sid', $sid, '=')
            ->execute();
        return $update;
    }

    static function delete(array $sids) {
        db_delete('behat_editor_base_url_settings')
            ->condition('sid', $sids, 'IN')
            ->execute();
    }

    public function getBaseUrlFromSidArray($settings = array()) {
        $path_to_behat = FALSE;
        if($settings) {
            //Produce the base_url needed for this
            //  return group level first
            if($settings['base_url_gsid']) {
                $group_sid = self::getSettingsBySID($settings['base_url_gsid']);
                return $group_sid['results']['base_url'];
            } elseif($settings['base_url_usid']) {
                $user_sid = self::getSettingsBySID($settings['base_url_usid']);
                return $user_sid['results']['base_url'];
            } else {
                return FALSE;
            }

        }
        return $path_to_behat;
    }

    private function resetDefault($insert) {
        //Select all for this user and group = id in $this->fields['gid']
        // set default 0 unless it = this $insert sid
        $uid = $this->fields['uid'];
        $gid = $this->fields['gid'];
        $sid = $insert;

        $query = db_update('behat_editor_base_url_settings');
        $query->fields(array('default_url' => 0));
        if($gid != 0) {
            $query->condition('gid', "$gid", '=');
        } else {
            $query->condition('uid', "$uid", '=');
        }
        $query->condition('sid', $sid, '!=');
        $query->execute();
    }

    private function fields() {
        global $user;
        $fields['uid'] = $user->uid;
        $fields['gid'] = 0;
        $fields['base_url'] = NULL;
        $fields['default_url'] = 0;
        $fields['active'] = 1;
        $fields['nice_name'] = NULL;

        return $fields;
    }

}