## Admin Index

When you visit admin/behat/index you will see the files you have access to.

The source of these files are 

 * Drupal's default folder public://behat_tests/
 * Any module with a behat_features folder and tests in there (read only)
 * Finally any module that hooks into the system to show files. For example this [github_behat_editor](https://github.com/alnutile/github_behat_editor) module will put files in **public://behat_github**. From there you can see files if they are part of your User Repo or Group's Repos. 



### Docs


#### Overview

 * [Writing](writing.html)
 * [Tags](tags.html)
 * [Add a Test](add.html)
 * [Delete](delete.html)
 * [Edit](edit.html)
 * [View](view.html)
 * [Batch Jobs](batch.html)
 * [Groups](groups.html)
 * [Repos/Github](repos.html)
 * [SauceLabs](saucelabs.html)
 * [URL Admin](urls.html)
 * [Drush Integration](drush.html)
 * [Cron job tests](cron.html)
 * [Settings Group](settings_group.html)
 * [Settings Overview](settings_all.html)
 * [Current Test](current_test.html)
 * [Choose OS and Browser](choose_os_browser.html)
 * [Past Results](past_results.html)
 
#### Steps

 * [Feature](feature.html)
 * [Filename](filename)
 * [Background Step](background.html)
 * [Scenario](scenario.html)
 * [Given URL](given.html) 
 * [Check box](checkbox.html)
 * [Press Click Element](press_click.html)
 * [Select Lists](select.html)
 * [Element Exists](exists.html) 
 * [URL Check](url_check.html)
 * [Field Value Check](field_value_check.html)
 * [Element Size](element_size.html)
 * [Response Code](status_code.html)
 * [Alert Window](alert_window.html)
 * [Remove Element](remove_element.html)
 * [Page Load Wait](page_load.html)
 * [Hover](hover.html)
 * [Check for Style on Element](check_element_style.html)
 * [Should See Cookie](cookie.html)
 * [Click Logout](logout.html)
 * [Switch back to window](switchback.html)
 * [Switch to Popup](switch2popup.html)
 * [Destroy Cookies](cookiesdestroy.html)
 * [Visible Element](element_visible.html)
 * [Waiting](waiting.html)
 * [Switch to Iframe Named](iframe_named.html)
 * [Switch to Iframe no name](no_name_frame.html)
 * [Submit by ID](submit_by_id.html)
 
 
#### Steps that may be removed soon
 * [Click a # Button](number_button.html)
 * [OnClick Handler](onlick.html)