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

There will be a submodule that will show how you can connect this to SauceLabs.

### Setup

After you download this repo go setup the module per drupal install steps
you will need to have composer_manager installed so it can download behat into it's vendor directory.
Behat [http://docs.behat.org/] explains how to run composer to install the module.
It relies on the Mink extension.

Also, if using selenium make sure you run it in the background.

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

after you are done.

The module comes with an Example button so when you visit node/add/gherkin-generator you
can click the "Just click here to load example" link and load that test.
You should be able to run it and see the results on the right.

If there are any errors check out the drupal reports area under this modules name.


### Hooking

This module will look for all modules that are enabled and see if they have a
gherkin_features folder.

If they do and the user has permissions then any test can be saved to that folder.
Later this will hook into the administration area as well to run tests.

There are other hooks as well.
Since gherkin allows for different languages you can add keys to the lists of keys that are checked for
as a new file is made or html is parsed

hook_behat_editor_string_feature

hook_behat_editor_string_scenario
--a feature can have multiple features

hook_behat_editor_string_tags
--later we can hook and limit tags as needed.

hook_behat_editor_string_steps

You can add other types here hook_behat_editor_string_types
which later can include Scenario Overviews, Backgrounds etc.

And for parsing we can consider gherkins table and PyString features.


### Todo

Add submodule for behat and SauceLabs
Add info on install libraries or have drush do it or composer manager