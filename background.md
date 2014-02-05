##Background

![Background Image](images/background.png)


**You can see a video here [Backgrounds Video](http://youtu.be/hAEPfTAZG20)**

--
A test can have a Background (just one). This will run before each Scenario. 

This is a good place to do logins, logouts etc.

There are some custom steps you can apply here and some drush integration

	Feature: Your Overall Test name like User Interaction

  	  Scenario: User Edits a page
  	  Scenario: User Deletes a page
  

A scenario can have tags (more on tags)[http://alnutile.github.io/behat_editor/tags.html] 

So then this would be tagged @javascript, note this only applies to the one Scenario.


	Feature: Your Overall Test name like User Interaction

  	  @javascript
  	  Scenario: User Edits a page
  
  	  Scenario: User Deletes a page
  

###Review of the Background Video


--
Some pages you need to run a behat "background".

This background will run before every scenerio.


####Example: Login Background 


* Put that on top of the scenerio

* It needs a path as well

* So we could give it the same path or the log in path whatever your situation needs

* Then you could use it to start filling in form fields

##### Example: 'User Name' with 'test user' 



More Reading

[docs.behat.org](http://docs.behat.org/guides/1.gherkin.html#backgrounds)

[drush and behat](drush.html)





