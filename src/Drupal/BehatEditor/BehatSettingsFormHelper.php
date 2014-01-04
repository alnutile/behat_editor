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

    static function behatSettingsFields(&$form) {
        global $user;
        $form['settings'] = array(
            '#type' => 'container'
        );

        $form['settings']['intro'] = array(
            '#markup' => "<p class='alert alert-warning'>If you choose a Group's base_url it will be the base url used for the test.
                                   Otherwise the system will default to your user's default base url settings.
                                    <br>
                                   You can <a href='/admin/behat/settings' target='_blank'>click here</a> to manage them.
                                </p>",
        );
        //List of user tests to choose from
        $user_settings = BehatEditor\BehatSettingsBaseUrl::getSettingsByUID($user->uid);

        $default_sid = '';
        $options = array();
        if(empty($user_settings['results'])) {
            drupal_set_message(t('You do not have a default URL setup please update your !settings', array('!settings' => l('settings', 'admin/behat/settings'))), 'warning');
        } else {
            foreach($user_settings['results'] as $key => $value) {
                if($value['default_url'] == 1) {
                    $default_sid = $value['sid'];
                }
                $options[$value['sid']] = $value['nice_name'];
            }
        }

        $form['settings']['users'] = array(
            '#type' => 'select',
            '#options' => $options,
            '#title' => t('Your base url settings'),
            '#default_value' => $default_sid,
        );


        $groups_gids = new BehatEditor\BehatPermissions($user->uid);
        $groups_options = BehatSettingsBaseUrl::getSettingsByGID($groups_gids->getGroupIDs());

        $options = array();
        if(empty($groups_options['results'])) {
            drupal_set_message(t('You do not have a default Group URL setup you can, but it is not required, update your !settings', array('!settings' => l('settings', 'admin/behat/settings'))), 'error');
        } else {
            foreach($groups_options['results'] as $key => $value) {
                if($value['default_url'] == 1) {
                    $default_sid = $value['sid'];
                }
                $options[$value['sid']] = $value['nice_name'];
            }
        }

        $form['settings']['group'] = array(
            '#type' => 'select',
            '#options' => $options,
            '#title' => t('Your Groups base url settings'),
            '#empty_option' => t('--none--')
        );


        $helpers = new BehatSettingsFormHelper();
        $helpers->osBrowser($form);

    }

    public function _behat_editor_group_gid_and_title_options_list($gids) {
        //@todo assuming a node id always = gid?
        $nodes_loaded = node_load_multiple($gids);
        foreach($nodes_loaded as $key => $value) {
            $options[$value->nid] = $value->title;
        }
        return $options;
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
                            <div id=\"settings\" class=\"panel-collapse collapse in\">
                              <div class=\"panel-body\">
                                <p class='alert alert-warning'>If you choose a Group's base_url it will be the base url used for the test.
                                   Otherwise the system will default to your user's default base url settings.
                                    <br>
                                   You can <a href='/admin/behat/settings' target='_blank'>click here</a> to manage them.
                                </p>",
            '#suffix' => '</div><!--end panel-body-->
                    </div><!--end panel-collapse-->
                    </div><!--end panel-->'

        );

        //List of user tests to choose from
        $user_settings = BehatEditor\BehatSettingsBaseUrl::getSettingsByUID($user->uid);

        $default_sid = '';
        $options = array();
        if(empty($user_settings['results'])) {
            drupal_set_message(t('You do not have a default URL setup please update your !settings', array('!settings' => l('settings', 'admin/behat/settings/user'))), 'error');
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

        $groups_gids = new BehatEditor\BehatPermissions($user->uid);
        $groups_options = BehatSettingsBaseUrl::getSettingsByGID($groups_gids->getGroupIDs());

        $options = array();
        if(empty($groups_options['results'])) {
            drupal_set_message(t('You do not have a default Group URL setup you can, but it is not required, update your !settings', array('!settings' => l('settings', 'admin/behat/settings'))), 'error');
        } else {
            foreach($groups_options['results'] as $key => $value) {
                if($value['default_url'] == 1) {
                    $default_sid = $value['sid'];
                }
                $options[$value['sid']] = $value['nice_name'];
            }
        }



        $form['results_area']['settings']['group'] = array(
            '#type' => 'select',
            '#options' => $options,
            '#title' => t('Your Groups base url settings'),
            '#empty_option' => t('--none--')
        );

        $helpers = new BehatSettingsFormHelper();
        $helpers->osBrowser($form);
    }


    private function osBrowser(&$form) {
        //@todo
        // Setup default for local user to enter
        // might only need a browser actually
        // or grab from the behat.yml.default / behat.yml files?
        $form['results_area']['settings']['os_browser'] = array(
            '#type' => 'container',
            '#prefix' => t('<hr><h4>Choose an OS and a Browser to run the test on</h4><br>')
        );

        $form['results_area']['settings']['os_browser']['os'] = array(
            '#type' => 'select',
            '#options' => array('Windows 2012' => 'Windows 2012'),
            '#default_value' => 'Windows 2012',
            '#validated' => TRUE,
            '#description' => t('What OS and Browser should be used for this test')
        );

        $form['results_area']['settings']['os_browser']['browser'] = array(
            '#type' => 'select',
            '#options' => array('chrome|31' => 'Google Chrome - 31'),
            '#empty_value' => '--choose OS first--',
            '#default_value' => 'chrome|31',
            '#validated' => TRUE,
            '#description' => t('What Browser do you want to use')
        );
    }
}