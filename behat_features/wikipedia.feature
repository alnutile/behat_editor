 @javascript
 Feature: Example Test for WikiPedia
 
   Scenario: WikiPedia
     Given I am on "http://en.wikipedia.org/wiki/Main_Page"
     Given I hover over the "Test" menu item
     Then I should see "WikiPedia"
     And I follow "Donate"
     Then I should see "Thanks"
     Then I should not see "Muffins"
