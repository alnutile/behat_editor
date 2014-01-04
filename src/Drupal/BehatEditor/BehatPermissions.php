<?php
/**
 * @file
 *   Get permissions of the user relative to
 *   Settings and other behat editor items.
 */

namespace Drupal\BehatEditor;


class BehatPermissions {
    public $userGids;
    public $uid;
    public $groups;

    function __construct($uid) {
        $this->uid = $uid;
        $this->groups = array();
        $this->userGids = array();
    }

    public function getGroupIDs() {
        if(module_exists('og')) {
            $this->groups = og_get_entity_groups('user', $this->uid);
            if($this->groups) {
                $this->groupIdFromNode();
            }
        }
        return $this->userGids;
    }

    private function groupIdFromNode() {
        $groups = array();
        if(isset($this->groups['node'])) {
            $groups = array_combine(array_values($this->groups['node']), array_values($this->groups['node']));
        }
        return $this->userGids = $groups;
    }



}