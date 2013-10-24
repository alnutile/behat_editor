<?php


/**
 * Implements hook_drush_command().
 */
function behat_libraries_drush_command() {
    $items = array();

    $items['behat-libraries'] = array(
        'callback' => 'drush_behat_libraries',
        'description' => dt("Download all the needed libraries"),
        'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
        'arguments' => array(
            'path' => dt("If you do not want to use the default library path")
        ),
        'aliases' => array('bl')
    );
    return $items;
}

/**
 * Thanks to https://drupal.org/node/1231378
 */
function drush_behat_libraries(){
    if (!drush_shell_exec('type unzip')) {
        return drush_set_error(dt('Missing dependency: unzip. Install it before using this command.'));
    }

    $args = func_get_args();
    if ($args[0]) {
        $path = $args[0];
    }
    else {
        $path = 'sites/all/libraries';
    }

    // Create the path if it does not exist.
    if (!is_dir($path)) {
        drush_op('mkdir', $path);
        drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
    }

    // Set the directory to the download location.
    $olddir = getcwd();
    chdir($path);

    $libraries = array(
        'tag_it' => array('url' => 'https://codeload.github.com/aehlke/tag-it/zip/master'),
        'data_table' => array('url' => 'http://datatables.net/releases/DataTables-1.9.4.zip'),
        'date_format' => array('url' => 'https://codeload.github.com/alnutile/jacob_wright_date_format/zip/master')
    );

    foreach($libraries as $key => $value) {
        $dirname = 'behat_editor_' . $key;
        //Check if installed
        if(!is_dir($dirname)) {
            //Make folder
            drush_op('mkdir', $dirname);
            chdir($dirname);
            //Get zips
            if (!drush_shell_exec('wget ' . $libraries[$key]['url'] . ' -O file.zip')) {
                drush_shell_exec('curl ' . $libraries[$key]['url'] . ' > file.zip' );
            }
            //Unzip and 'remove zip
            if (is_file('file.zip')) {
                drush_shell_exec('unzip -qqj -o file.zip');
                drush_op('unlink', 'file.zip');
            }
            //Down go back to the root library folder
            chdir('../');

        }


        print $libraries[$key]['url'];
        print $key;
    }

}