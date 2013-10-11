<?php
/**
 * @file
 * behat_editor.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function behat_editor_ctools_plugin_api() {
    list($module, $api) = func_get_args();
    if ($module == "services" && $api == "services") {
        return array("version" => "3");
    }
}
