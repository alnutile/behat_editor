Feature: View Page

  Scenario: User clicks link to see view
    Given I am on "/admin/behat/index"
    And I follow "view"
    Then I should see "Feature: anon_save.feature"
    Then I should see "Anonymous user makes tests and saves it"
    Then I should see "Your results will show here..."

  @javascript
  Scenario: User clicks Run Test
    Given I am on "/admin/behat/index"
    And I follow "aa_mock.feature"
    And I wait
    Then I should see "Feature: aa_mock.feature"
    And I follow "Run Test"
    And I wait
    Then I should see "Test successful!"
