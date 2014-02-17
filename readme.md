# Behat Editor and Gherkin Generator

## About

This module will allow a user to make Gherkin formatted test files for behat as well as centralize reports of tests. So even if the graphical interface is not needed to build the tests you can still use the site to run and review tests. 

A key guide can be found here [http://docs.behat.org/guides/1.gherkin.html] to get a sense of the Gherkin and BDD format.


## Install/Setup

**PHP VERSION**

This module requires php 5.4 http://www.php.net/downloads.php

Commands for setting up php 5.4 on Ubuntu Precise Pangolin (12.04 LTS):

```bash
 sudo add-apt-repository -y ppa:ondrej/php5-oldstable
```

```bash
 sudo apt-get update
```

```bash
 sudo apt-get install
```

**NOTE**

There is a dependency to Organic Groups that was not made clear in the install file. You need to setup OG and one group node say "Basic Page" otherwise you may get some errors. Issue #98

This module needs composer_manager to setup a number of libraries. For those not able to be managed here it also has a sub module called behat_lib to help setup those libraries.

### 1. Libraries


Some libraries could not be managed via Composer Manager.

Enable the behat_lib module

You can see how to get your Libraries here [admin/reports/status]

**or**

If using Drush just type 

```bash
drush bl 
```

and it will download the libraries for you.

##### NOTE: you need this one module as well. Will move it into the composer.json soon
https://github.com/alnutile/simple_noty

### 2. Composer Manager

Make sure to setup Composer Manger

See [https://drupal.org/project/composer_manager] for installing Composer Manager first.

### 3. Behat Editor Module

Finally to enable the module.

After you download this repo go setup the module per drupal install steps.

If you enable this module via drush composer_manager will kick in and download the needed behat files.

After that you can optionally copy the behat/behat.yml.example file to behat/behat.yml and change it if needed.

The example file will be used for defaults if that file is not there. 

```bash
cp behat/behat.yml.example behat/behat.yml
```

**Keep in mind the dependencies in the info file JqueryUpdate 1.7 or higher will be set**


### 4. Run the Selenium Server

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


## Adding Your Tests

There are three ways right now to add a test

 1. visit admin/behat and click add
 
> This will place a test in your sites/default/file/behat_tests folder for
> you to edit or run as needed.

 2. visit admin/behat and upload a test

> Same as above

 3. make a module

> Your module just needs a folder behat_features and those files will be 
> seen at admin/index. You can not EDIT those tests since most servers do not have write 
> level at that folder level.

  4. GitHub
  
> Coming soon…



## Saucelabs Integration

This module lives at http://github.com/alnutile/behat_editor_saucelabs


## What now

### Other modules
Also checkout https://github.com/alnutile/behat_editor_saucelabs
and https://github.com/alnutile/behat_cron_runner

### Visit on your site

 * admin/behat/index to see all files
 * click Behat Settings and start adding URLs since it is good to use relative URLs in your tests. 
 * Click the Add to make a test.
 * Click on a file to View, Edit
 * Save a file to your modules directory

The module comes with an Example button so when you visit admin/behat/add you

can click the "Just click here to load example" link and load that test.

You should be able to run it and see the results on the right.

If there are any errors check out the drupal reports area under this modules (behat_editor) name.

Or see drush below and run some of the tests that come with it. See **DRUSH** below.

### Drush

Clear drush cache first

```bash
drush cc drush
```


Find the SID of the url you are testing

```bash
drush bus 1 
```
or
```bash
drush help bus
```

Get help 
```bash
drush help br
```

Run a test
```bash
drush br module_name file.feature 1 0 0 1
```

The above will run the test in javascript good for local tests for URL #1

```bash
drush br module_name file.feature 1 0 0 1
```
The above will run it and skip @javascript good for remote tests for URL #1

```bash
drush brf module_name
```
This will run all the files in that modules behat_features folder again 1 / 0 to turn Javascript on or off

### Hooking

See documentation at http://dspeak.com/drupalextension/ for how to include your own custom steps.

After that you need to hook_form_alter the View, Add, Edit form to add your steps there for the user if needed. The layout of those Form Fields is key for the javascript to automatically parse them.


Also your module can alter the yml creation by using this 
```
hook_behat_editor_yml_array_alter
```

A good example of this can be seen at [https://github.com/alnutile/behat_editor_saucelabs/blob/master/behat_editor_saucelabs.module#L213]

```
behat_editor_saucelabs_behat_editor_yml_array_alter
````

Finally it can alter the command by using the
```
hook_behat_editor_command_alter
```
You can see a good example below

```php
function behat_editor_saucelabs_behat_editor_command_alter(&$command, $context1){
    //context behat_run we leave alone for now
    if($context1 == 'behat_run_saucelabs') {
	  array('@context' => $context1)));
        $command['profile'] = "--profile=saucelabs";
    }
}
```

In this example we look for the context and then we add to the command the different switches we would like to alter.

You can see the root of this in the src/Drupal/BehatEditor/BehatEditorRun.php file under the methods exec and execDrush


## RoadMap

 * Better use of Services for separate CRUD/I array for Results, Tests et. Right now Results is using Targeted Actions.
 * Simple Noty install via the behat_lib or composer_manage modules
 * Properly Formatted Feature with the following 3 lines Benefit, Role and Description lines. 
 * Tables in Tests
 * Large Text Blocks in Tests
 * Github to pull from a repo your tests to read and run.



### Todo
lots


### Tips

There is a run_tests.php file. It triggers the full_tests.php test. It is a quick way
to run the tests for this module before committing code.
You can link it to your git pre-commit hook as well.
