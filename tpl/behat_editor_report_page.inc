<?php
/**
 * @file
 *   Show reports for pages
 */

function behat_editor_report() {
    $arg = arg(3);
    if(!empty($arg)) {
        //Remove first 3
        // use remaining to query for test results
        // make output
        $output = "Hello";
        dpm(arg());
        return $output;
    } else {
        return drupal_not_found();
    }
}