Feature: Index Page
  Show admin like interface for a user
  User who has permission to see index
  User should be able to come here and see all the files

  Scenario: User visits page and sees list of files
    Given I am on "/admin/behat/index"
    Then I should see "behat_3.feature"