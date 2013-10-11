<?php

/**
 * @file the services resrources
 *
 */


/**
 * Implementation of hook_services_resources().
 */
function behat_editor_services_resources() {
    $path = drupal_get_path('module', 'behat_editor');
    $resources = array(
        'behat_editor_actions' => array(
            'retrieve' => array(
                'help' => t('This method returns a job.'),
                'file' => array('type' => 'inc', 'module' => 'behat_editor', 'name' => 'includes/behat_editor_view'),
                'callback' => 'behat_editor_view_form_service',
                'access callback' => '_behat_rest',
                'args' => array(
                    array(
                        'name' => 'module',
                        'type' => 'string',
                        'description' => t('The id of the job to get.'),
                        'source' => array('path' => '0'),
                        'optional' => FALSE,
                    ),
                    array(
                        'name' => 'filename',
                        'type' => 'string',
                        'description' => t('The id of the job to get.'),
                        'source' => array('path' => '1'),
                        'optional' => FALSE,
                    ),
                ),
            ),
            'create' => array(
                'help' => t('This method returns a job.'),
                'file' => array('type' => 'inc', 'module' => 'behat_editor', 'name' => 'includes/behat_editor_view'),
                'callback' => 'behat_editor_view_form_service',
                'access callback' => '_behat_rest',
                'args' => array(
                    array(
                        'name' => 'module',
                        'type' => 'string',
                        'description' => t('The id of the job to get.'),
                        'source' => array('path' => '0'),
                        'optional' => FALSE,
                    ),
                    array(
                        'name' => 'filename',
                        'type' => 'string',
                        'description' => t('The id of the job to get.'),
                        'source' => array('path' => '1'),
                        'optional' => FALSE,
                    ),
                ),
            ),
            'update' => array(
                'help' => t('This method returns a job.'),
                'file' => array('type' => 'inc', 'module' => 'behat_editor', 'name' => 'includes/behat_editor_view'),
                'callback' => 'behat_editor_view_form_service',
                'access callback' => '_behat_rest',
                'args' => array(
                    array(
                        'name' => 'module',
                        'type' => 'string',
                        'description' => t('The id of the job to get.'),
                        'source' => array('path' => '0'),
                        'optional' => FALSE,
                    ),
                    array(
                        'name' => 'filename',
                        'type' => 'string',
                        'description' => t('The id of the job to get.'),
                        'source' => array('path' => '1'),
                        'optional' => FALSE,
                    ),
                ),
            ),
            'delete' => array(
                'help' => t('This method returns a job.'),
                'file' => array('type' => 'inc', 'module' => 'behat_editor', 'name' => 'includes/behat_editor_view'),
                'callback' => 'behat_editor_view_form_service',
                'access callback' => '_behat_rest',
                'args' => array(
                    array(
                        'name' => 'module',
                        'type' => 'string',
                        'description' => t('The id of the job to get.'),
                        'source' => array('path' => '0'),
                        'optional' => FALSE,
                    ),
                    array(
                        'name' => 'filename',
                        'type' => 'string',
                        'description' => t('The id of the job to get.'),
                        'source' => array('path' => '1'),
                        'optional' => FALSE,
                    ),
                ),
            ),
            'index' => array(
                'help' => t('Shows all module features files'),
                'file' => array('type' => 'inc', 'module' => 'behat_editor', 'name' => 'includes/behat_editor_index'),
                'callback' => 'behat_editor_index',
                'access callback' => 'user_access',
                'access arguments' => array('behat run test'),
            ),
            'targeted_actions' => array(
                'run' => array(
                    'help' => 'Retrieves a Behat Test File',
                    'file' => array('type' => 'inc', 'module' => 'behat_editor', 'name' => 'includes/behat_editor_run'),
                    'callback' => 'behat_editor_run_api',
                    'access callback' => '_behat_rest',
                    'access arguments' => array('behat run test'),
                    'access arguments append' => TRUE,
                    'args' => array(
                        array(
                            'name' => 'action',
                            'type' => 'string',
                            'description' => 'The name of the file',
                            'source' => array('path' => 1),
                            'optional' => FALSE,
                        ),
                        array(
                            'name' => 'module',
                            'type' => 'string',
                            'description' => 'The name of the file',
                            'source' => array('path' => 2),
                            'optional' => FALSE,
                        ),
                        array(
                            'name' => 'filename',
                            'type' => 'string',
                            'description' => 'The name of the file',
                            'source' => array('path' => 3),
                            'optional' => FALSE,
                        ),
                    ),
                ),
                'create_run' => array(
                    'help' => 'Create the test and run',
                    'file' => array('type' => 'inc', 'module' => 'behat_editor', 'name' => 'includes/behat_editor_run'),
                    'callback' => 'behat_editor_create_and_run_api',
                    'access callback' => '_behat_rest',
                    'access arguments' => array('behat edit test'),
                    'access arguments append' => TRUE,
                    'args' => array(
                        array(
                            'name' => 'action',
                            'type' => 'string',
                            'description' => 'action',
                            'source' => array('path' => 1),
                            'optional' => FALSE,
                        ),
                        array(
                            'name' => 'module',
                            'type' => 'string',
                            'description' => 'The name of the file',
                            'source' => array('path' => 2),
                            'optional' => FALSE,
                        ),
                        array(
                            'name' => 'filename',
                            'type' => 'string',
                            'description' => 'The name of the file',
                            'source' => array('path' => 3),
                            'optional' => FALSE,
                        ),
                        array(
                            'name' => 'scenario',
                            'type' => 'string',
                            'description' => 'Informs Run that what steps to take eg view or create then view.',
                            'source' => array('data' => 'scenario'),
                            'optional' => FALSE,
                        ),
                    ),
                ),
            ),
        ),
    );
    return $resources;
}

function _behat_rest($arg){
    if(user_access($arg)) {
        return TRUE;
    } else {
        return FALSE;
    }
}
