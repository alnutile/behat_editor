@javascript
Feature: Add Page

  Scenario: Log In
    Given I am on "/user/logout"
    Then I am on "/user/login"
    And I fill in "Username" with "admin"
    And I fill in "Password" with "password"
    And I press "Log in"
    And I wait for "1" seconds

  Scenario: User clicks Add and Saves tests
    Given I am on "/admin/behat/add"
    Then I should see "This is a tool to help to generate "
    And I fill in "filename" with "tests_of_tests.feature"
    And I fill in "feature" with "My Feature Name"
    And I press "feature_button"
    Then I should see "Feature: My Feature Name"
    And I fill in "edit-scenario" with "Hello Worlds"
    And I press "scenario_button"
    And I fill in "edit-url" with "http://en.wikipedia.org/wiki/Main_Page"
    And I press "Add"
    And I fill in "see_not_see_some_text" with "Wiki"
    And I press "see_not_see"
    And I follow "Run Test"
    And I wait for "3" seconds
    And I should see "Test successful!"
    And I fill in sectionOneTag
    And I fill in featuresTag
    And I fill in "see_not_see_some_text" with "Bob"
    And I press "see_not_see"
    And I follow "Run Test"
    And I wait for "4" seconds
    And I should see "Test successful!"
    Then I follow "Save New Test"
    And I wait for "1" seconds
    And I press "Continue"
    And I wait for "3" seconds
    Then I should see "has been saved"
    And I should see "tests_of_tests.feature"
    And I follow "Run Test"
    And I wait for "1" seconds
    And I follow savedTest
    Then I should not see "This is a tool to help"
    And I should see "@local"
    And I should see "Hello Worlds"
    And I should see "@readonly"

  Scenario: Admin Index page Edit and Delete
    Given I am on "/admin/behat/index"
    Then I follow "tests_of_tests.feature"
    And I wait for "1" seconds
    And I follow "Edit Test"
    And I wait for "3" seconds
    And I follow "Delete Test"
    And I wait for "2" seconds
    Then I press "Confirm"
    And I wait for "1" seconds
    Then I should see "File deleted"

  @not_done
  Scenario: Admin Index Test File Upload
    Given I am on "/admin/behat/index"
    And I follow "Upload a test"
    And I wait for "1" seconds
    Then I should see "Filename must end in .feature"


