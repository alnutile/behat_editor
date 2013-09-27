@javascript
Feature: Run Tests on SauceLabs

  Scenario: User runs a test
    Given I am on "/admin/behat/view/behat_editor/aa_mock.feature"
    And I follow "Run on Sauce Labs"
    And I wait
    And I wait
    And I should see "Connecting to Saucelabs and waiting"

  @thisone
  Scenario: User Sees button on different Pages
    Given I am on "/admin/behat/view/behat_editor/aa_mock.feature"
    And I should see "Run on Sauce Labs"
    Given I am on "/admin/behat/edit/behat_editor/aa_mock.feature"
    And I should see "Run on Sauce Labs"
    Given I am on "/admin/behat/add"
    And I should see "Run on Sauce Labs"

