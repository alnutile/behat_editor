# Github Workflow

The test files are stored in git. You can have numerous repos setup.

So in the admin interface you might see



	|Repo Name | Repo Branch | Related Group |View Tests |
	|----------|-------------|---------------|-----------|
	| repo_foo |    master   |    Team Foo   |   view    |
	| repo_foo |    dev      |    Team Foo   |   view    |
	| repo_foo |    prod     |    Team Foo   |   view    |



Assuming you are on Team Foo that team has 3 entries for 1 repo.
Each repository is set up to use a different branch.

If you were to look at the admin interface for editing or setting up a repository it is there that you can choose the branch and the folder. Actually in the admin interface seen above you can even edit it

With  this set up all of teams can work locally on different branches/projects and then push to origin. The website will then show the files in that branch so you can run a test or see on the tests  in the related reports.