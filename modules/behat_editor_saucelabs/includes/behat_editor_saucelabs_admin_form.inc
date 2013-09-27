<?php

function behat_editor_saucelabs_admin_form($form, &$form_state) {

        $form['behat_editor_saucelabs_username'] = array(
            '#type' => 'textfield',
            '#title' => t('SauceLabs Username'),
            '#description' => t('This is your username. Not your email'),
            '#default_value' => variable_get('behat_editor_saucelabs_username', ''),
        );


        $form['behat_editor_saucelabs_api'] = array(
            '#type' => 'textfield',
            '#title' => t('SauceLabs API Key'),
            '#description' => t('You can see on this on the left lower side of the Dashboard or Account Page'),
            '#default_value' => variable_get('behat_editor_saucelabs_api', ''),
        );

        return system_settings_form($form);
}