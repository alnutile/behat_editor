@javascript
Feature: Run Tests on SauceLabs

  Scenario: User runs a test
    Given I am on "/admin/behat/view/behat_editor/aa_mock.feature"
    And I follow "Run on Sauce Labs"
    And I wait
    And I wait
    And I wait
    And I wait
    And I wait
    And I wait
    Then I should see "1 scenario (1 passed)"
