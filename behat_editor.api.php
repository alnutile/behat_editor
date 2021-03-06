<?php

/**
* @file Documentation about BehatEditor alter hooks.
*/

/**
* Allow to alter arguments before they are passed to service callback.
*
* @param $files_array
*   Files found
* @param $context
*   Context of the call eg "public" folder area or "module" folder area etc
* @param $options
*
* @see _buildArrayOfAvailableFilesInPublicFolders()
* @see FileModel.php
*/
function hook_behat_editor_files_found_alter($files_array, $context) {

}

/**
 * Allow to alter arguments before they are passed to service callback.
 *
 * @param $files_array
 *   output of the test
 *   return_var eg pass fail
 *   settings
 *   filename
 *   module
 * @param $context
 *   Context of the call eg "behat_run", "behat_batch", "behat_saucelabs" etc
 * @param $options
 *
 * @see exec()
 * @see BehatEditorRun.php
 */
function hook_behat_editor_results_alter($results_params, $contenxt) {

}

/**
 * Hook to alter the command array that will later be parsed into the
 * command string for behat
 *
 * @param $command
 * @param $context1
 *
 * @see exec
 * @see BehatEditorRun.php
 */
function hook_behat_editor_command_alter(&$command, $context1){
}



/**
 * Hook to alter the results of the Results table query
 * since filename and module are key we may have to do a double
 * check if the "module" being searched is a public subfolder
 *
 * @param $rows from the results
 * @param $params
 *   filename
 *   module
 *   file_object
 *
 * @see getLatestResultForFile
 * @see Results.php
 */
function hook_behat_editor_results_per_file_alter(&$command, $context1){
}


