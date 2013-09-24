Feature: Edit Page
  @javascript
  Scenario: User clicks link to see view
    Given I am on "/admin/behat/view/behat_editor/aa_mock.feature"
    And I follow "Edit Test"
    And I wait
    Then I should see "Editing: aa_mock.feature"
    And I should see "Your results will show here..."
    And I should see "Feature:"
    And I should see "User clicks link"
    And I should see "Save Test"

  @javascript
  Scenario: User runs a test
    Given I am on "/admin/behat/edit/behat_editor/aa_mock.feature"
    And I follow "Run Test"
    And I wait
    And I wait
    And I should see "Test successful!"

  @javascript
  Scenario: 2 Tags 2 Scenarios
    Given I am on "/admin/behat/edit/behat_editor/aa_mock.feature"
    Then I should see "This is a tool to help"
    And I fill in featuresTag
    And I fill in "name" with "Mink Rocks"
    And I press "Name it"
    And I fill in sectionOneTag
    And I fill in "name" with "Testing Rocks"
    And I press "Name it"
    And I fill in sectionTwoTag
    And I follow "Run Test"
    And I wait
    Then I should see "File created"
    And I follow savedTest
    Then I should see "@tag1"
    Then I should see "@tag2"
    Then I should not see "This is a tool to help"