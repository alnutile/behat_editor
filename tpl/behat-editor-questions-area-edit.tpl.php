<ul class="scenario sortable" data-mode="create-mode">
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
    <li class="name"><i class="glyphicon glyphicon-move"></i> Scenario: WikiPedia</li>
    <li class="url"><i class="glyphicon glyphicon-move"></i> Given I am on "http://en.wikipedia.org/wiki/Main_Page"</li>
    <li class="should_see"><i class="glyphicon glyphicon-move"> </i>Then I should see "Wiki" <i class="remove glyphicon glyphicon-remove-circle"></i></li>
    <li class="click_on_type"><i class="glyphicon glyphicon-move"> </i>And I follow "Donate to Wikipedia" <i class="remove glyphicon glyphicon-remove-circle"></i></li>
    <li class="should_see"><i class="glyphicon glyphicon-move"> </i>Then I should see "Thanks" <i class="remove glyphicon glyphicon-remove-circle"></i></li>
</ul>