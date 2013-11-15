<?php
use Drupal\BehatEditor;

function behat_settings_base_url_group() {
    composer_manager_register_autoloader();
    global $user;
    $groups = new BehatEditor\BehatPermissions($user->uid);

    dpm($groups->getGroupIDs());

    $build['intro'] = array(
        '#markup' => t('You are here')
    );

    //Get Settings for User eg uid = $user->uid and gid = 0
    $behatSettings = new BehatEditor\BehatSettingsBaseUrl();

    $userSettings = $behatSettings->getSettingsByUID($user->uid);
    dpm($userSettings);

    $build['user_settings'] = drupal_get_form('behat_editor_base_url_settings_group', $userSettings);

    //Get settings for Gid if applicable

    return $build;
}



function behat_editor_base_url_settings_group($form, $userSettings) {
    dpm($userSettings);

    $header = array(
        'sid' => array('data' => t('Settings ID')),
        'nice_name' => array('data' => t('Name')),
        'url' => array('data' => t('URL')),
        'default' => array('data' => t('Default')),
        'active' => array('data' => t('Active')),
    );

    $rows = array();

    $build['dashboard'] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $rows,
        '#empty' => t('No Settings'),
    );

    $options = array(
        'delete' => t('Delete'),
        'add' => t('Add'),
        'update' => t('Update'),
    );

    $build['operations']['operation'] = array(
        '#type' => 'select',
        '#title' => t('Operation'),
        '#title_display' => 'invisible',
        '#options' => $options,
    );

    $build['operations']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit User Settings'),
        '#validate' => array('behat_editor_base_url_settings_group_validate'),
        '#submit' => array('behat_editor_base_url_settings_group_submit'),
    );

    return $build;
}

function behat_editor_base_url_settings_group_validate($form, &$form_state) {

}

function behat_editor_base_url_settings_group_submit($form, &$form_state) {

}

