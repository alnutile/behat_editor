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
            'operations' => array(
                'retrieve' => array(
                    'help' => t('Method to return a test file'),
                    'callback' => 'behat_editor_view_service',
                    'file' => array('type' => 'inc', 'module' => 'behat_editor', 'name' => 'includes/behat_editor_view'),
                    'access callback' => '_behat_rest',
                    'access arguments' => array('behat view test'),
                    'access arguments append' => TRUE,
                    'args' => array(
                        array(
                            'name' => 'module',
                            'type' => 'string',
                            'description' => t('Name of Module'),
                            'source' => array('path' => '0'),
                            'optional' => FALSE,
                        ),
                        array(
                            'name' => 'filename',
                            'type' => 'string',
                            'description' => t('Name of file.'),
                            'source' => array('path' => '1'),
                            'optional' => FALSE,
                        ),
                    ),
                ),
                'create' => array(
                    'help' => t('Method to create a test file.'),
                    'callback' => 'behat_editor_create_api',
                    'file' => array('type' => 'inc', 'module' => 'behat_editor', 'name' => 'includes/behat_editor_add'),
                    'access callback' => '_behat_rest',
                    'access arguments' => array('behat add test'),
                    'access arguments append' => FALSE,
                    'args' => array(
                        array(
                            'name' => 'data',
                            'type' => 'struct',
                            'description' => t('Name of Module'),
                            'source' => 'data',
                            'optional' => FALSE,
                        ),
                    ),
                ),
                'update' => array(
                    'help' => t('Method to update a test file'),
                    'callback' => 'behat_editor_update_api',
                    'file' => array('type' => 'inc', 'module' => 'behat_editor', 'name' => 'includes/behat_editor_save'),
                    'access callback' => '_behat_rest',
                    'access arguments' => array('behat edit test'),
                    'access arguments append' => FALSE,
                    'args' => array(
                        array(
                            'name' => 'module',
                            'type' => 'string',
                            'description' => t('Name of Module'),
                            'source' => array('path' => '0'),
                            'optional' => FALSE,
                        ),
                        array(
                            'name' => 'filename',
                            'type' => 'string',
                            'description' => t('Name of file.'),
                            'source' => array('path' => '1'),
                            'optional' => FALSE,
                        ),
                        array(
                            'name' => 'data',
                            'type' => 'struct',
                            'description' => t('Scenario'),
                            'source' => 'data',
                            'optional' => FALSE,
                        ),
                    ),
                ),
                'delete' => array(
                    'help' => t('Method to delete a test file'),
                    'callback' => 'behat_editor_delete_api',
                    'file' => array('type' => 'inc', 'module' => 'behat_editor', 'name' => 'includes/behat_editor_delete'),
                    'access callback' => '_behat_rest',
                    'access arguments' => array('behat delete test'),
                    'access arguments append' => TRUE,
                    'args' => array(
                        array(
                            'name' => 'module',
                            'type' => 'string',
                            'description' => t('Name of Module'),
                            'source' => array('path' => '0'),
                            'optional' => FALSE,
                        ),
                        array(
                            'name' => 'filename',
                            'type' => 'string',
                            'description' => t('Name of file.'),
                            'source' => array('path' => '1'),
                            'optional' => FALSE,
                        ),
                    ),
                ),
                'index' => array(
                    'help' => t('Shows all module features files'),
                    'callback' => 'behat_editor_index_api',
                    'file' => array('type' => 'inc', 'module' => 'behat_editor', 'name' => 'includes/behat_editor_index'),
                    'access callback' => '_behat_rest',
                    'access arguments' => array('behat view test'),
                    'access arguments append' => TRUE,
                ),
            ),
            'targeted_actions' => array(
                'run' => array(
                    'help' => 'Runs the file in the module folder specified',
                    'file' => array('type' => 'inc', 'module' => 'behat_editor', 'name' => 'includes/behat_editor_run'),
                    'callback' => 'behat_editor_run_api',
                    'access callback' => '_behat_rest',
                    'access arguments' => array('behat run test'),
                    'access arguments append' => TRUE,
                    'args' => array(
                        array(
                            'name' => 'action',
                            'type' => 'string',
                            'description' => 'The name of the action eg run @todo why is this here?',
                            'source' => array('path' => 1),
                            'optional' => FALSE,
                        ),
                        array(
                            'name' => 'module',
                            'type' => 'string',
                            'description' => 'The name of the module',
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
                            'name' => 'settings',
                            'type' => 'array',
                            'description' => 'USid and GSid ids for the related behat settings',
                            'source' => array('data' => 'settings'),
                            'optional' => FALSE,
                        ),
                    ),
                ),
                'create_run' => array(
                    'help' => 'Runs the file but first saves any changes.',
                    'file' => array('type' => 'inc', 'module' => 'behat_editor', 'name' => 'includes/behat_editor_run'),
                    'callback' => 'behat_editor_create_and_run_api',
                    'access callback' => '_behat_rest',
                    'access arguments' => array('behat edit test'),
                    'access arguments append' => TRUE,
                    'args' => array(
                        array(
                            'name' => 'action',
                            'type' => 'string',
                            'description' => 'Name of the action eg create_run @todo why is this here?',
                            'source' => array('path' => 1),
                            'optional' => FALSE,
                        ),
                        array(
                            'name' => 'module',
                            'type' => 'string',
                            'description' => 'The name of the module',
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
                            'type' => 'array',
                            'description' => 'Informs Run that what steps to take eg view or create then view.',
                            'source' => array('data' => 'scenario'),
                            'optional' => FALSE,
                        ),
                        array(
                            'name' => 'settings',
                            'type' => 'array',
                            'description' => 'USid and GSid ids for the related behat settings',
                            'source' => array('data' => 'settings'),
                            'optional' => FALSE,
                        ),
                    ),
                ),
                'results' => array(
                    'help' => 'Show results related to a test',
                    'file' => array('type' => 'inc', 'module' => 'behat_editor', 'name' => 'includes/behat_editor_results'),
                    'callback' => 'behat_editor_results_api',
                    'access callback' => '_behat_rest',
                    'access arguments' => array('behat view test'),
                    'access arguments append' => TRUE,
                    'args' => array(
                        array(
                            'name' => 'action',
                            'type' => 'string',
                            'description' => 'The name of the action eg results @todo why is this here?',
                            'source' => array('path' => 1),
                            'optional' => FALSE,
                        ),
                        array(
                            'name' => 'module',
                            'type' => 'string',
                            'description' => 'The name of the module',
                            'source' => array('path' => 2),
                            'optional' => FALSE,
                        ),
                        array(
                            'name' => 'filename',
                            'type' => 'string',
                            'description' => 'The name of the file',
                            'source' => array('path' => 3),
                            'optional' => FALSE,
                        )
                    ),
                ),
            ),
        ),
    );
    return $resources;
}

function _behat_editor_coming_soon($module, $filename, $query) {
    watchdog('test_query', print_r($query, 1));
    return "Coming soon";
}

function _behat_rest($arg){
    if(user_access($arg)) {
        return TRUE;
    } else {
        return FALSE;
    }
}
