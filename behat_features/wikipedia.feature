 @external
 Feature: Example Test for WikiPedia

   @anonymous @external
   Scenario: WikiPedia
     Given I am on "http://en.wikipedia.org/wiki/Main_Page"
     Then I should see "WikiPedia"
     And I follow "Donate"
     Given I ponder life
     And I ponder life
     Then I should not see "Bob"
     Then I should see "Thanks"
     Given I ponder life for 30 seconds
     And I fill in "Test1" with "Test2"

   @admin
   Scenario: "Test this is the new group"
     Given I click the  "facebook"  social button
     Given I am on the  "test "  page
     Given I change the text size to  "10"
