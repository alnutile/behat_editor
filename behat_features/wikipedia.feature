@javascript
Feature: Example Test for WikiPedia

  Scenario: WikiPedia
    Given I am on "http://en.wikipedia.org/wiki/Main_Page"
    And I fill in "search" with "Behavior Driven Development"
    And I press "searchButton"
    Then I should see "BDD"
    And I follow "Donate to Wikipedia"
    Then I should see "Jimmy"

