 @feature_tag @tag1 @tag2
 Feature: View Page aa_mock

   @tag3
   Scenario: User clicks link to see view
     Given I am on "/admin/behat/index"
     And I follow "aa_mock.feature"
     Then I should see "Feature: aa_mock.feature"
 
   Scenario: Mink Rocks
 
   Scenario: Testing Rocks
