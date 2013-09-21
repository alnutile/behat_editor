Feature: View Page
  See question fully and run a test
  User who has permission to see a test
  User should be able click view on admin page to see test

  Scenario: User clicks link to see view
    Given I am on "/admin/behat/index"
    And I follow "view"
    Then I should see "Feature: anon_save.feature"
    Then I should see "Anonymous user makes tests and saves it"
    Then I should see "Your results will show here..."

