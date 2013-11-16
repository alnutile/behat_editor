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
        $query->orderBy('b.sid', 'DESC');
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
        $query = db_select('behat_editor_base_url_settings', 'b');
        $query->fields('b');
        $query->condition('b.gid', $gids, 'IN');
        $query->orderBy('b.sid', 'DESC');
        $result = $query->execute();
        $rows = array();
        if ($result) {
            foreach ($result as $record) {
                $rows[] = (array) $record;
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