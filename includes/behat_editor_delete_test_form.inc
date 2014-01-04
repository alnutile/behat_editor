<?php

/**
 * Implements hook_form().
 *
 * Create the form to add your Saucelab info
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function behat_editor_saucelabs_admin_form($form, &$form_state) {

        $form['are_you_sure'] = array(
            '#markup' => t('Are you sure you want to delete this test and it\'s reports?'),
        );

        $form['path'] = array(
            '#type' => 'hidden',
            '#default_value' => 'from/session',
        );

        $form['behat_editor_saucelabs_hide_run'] = array(
            '#type' => 'button',
            '#title' => t('Delete'),
            '#attributes' => array('class' => array('btn', 'btn-danger', 'delete')),
        );

        return system_settings_form($form);
}