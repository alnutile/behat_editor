@feature_tag
Feature: View Page

  @test @tag
  Scenario: User clicks link to see view
    Given I am on "/admin/behat/index"
    And I follow "view"
    Then I should see "Feature: aa_mock.feature"
    Then I should see "Anonymous user makes tests and saves it"
    Then I should see "Your results will show here..."


