 @feature_tag @tag1 @tag2
 Feature: View Page

   @tag3 tag4
   Scenario: User clicks link to see view
     Given I am on "/admin/behat/index"
     And I follow "view"
     Then I should see "Feature: aa_mock.feature"
     Then I should see "Your results will show here..."
     Then I should see "Drupal"
