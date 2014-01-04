<?php
use Drupal\BehatEditor;

function behat_settings_base_url() {
    composer_manager_register_autoloader();
    global $user;
    $groups = new BehatEditor\BehatPermissions($user->uid);

    $build['intro'] = array(
        '#markup' => t('You are here')
    );


    return $build;
}