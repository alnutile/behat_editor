 
 Feature: Example Test for WikiPedia

   @anonymous
   Scenario: WikiPedia
     Given I am on "http://en.wikipedia.org/wiki/Main_Page"
     Then I should see "WikiPedia"
     And I follow "Donate"
     Then I should not see "Bob"
     Then I should see "Thanks"
