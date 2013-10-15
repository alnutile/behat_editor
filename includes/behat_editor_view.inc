<?php
use Drupal\BehatEditor\File;

/**
 * @file File for viewing the Tests
 * You can see the related js file as well in the js folder
 */


function behat_editor_view_form_service($module, $filename) {
    watchdog('test_view', print_r("You are at view", 1));
    return array(1, 2, 3);
}