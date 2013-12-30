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