# Behat Editor and Gherkin Generator
........
### About

This module will allow a user to make Gherkin formatted test files for behat.

The interface will allow tagging at both the feature and scenario level, multi scenarios and dragging
of tests so you can arrange the steps as you go.

This is a key guide [http://docs.behat.org/guides/1.gherkin.html] to get a sense of this format.


### Saucelabs Integration

This module lives at alnutile/behat_editor_saucelabs

## Setup

#### 1. Composer Manager

Make sure to setup Composer Manger

See [https://drupal.org/project/composer_manager] for installing Composer Manager first.

#### 2. Libraries

Some libraries could not be managed via Composer Manager.

Enable the behat_lib module

You can see how to get your Libraries here [admin/reports/status]

**or**

If using Drush just type 

```bash
drush bl 
```

and it will download the libraries for you.


#### 3. Behat Editor Module

Finally to enable the module.

After you download this repo go setup the module per drupal install steps.

If you enable this module via drush composer_manager will kick in and download the needed behat files.


#### 4. Behat.yml file

By default the system uses behat.yml.example to grab the default settings. You can make your own behat.yml and the system will pull from that instead to setup the default browser, FeatureContext.php path etc.

copy behat/behat.yml.example to behat/behat.yml

```bash
cp behat/behat.yml.sample behat/behat.yml
```

In this modules folder.

Edit/Update your behat/behat.yml file as needed. 

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

Though why do that when drush has a bunch of commands you can use see **DRUSH** below


**Keep in mind the dependencies in the info file
JqueryUpdate 1.7 or higher will be set**


#### 5. (finally!) FeatureContext.php.sample file

Rename the behat/features/bootstrap/FeatureContext.php.sample file 

to

behat/features/bootstrap/FeatureContext.php

Then you can add custom step definitions in there that your app may need.
This is kept out of git so that if there are updates to the module your custom work does not get lost.

[http://docs.behat.org/quick_intro.html#writing-your-step-definitions]



## What now

 * admin/behat/index to see all files
 * Click the Add to make a test.
 * Click on a file to View, Edit
 * Save a file to your modules directory

The module comes with an Example button so when you visit admin/behat/add you

can click the "Just click here to load example" link and load that test.

You should be able to run it and see the results on the right.

If there are any errors check out the drupal reports area under this modules (behat_editor) name.

Or see drush below and run some of the tests that come with it. See **DRUSH** below.

### Drush

Clear drush cach first

```bash
drush cc drush
```

Get help 
```bash
drush help br
```
Run a test
```bash
drush br module_name file.feature 1 0 0
```

This will run the test in javascript good for local tests
```bash
drush br module_name file.feature 1
```
This will run it and skip @javascript good for remote tests

```bash
drush brf module_name
```
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
For example you may need a longer wait time and the I wait is not good enough.

You would get a warning that the Step does not exists and behat would show you how
to make it.

You would then copy the vender/behat/behat/features/bootstrap/FeatureContex.php file
to behat/features/bootstrap in the behat_editor module's folder.
Then you would add that code there.

```bash
cd behat_editor
cp ../../vendor/behat/behat/features/bootstrap/FeatureContext.php behat/features/bootstrap/FeatureContex.php 
```


### Todo
lots


### Tips

There is a run_tests.php file. It triggers the full_tests.php test. It is a quick way
to run the tests for this module before committing code.
You can link it to your git pre-commit hook as well.
