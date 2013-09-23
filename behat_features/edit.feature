Feature: Edit Page

  Scenario: User clicks link to see view
    Given I am on "/admin/behat/view/behat_editor/aa_mock.feature"
    And I follow "Edit Test"
    Then I should see "Editing: aa_mock.feature"
    And I should see "Your results will show here..."
    And I should see "Feature: View Page"

