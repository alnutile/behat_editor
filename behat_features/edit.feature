Feature: Edit Page
  @javascript
  Scenario: Testing Edit Test Just made
    Given I get first test name
    And I edit first test
    Then I fill in "see_not_see_some_text" with "Hello Worlds Again"
    And I press "see_not_see"
    And I follow "Run Test"
    And I ponder life
    And I should see "Test successful!"
    And I follow savedTest
    And I should see "Hello worlds Again"
