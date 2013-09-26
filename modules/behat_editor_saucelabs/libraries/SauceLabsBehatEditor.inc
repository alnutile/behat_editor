<?php

namespace BehatEditor\SauceLabs;
use WebDriver\SauceLabs\SauceRest;

class SauceLabsBehatEditor {
    public $username = '';
    public $api = '';

    public function __construct($username, $api) {
        $this->username = $username;
        $this->api = $api;
    }

    public function connect() {
        $res = new SauceRest($this->username, $this->api);
        return $res;
    }

    public function connectTest() {
        $res = self::connect();
        return $res->getAccountDetails($this->username);
    }

    public function jobsSummary() {
        $res = self::connect();
        $jobs = $res->getJobs(1);
        $jobs_trimmed = array_slice($jobs, 0, 5);
        $latest_id = $jobs[0]['id'];
        $lastest_job = $jobs[0];
        return array('count' => count($jobs), 'lastest_id' => $latest_id, 'jobs' => $jobs_trimmed, 'lastest_jobs' => $lastest_job);
    }

}