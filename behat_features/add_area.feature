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
