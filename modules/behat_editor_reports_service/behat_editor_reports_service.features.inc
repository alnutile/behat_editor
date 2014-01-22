<?php

/**
 * Implements hook_ctools_plugin_api().
 */
function behat_editor_reports_service_ctools_plugin_api($module = NULL, $api = NULL) {
    if ($module == "services" && $api == "services") {
        return array("version" => "3");
    }
}
