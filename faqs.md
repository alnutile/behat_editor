# FAQ

## Why can I not make banners rotate

Banners can work. One trick is that many banners do not rotate if the mouse is hovering over them, which it is the moment you click one.

So if possible make your next step

	And I fill in "search" with "test"

Or some other form field to get your mouse away from the banner


## Can I do Captcha

No. It would make this way to good as a spamming tool. We are working though
on ways to allow people to bypass captchas as needed when using this tool.


## Can I use xpath

Yes. Make sure to replace double quote with single quote.

So

	"//*[@id="knowpneumonia.com"]"

Becomes

	"//*[@id='knowpneumonia.com']"


## How do I add comments.

Version 2 of the tool will be better about this.

You just use the # symbol here is an example


	Feature: Show a comment
	  Scenario: Step 1
	  # And this step is not seen by the system
	  Given I am on "/"
	  # or you can just type a comment to help others read your test
	
## How can I get help

Tuesday and Thursday 11 EST there is an open conference session at https://join.me/alfrednutile come by and ask you questions.

Also there is a Feedback tab on the right of the site so send some info there as well.

## When do I use Tokens

They are a good way to build easy to read tests. Like this

	And I click "THE SAVE BUTTON'
	
Versus

	And I click the xpath "//*[@id='knowpneumonia.com']" 
	

Also they are a good way to reuse a test. Say one site is English and the other Spanish. You could write one test with tokens.

## Can I add custom steps

We are working on the best way to do this. Post some feedback to the Feedback tab and we can talk about how to work your steps into the site.

It really is easy overall. Behat is very extendable.

## I messed up my test can I recover it?

Yes, all tests are in git version control on Github. If you have access to that repo and branch just go there and did 


## What is Clone to Repo

## Can I name a file

## Can I make folders and subfolders 
