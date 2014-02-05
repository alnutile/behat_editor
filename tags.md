## Tags

There is a video here
[Tags](http://www.youtube.com/watch?v=f-330tQBe2E)

Tags are key to organizing tests. But tags also trigger built in features

####Example tags that offer functionality

@javascript will trigger javascript on pages that have javascript. If this was not set and we where not using SauceLabs which defaults to JS then all the tests would fail.

@mink:goutte can turn off javascript for a moment if you need to test a lower level test. Search for this in admin/index to find the example test.

@private will hide the view and edit mode from the admin ui.

@critical will put the test into a queue so that they will be run during a cron job (coming soon)

####Organizing

Tags will help to unify tests. One way is by url. Since we are not putting a URL in the test we can unify the tests under a domain. eg

@google.com

Then all tests with that tag would be about that domain and can be run as a batch [See Batch](batch.html)

You can have more than one tag and search by that combination.

Tags can be key since filenames do not really play a part in running batcxh jobs. So using tags to organize your files will be the real goal.

###Tag limitations

There is a module in the modules folder of behat_editor call behat_editor_limit_tags that limits tags. Otherwise you can add any tag you want.




## Read More

[docs.behat.org](http://docs.behat.org/guides/1.gherkin.html#tags)


