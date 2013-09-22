<ul class="scenario sortable">
    <li class="ignore"><strong>Feature Tags:</strong></li>
    <li id="feature-tags" class='tag'>
        <?php print render($features_tags_value); ?>
    </li>
    <li class="ignore"><?php print render($features_tags_input); ?></li>
    <li class="feature">Feature: Tests for ?</li>
</ul>


<ul class="hidden example-test scenario sortable ui-sortable">
    <li class="tags"><span class='tag'>@example</span></li>
    <li class="feature">Feature: Example Test for WikiPedia</li>
    <li class="name"><i class="icon-move"></i> Scenario: WikiPedia</li>
    <li class="url"><i class="icon-move"></i> Given I am on "http://en.wikipedia.org/wiki/Main_Page"</li>
    <li class="form_field_and_fill_in"><i class="icon-move"> </i>And I fill in "search" with "Behavior Driven Development" <i class="remove icon-remove-circle"></i></li>
    <li class="click_on_type"><i class="icon-move"> </i>And I press "searchButton" <i class="remove icon-remove-circle"></i></li>
    <li class="should_see"><i class="icon-move"> </i>Then I should see "agile software development" <i class="remove icon-remove-circle"></i></li>
    <li class="click_on_type"><i class="icon-move"> </i>And I follow "Donate to Wikipedia" <i class="remove icon-remove-circle"></i></li>
    <li class="should_see"><i class="icon-move"> </i>Then I should see "Thanks" <i class="remove icon-remove-circle"></i></li>
</ul>