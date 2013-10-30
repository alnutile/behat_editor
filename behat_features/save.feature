 @javascript
 Feature: Saving only not run
 
   Scenario: User can click Save
     Given I am on "/admin/behat/add"
     And I follow "click here"
     And I wait
     And I should see "Scenario: WikiPedia"
     And I fill in "name" with "Mink Rocks"
     And I press "Name it"
     Then I should see "Scenario: Mink Rocks"
     And I should see "Scenario: WikiPedia"
     Then I follow "Save Test"
     And I wait
     Then I should see "File created"
