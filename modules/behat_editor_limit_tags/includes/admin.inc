<?php
/**
 * @file admin interface
 */

function behat_editor_limit_tags_form($form, &$form_state) {
    $form['behat_editor_limit_tags_allowed_tags'] = array(
        '#type' => 'textarea',
        '#title' => t('Tags'),
        '#description' => t("Enter 1 tag per line \n. Must start with @ eg @tag1 \n @tag2"),
        '#default_value' => _behat_editor_tags_get_tags(),
    );

    $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save Tags'),
    );

    return $form;
}

function behat_editor_limit_tags_form_validate($form, &$form_state) {
    $matches = array();
    $spaces = array();
    //Make sure all tags start with @
    $tags = $form_state['values']['behat_editor_limit_tags_allowed_tags'];
    $array = _behat_editor_tags_create_array($tags);
    foreach($array as $key) {
        $test = preg_match('/^@/', $key);
        if($test == 0) {
            $matches[] = $key;
        }
    }
    if(count($matches)) {
        form_set_error('behat_editor_limit_tags_allowed_tags', t('Please make sure your tags start with @. @tag was missing at @ sign', array("@tag" => implode(', ', $matches))));
    }
    //Make sure there are no other characters
    foreach($array as $key) {
        $test = preg_match('/[\s,\,,\"]/', $key);
        if($test == 1) {
            $spaces[] = $key;
        }
    }
    if(count($spaces)) {
        form_set_error('behat_editor_limit_tags_allowed_tags', t('Please make sure your tags do not have spaces, commas etc. These @tag had issues.', array("@tag" => implode(', ', $spaces))));
    }
}

function behat_editor_limit_tags_form_submit($form, &$form_state) {
    if(!empty($form_state['values']['behat_editor_limit_tags_allowed_tags'])) {
        $form_state['values']['behat_editor_limit_tags_allowed_tags'] = _behat_editor_tags_create_array($form_state['values']['behat_editor_limit_tags_allowed_tags']);
    } else {
        $form_state['values']['behat_editor_limit_tags_allowed_tags'] = array();
    }
    system_settings_form_submit($form, $form_state);
    return system_settings_form($form);
}

/**
 * Render tags needed for the form
 */
function _behat_editor_tags_get_tags(){
    $tags = variable_get('behat_editor_limit_tags_allowed_tags', array());

    return implode("\n", $tags);
}

/**
 * Turn form into array
 *
 * @todo easier way to remove \r ?
 */
function _behat_editor_tags_create_array($values) {
    $values_array = explode("\n", $values);
    foreach($values_array as $key) {
        $new_array[] = trim($key, " \t\n\r\0\x0B");
    }
    return $new_array;
}