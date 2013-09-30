 @example @javascript
 Feature: Example Test for WikiPedia

   @example
   Scenario: WikiPedia
     Given I am on "http://en.wikipedia.org/wiki/Main_Page"
     And I fill in "search" with "Behavior Driven Development"
     And I press "searchButton"
     Then I should see "agile software development"
     And I follow "Donate to Wikipedia"
     Then I should see "Thanks"
