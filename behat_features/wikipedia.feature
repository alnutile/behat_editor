 
 Feature: Example Test for WikiPedia
   Background: "BGG"
     Then I should not see "Muffins"
 
   Scenario: WikiPedia
     Given I am on "http://en.wikipedia.org/wiki/Main_Page"
     Then I should see "WikiPedia"
     And I follow "Donate"
     Then I should see "Thanks"
