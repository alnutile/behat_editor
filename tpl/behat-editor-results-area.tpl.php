<?php
    $messages =  array('Working on things...', 'Give me a moment....', 'Doing as you command...');
    $message = array_rand($messages, 1);
?>
<div class="span6 well pull-right">
    <h4>Your results will show here...</h4>
    <div id="messages">
        <div class="alert alert-success running-tests">
            <?php print $messages[$message]; ?><img src='/<?php echo drupal_get_path('module','behat_editor'); ?>/images/ajax-loader.gif'>
        </div>
        <div class="alert alert-success saving-tests">
            Saving Test... <img src='/<?php echo drupal_get_path('module','behat_editor'); ?>/images/ajax-loader.gif'>
        </div>
    </div>
    <div class='test-result'>

    </div>
</div>