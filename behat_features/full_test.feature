@javascript
Feature: Add Page

  Scenario: Log In
    Given I am on "/user/logout"
    Then I am on "/user/login"
    And I fill in "Username" with "admin"
    And I fill in "Password" with "password"
    And I press "Log in"
    And I ponder life

  Scenario: User Sees Add Page
    Given I am on "/admin/config/behat/add"
    Then I should see "This is a tool to help to generate "
    And I fill in "edit-name" with "Hello Worlds"
    And I press "Name it"
    And I fill in "edit-url" with "http://en.wikipedia.org/wiki/Main_Page"
    And I press "Add"
    And I fill in "see_not_see_some_text" with "Wiki"
    And I press "see_not_see"
    And I follow "Run Test"
    And I ponder life
    And I ponder life
    And I ponder life
    And I should see "Test successful!"
    And I fill in sectionOneTag
    And I fill in featuresTag
    And I fill in "see_not_see_some_text" with "Bob"
    And I press "see_not_see"
    And I follow "Run Test"
    And I ponder life
    And I ponder life
    And I ponder life
    And I should see "Test successful!"
    Then I follow "Save New Test"
    And I ponder life
    And I press "Continue"
    And I ponder life
    And I ponder life
    Then I should see "has been saved"
    And I follow "Run Test"
    And I ponder life
    And I follow savedTest
    Then I should not see "This is a tool to help"
    And I should see "@local"
    And I should see "Hello Worlds"
    And I should see "@readonly"
    Then I get first test name
    And I view first test
    And I ponder life
    And I follow "Edit Test"
    And I ponder life
    And I ponder life
    And I follow "Delete Test"
    And I ponder life for "2" seconds
    Then I press "Confirm"
    And I ponder life
    Then I should see "File deleted"


