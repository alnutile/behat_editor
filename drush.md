## Drush


You can see the video here 
[Drush Video](http://youtu.be/bFx3pxPkvVo)

Drush is a command line interface for drupal

You can use drush --help to see all the commands for behat

the behat_editor module will allow you to run tests from the command line.

You will need to use

drush bus 1 

to get the URLs for user 1

or

drush bug 3

to get the URLs for group 3

once you have that you can run a test


Do 

drush br --help 

to see all the switches needed to run a test via drush.


Your final command may end up like this


drush br behat_tests 'behat_tests/wikipedia.feature' 1 0 default 0 74

This would run the wikipedia.feature test in the behat_tests module folder with javascript on (1) no specific tags (0) default profile 0 URL id and 74 for the ID that group 3 has for this test.

The video will help more though :)