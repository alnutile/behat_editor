## Admin Index

When you visit admin/behat/index you will see the files you have access to.

The source of these files are 

 * Drupals default folder public://behat_tests/
 * Any module with a behat_features folder and tests in there (read only)
 * Finally any module that hooks into the system to show files. For example this https://github.com/alnutile/github_behat_editor module will put files in public://behat_github. From there you can see files if they are part of your User Repo or Group's Repos. 
 
 
