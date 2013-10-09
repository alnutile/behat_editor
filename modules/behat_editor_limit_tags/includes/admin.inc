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