 @feature_tag @tag1 @tag2
 Feature: View Page

   @tag3
   Scenario: User clicks link to see view
     Given I am on "/admin/behat/index"
     And I follow "view"
     Then I should see "Feature: aa_mock.feature"
