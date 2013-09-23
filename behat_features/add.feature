@javascript
Feature: Add Page

  Scenario: User Sees
    Given I am on "/admin/behat/add"
    Then I should see "This is a tool to help to generate "


  Scenario: User should see questions form
    Given I am on "/admin/behat/add"
    Then I should see "When I go to"


  Scenario: User should see Results area
    Given I am on "/admin/behat/add"
    Then I should see "Your results will show here..."


  Scenario: User fills in form
    Given I am on "/admin/behat/add"
    And I fill in "form_field_and_fill_in" with "name"
    And I fill in "field_with_text" with "Hello World"
    And I press "edit-field-text-button"
    And I fill in "field_with_text" with "Goodbye World"
    Then I wait
    Then I should see "Hello World"


  @anonymous
  Scenario: Anonymous user adds 2nd scenario
    Given I am on "/admin/behat/add"
    And I follow "click here"
    And I wait
    And I should see "Scenario: WikiPedia"
    And I fill in "name" with "Mink Rocks"
    And I press "Name it"
    Then I should see "Scenario: Mink Rocks"
    And I should see "Scenario: WikiPedia"

  @anonymous
  Scenario: Anonymous user adds and looks at file
    Given I am on "/admin/behat/add"
    Then I should see "This is a tool to help"
    And I follow "click here"
    And I wait
    And I wait
    And I follow "Run Test"
    Then I wait
    Then I wait
    Then I should see "File created"
    And I follow savedTest
    Then I should see "Scenario: WikiPedia"
    Then I should not see "This is a tool to help"

  @anonymous
  Scenario: Anonymous user adds feature tag
    Given I am on "/admin/behat/add"
    Then I should see "This is a tool to help"
    And I fill in "name" with "Mink Rocks"
    And I press "Name it"
    And I fill in featuresTag
    And I follow "Run Test"
    And I wait
    Then I should see "File created"
    And I follow savedTest
    Then I should see "@tag1"
    Then I should not see "This is a tool to help"

  @anonymous
  Scenario: 2 Tags 2 Scenarios
    Given I am on "/admin/behat/add"
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

@anonymous
Scenario: User runs a test
  Given I am on "/admin/behat/add"
  And I fill in "name" with "Run Test Test"
  And I fill in "url" with "/admin/behat/add"
  And I fill in "see_not_see_some_text" with "Success"
  And I follow "Run Test"
  And I wait
  And I should see "Test successful!"
