<?php

namespace Drupal\BehatEditor;

use Drupal\BehatEditor;

class BehatSettingsFormHelper {

    function __construct(){
        composer_manager_register_autoloader();
    }

    static function rowBuilder(array $rows, $type = 'user') {
        $rows_results = $rows['results'];
        $output = array();

        foreach($rows_results as $key => $value) {
            $output[$value['sid']]['sid'] = $value['sid'];
            $output[$value['sid']]['gid'] = $value['gid'];
            $output[$value['sid']]['base_url'] = $value['base_url'];
            $output[$value['sid']]['default_url'] = $value['default_url'];
            $output[$value['sid']]['active'] = $value['active'];
            $output[$value['sid']]['nice_name'] = l($value['nice_name'], 'admin/behat/settings/base_url/'.$type.'/edit/' . $value['sid']);
            $output[$value['sid']]['edit'] = l('edit', 'admin/behat/settings/base_url/'.$type.'/edit/' . $value['sid']);
        }

        return $output;
    }

    static function sharedValidations($form_state) {
        if (empty($form_state['values']['nice_name'])) {
            form_set_error('nice_name', t('Please enter a nice name'));
        }

        if (empty($form_state['values']['base_url'])) {
            form_set_error('base_url', t('Please enter a url'));
        }
    }

    static function behatSettingsContainer(&$form) {
        global $user;
        $form['results_area']['settings'] = array(
            '#type' => 'container',
            '#prefix' => "<div class=\"panel-default panel\">
                        <div class=\"panel-heading\">
                            <a class=\"accordion-toggle\" data-toggle=\"collapse\" data-parent=\"#accordion\" href=\"#settings\">
                                <h4 class=\"panel-title\">Settings</h4>
                            </a>
                      </div>
                      <div id=\"settings\" class=\"panel-collapse collapse\">
                        <div class=\"panel-body\">
                        <p>If you choose a group it will be the base url used.
                           Otherwise the system will default to your user's defaul base url setting.</p>
                        ",
            '#suffix' => '</div>
                    </div>'

        );

        //List of user tests to choose from
        $user_settings = BehatEditor\BehatSettingsBaseUrl::getSettingsByUID($user->uid);

        $default_sid = '';
        $options = array();
        if(empty($user_settings['results'])) {
            drupal_set_message('You do not have a default URL setup please update your !settings', array('!settings' => l('settings', 'admin/behat/settings/user')));
        } else {
            foreach($user_settings['results'] as $key => $value) {
                if($value['default_url'] == 1) {
                    $default_sid = $value['sid'];
                }
                $options[$value['sid']] = $value['nice_name'];
            }
        }


        $form['results_area']['settings']['users'] = array(
            '#type' => 'select',
            '#options' => $options,
            '#title' => t('Your base url settings'),
            '#default_value' => $default_sid,
        );

        //Get Settings for User eg uid = $user->uid and gid = 0

        $groups = array();

        $groups = new BehatEditor\BehatPermissions($user->uid);
        $groups = _behat_editor_group_gid_and_title_options_list($groups->getGroupIDs());


        $form['results_area']['settings']['group'] = array(
            '#type' => 'select',
            '#options' => $groups,
            '#title' => t('Your Groups base url settings'),
            '#empty_option' => t('--none--')
        );
    }
}