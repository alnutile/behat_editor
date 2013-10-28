#!/usr/bin/php
<?php
/**
 * This can be used in your git pre-commit just go to the root of 
 * your project and type at the command line
 * ln -s ../../run_behat_tests.php .git/hooks/pre-commit
 * Mac users can get sound by using the say command around line 20.
 * 
*/

define("WORKING_PATH", '/Users/alfrednutile/Drupal/pfizertestinstall/sites/all/modules/custom/behat_editor');
define("RUN_TEST", 'full_test.feature');
chdir(WORKING_PATH);
exec('git pull origin development', $output, $return_var);
exec("drush br behat_editor " .RUN_TEST. " 1 0 0", $output, $return_var);
if ( !$return_var ) {
	$message = "Failed Tests " . RUN_TEST;
} else {
	$message = "Passed Tests " . RUN_TEST;
}

shell_exec('say "'.$message.'"');

print $message;
print implode("\n", $output);
