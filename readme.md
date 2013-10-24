## Behat Editor and Gherkin Generator

...

### About

This module will allow a user to make Gherkin formatted test files for behat.
The interface will allow tagging at both the feature and scenario level, multi scenarios and dragging
of tests so you can arrange the steps as you go.
This is a key guide [http://docs.behat.org/guides/1.gherkin.html] to get a sense of this format.

Depending on the permissions of the user they can
 1. Run a test and see results
 2. Download the file from the /tmp folder of the system.
 3. Save the file to their modules gherkin_features folder
 4. Save it to a git repository
 5. Edit a file and Run it to verify it is working
 6. Edit a file and save it back to the system or /tmp folder for download.

### Saucelabs Integration
This module lives at alnutile/behat_editor_saucelabs

### Setup

After you download this repo go setup the module per drupal install steps.
See [https://drupal.org/project/composer_manager] for installing Composer Manager first.
you will need to have composer_manager installed so it can download behat into it's vendor directory.
If you enable via drush composer_manager will kick in and download the needed behat files.

Some libraries could not be managed via Composer Manager.
If using Drush just type drush bl and it will download the libraries for you.

copy behat/behat.yml.sample to behat/behat.yml
Update your behat/behat.yml file as needed. 

Also, if using selenium make sure you run it in the background. You can see more notes here 

[http://mink.behat.org/#seleniumdriver]

Download the jar file to your computer/server

```bash
curl -O http://selenium.googlecode.com/files/selenium-server-standalone-2.31.0.jar
```

Then start it at the command line

```bash
java -jar selenium-server-standalone-2.31.0.jar
```

So you should be able to run

```
vendor/behat/behat/bin/behat
```

from your drupal sites directory.

Keep in mind the dependencies in the info file
JqueryUpdate 1.7 or higher


## What now

 * admin/behat/index to see all files
 * Click the Add file there to add one.
 * Click on a file to View, Edit
 * Save a file to your modules directory

The module comes with an Example button so when you visit admin/behat/add you
can click the "Just click here to load example" link and load that test.
You should be able to run it and see the results on the right.

If there are any errors check out the drupal reports area under this modules name.

Or see drush below and run some of the tests that come with it.

### Drush

drush cc drush

drush br module_name file.feature 1

This will run the test in javascript good for local tests

drush br module_name file.feature 1
This will run it and skip @javascript good for remote tests

drush brf module_name
This will run all the files in that modules behat_features folder again 1 / 0 to turn Javascript on or off

### Hooking

This module will look for all modules that are enabled and see if they have a
gherkin_features folder.

If they do and the user has permissions then any test can be saved to that folder.
Later this will hook into the administration area as well to run tests.


You can add other types here hook_behat_editor_string_types
which later can include Scenario Overviews, Backgrounds etc.

And for parsing we can consider gherkins table and PyString features.

### FeatureContext.php

The FeatureContext.php can be extended if you need custom gherkin commands.
For example you may need a longer wait and the I wait is not good enough.

And I wait longer.

You would get a warning that the Step does not exists and behat would show you how
to make it.

You would then copy the vender/behat/behat/features/bootstrap/FeatureContex.php file
to behat/features/bootstrap in the behat_editor module's folder.
Then you would add that code there.

### Todo

Add submodule for behat and SauceLabs
Add info on install libraries or have drush do it or composer manager
