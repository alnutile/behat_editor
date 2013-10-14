@javascript
Feature: Add Page

  Scenario: Log In
    Given I am on "/user/logout"
    Then I am on "/user/login"
    And I fill in "Username" with "admin"
    And I fill in "Password" with "password"
    And I press "Log in"
    And I ponder life

  Scenario: User Sees
    Given I am on "/admin/behat/add"
    Then I should see "This is a tool to help to generate "


  Scenario: User fills in form
    Given I am on "/admin/behat/add"
    And I fill in "form_field_and_fill_in" with "name"
    And I fill in "field_with_text" with "Hello World"
    And I press "edit-field-text-button"
    And I fill in "field_with_text" with "Goodbye World"
    Then I wait
    Then I should see "Hello World"


  Scenario: User adds 2nd scenario
    Given I am on "/admin/behat/add"
    And I follow "click here"
    And I wait
    And I should see "Scenario: WikiPedia"
    And I fill in "name" with "Mink Rocks"
    And I press "Name it"
    Then I should see "Scenario: Mink Rocks"
    And I should see "Scenario: WikiPedia"

  Scenario: User adds and looks at file
    Given I am on "/admin/behat/add"
    Then I should see "This is a tool to help"
    And I follow "click here"
    And I wait
    And I follow "Run Test"
    Then I wait
    Then I wait
    Then I should see "Test successful"
    And I follow savedTest
    Then I should see "Scenario: WikiPedia"
    Then I should not see "This is a tool to help"

  Scenario: Anonymous user adds feature tag
    Given I am on "/admin/behat/add"
    Then I should see "This is a tool to help"
    And I fill in "name" with "Mink Rocks"
    And I press "Name it"
    And I fill in featuresTag
    And I follow "Run Test"
    And I wait
    Then I should see "Test successful!"
    And I follow savedTest
    Then I should see "@local"
    Then I should not see "This is a tool to help"

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
    Then I should see "Test successful!"
    And I follow savedTest
    Then I should see "@anonymous"
    Then I should not see "This is a tool to help"

Scenario: User runs a test
  Given I am on "/admin/behat/add"
  And I fill in "name" with "Run Test Test"
  And I fill in "url" with "/admin/behat/add"
  And I fill in "see_not_see_some_text" with "Success"
  And I follow "Run Test"
  And I wait
  And I should see "Test successful!"
