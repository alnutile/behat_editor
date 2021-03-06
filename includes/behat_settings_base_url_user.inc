<?php
use Drupal\BehatEditor;

function behat_settings_base_url_user() {

    $output = drupal_get_form('behat_editor_base_url_settings_user');
    return $output;
}

function behat_editor_base_url_settings_user($form, &$form_state) {
    composer_manager_register_autoloader();
    global $user;

    $build['intro'] = array(
        '#markup' => t('Manage your tests. See other tab for group related tests')
    );

    //Get Settings for User eg uid = $user->uid and gid = 0
    $behatSettings = new BehatEditor\BehatSettingsBaseUrl();

    $userSettings = $behatSettings->getSettingsByUID($user->uid);

    $header = array(
        'sid' => array('data' => t('Settings ID')),
        'nice_name' => array('data' => t('Name')),
        'base_url' => array('data' => t('URL')),
        'default_url' => array('data' => t('Default')),
        'active' => array('data' => t('Active')),
        'edit' => array('data' => t('Edit')),
    );

    $rows = BehatEditor\BehatSettingsFormHelper::rowBuilder($userSettings);

    $build['dashboard'] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $rows,
        '#empty' => t('No Settings'),
    );

    $options = array(
        'delete' => t('Delete'),
        'deactivate' => t('Set to not-active'),
        'active' => t('Set to active'),
    );

    $build['operations']['operation'] = array(
        '#type' => 'select',
        '#title' => t('Operation'),
        '#title_display' => 'invisible',
        '#options' => $options,
    );

    $build['operations']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit Settings'),
        '#validate' => array('behat_editor_base_url_settings_user_validate'),
        '#submit' => array('behat_editor_base_url_settings_user_submit'),
    );

    $build['operations']['add'] = array(
        '#type' => 'link',
        '#href' => 'admin/behat/settings/base_url/user/add',
        '#title' => t('Add New Row'),
        '#attributes' => array('class' => array('btn btn-md btn-success'))
    );

    return $build;
}

function behat_editor_base_url_settings_user_add() {
    drupal_set_message("Add a new default base_url for your user account");
    $output = drupal_get_form('behat_editor_base_url_settings_user_edit_form');
    return $output;
}

function behat_editor_base_url_settings_user_edit_form($form, &$form_state){
    global $user;
    composer_manager_register_autoloader();

    $sid = '';
    $base_url = '';
    $nice_name = '';
    $active = 1;
    $default_url = 0;
    $uid = $user->uid;
    $gid = '';
    $button = "Add";

    if(isset($form_state['build_info']['args'][0]['sid'])) {
        $sid = $form_state['build_info']['args'][0]['sid'];
        $settings = BehatEditor\BehatSettingsBaseUrl::getSettingsBySID($sid);
        foreach($settings['results'] as $key => $value) {
            $$key = $value;
        }
        //List did not work due to db order
        //list($sid, $uid, $gid, $base_url, $default_url, $active, $nice_name) = array_values($settings['results']);
        drupal_set_message(t('You are editing @name', array('@name' => $nice_name)));
        $button = "Update";
    }

    $form['sid'] = array(
        '#type' => 'hidden',
        '#default_value' => $sid,
    );

    $form['gid'] = array(
        '#type' => 'hidden',
        '#default_value' => $gid,
    );

    $form['uid'] = array(
        '#type' => 'hidden',
        '#default_value' => $uid,
    );

    $form['nice_name'] = array(
        '#type' => 'textfield',
        '#default_value' => $nice_name,
        '#description' => t('Name people will see when choosing the URL'),
        '#attributes' => array(
            'placeholder' => t('Nice Name Here...')
        )
    );

    $form['base_url'] = array(
        '#type' => 'textfield',
        '#default_value' => $base_url,
        '#description' => t('You can include basic auth as well http://admin:password@google.com'),
        '#attributes' => array(
            'placeholder' => t('http://example.com')
        )
    );

    $form['default_url'] = array(
        '#type' => 'checkbox',
        '#default_value' => $default_url,
        '#description' => t('Set to Default'),
    );

    $form['active'] = array(
        '#type' => 'checkbox',
        '#default_value' => $active,
        '#description' => t('Set to active'),
    );

    $form['add'] = array(
        '#type' => "submit",
        '#value' => t($button),
    );

    return $form;
}

/**
 * @param $form
 * @param $form_state
 *
 * @todo validate the format of the base url
 * @todo validate there are no repeats
 *
 */
function behat_editor_base_url_settings_user_edit_form_validate($form, &$form_state) {
    BehatEditor\BehatSettingsFormHelper::sharedValidations($form_state);
}

function behat_editor_base_url_settings_user_edit_form_submit($form, &$form_state) {
    composer_manager_register_autoloader();
    $settings = new BehatEditor\BehatSettingsBaseUrl();
    $settings->fields['default_url'] = $form_state['values']['default_url'];
    $settings->fields['active'] = $form_state['values']['active'];
    $settings->fields['base_url'] = $form_state['values']['base_url'];
    $settings->fields['nice_name'] = $form_state['values']['nice_name'];
    $nice_name = $settings->fields['nice_name'];
    //Update or Add
    if($form_state['values']['sid'] > 0) {
        $sid = $form_state['values']['sid'];
        $settings->fields['sid'] = $sid;
        $settings->fields['gid'] = $form_state['values']['gid'];
        $sid = $settings->update($sid);
        $messages[] = t("Your base_url @name was updated", array('@name' => $nice_name));
        $messages[] = t("!admin", array('!admin' => l('Return to admin area', 'admin/behat/settings/base_url/user')));
    } else {
        $sid = $settings->insert();
        $messages[] = t("Your base_url @name was added, !edit", array('@name' => $nice_name, '!edit' => l('edit settings further?', 'admin/behat/settings/base_url/user/edit/' . $sid)));
        $messages[] = t("!admin", array('!admin' => l('Return to admin area', 'admin/behat/settings/base_url/user')));
    }
    foreach($messages as $message) {
        drupal_set_message($message);
    }
}


function behat_editor_base_url_settings_user_edit($sid) {
    $output = drupal_get_form('behat_editor_base_url_settings_user_edit_form', array('sid' => $sid));
    return $output;
}

function behat_editor_base_url_settings_user_validate($form, &$form_state) {
    // Error if there are no items to select.
    if (!is_array($form_state['values']['dashboard']) || !count(array_filter($form_state['values']['dashboard']))) {
        form_set_error('', t('No items selected.'));
    }
}

function behat_editor_base_url_settings_user_submit($form, &$form_state) {
    composer_manager_register_autoloader();
    $operation = $form_state['values']['operation'];
    $sids = array_filter($form_state['values']['dashboard']);
    if($operation == 'delete') {
        BehatEditor\BehatSettingsBaseUrl::delete($sids);
        drupal_set_message("Your settings were deleted");
    }
}

/** HELPERS */

