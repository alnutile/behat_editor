@javascript
Feature: Saving and Running Tests
  Should allow the user to make a test, run and save a file
  User of the website both anonymous and authenticated
  The user can come to the site, make a tests, run and save it

  @anonymous
  Scenario: Anonymous user makes tests and saves it
    Given I am on "/node/add/gherkin-generator"
    And I follow "click here"
    And I press "Save Test"
    Then I wait
    Then I should see "File created"

  @anonymous
  Scenario: User fills in form
    Given I am on "/node/add/gherkin-generator"
    And I fill in "form_field_and_fill_in" with "name"
    And I fill in "field_with_text" with "Hello World"
    And I press "edit-field-text-button"
    And I fill in "field_with_text" with "Goodbye World"
    Then I wait
    Then I should see "Hello World"

  @anonymous
  Scenario: Anonymous user adds 2nd scenario
    Given I am on "/node/add/gherkin-generator"
    And I follow "click here"
    And I should see "Scenario: WikiPedia"
    And I fill in "name" with "Mink Rocks"
    And I press "Name it"
    Then I should see "Scenario: Mink Rocks"
    And I should see "Scenario: WikiPedia"

  @anonymous
  Scenario: Anonymous user adds looks at file
    Given I am on "/node/add/gherkin-generator"
    Then I should see "This is a tool to help"
    And I follow "click here"
    And I press "Save Test"
    Then I wait
    Then I should see "File created"
    And I follow savedTest
    Then I should see "Scenario: WikiPedia"
    Then I should not see "This is a tool to help"

  @anonymous
  Scenario: Anonymous user adds feature tag
    Given I am on "/node/add/gherkin-generator"
    Then I should see "This is a tool to help"
    And I fill in "name" with "Mink Rocks"
    And I press "Name it"
    And I fill in featuresTag
    And I press "Save Test"
    And I wait
    Then I should see "File created"
    And I follow savedTest
    Then I should see "@tag1"
    Then I should not see "This is a tool to help"

  #not ideal I need to hit the second tag box
  @anonymous
  Scenario: 2 Tags 2 Scenarios
    Given I am on "/node/add/gherkin-generator"
    Then I should see "This is a tool to help"
    And I fill in featuresTag
    And I fill in "name" with "Mink Rocks"
    And I press "Name it"
    And I fill in sectionOneTag
    And I fill in "name" with "Testing Rocks"
    And I press "Name it"
    And I fill in sectionTwoTag
    And I press "Save Test"
    And I wait
    Then I should see "File created"
    And I follow savedTest
    Then I should see "@tag1"
    Then I should see "@tag2"
    Then I should not see "This is a tool to help"

  @anonymous @runtest @thisone
  Scenario: User runs a test
    Given I am on "/node/add/gherkin-generator"
    And I fill in "name" with "Run Test Test"
    And I fill in "url" with "/node/add/gherkin-generator"
    And I fill in "see_not_see_some_text" with "Success"
    And I press "Run Test"
    And I wait
    And I should see "Test successful!"
