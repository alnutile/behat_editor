<?php

/**
 * @file
 * wf.services.inc
 */

/**
 * Implements hook_default_services_endpoint().
 */
function behat_editor_default_services_endpoint() {
    $export = array();

    $endpoint = new stdClass();
    $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
    $endpoint->api_version = 3;
    $endpoint->name = 'behat_feature';
    $endpoint->server = 'rest_server';
    $endpoint->path = 'behat_editor_v1';
    $endpoint->authentication = array(
        'services' => 'services',
    );
    $endpoint->server_settings = array(
        'formatters' => array(
            'json' => TRUE,
            'bencode' => FALSE,
            'jsonp' => FALSE,
            'php' => FALSE,
            'xml' => FALSE,
        ),
        'parsers' => array(
            'application/json' => TRUE,
            'application/x-www-form-urlencoded' => TRUE,
            'application/vnd.php.serialized' => FALSE,
            'application/xml' => FALSE,
            'multipart/form-data' => FALSE,
            'text/xml' => FALSE,
        ),
    );
    $endpoint->resources = array(
        'behat_editor_actions' => array(
            'operations' => array(
                'create' => array(
                    'enabled' => '1',
                ),
                'retrieve' => array(
                    'enabled' => '1',
                ),
                'update' => array(
                    'enabled' => '1',
                ),
                'delete' => array(
                    'enabled' => '1',
                ),
                'index' => array(
                    'enabled' => '1',
                ),
            ),
            'targeted_actions' => array(
                'run' => array(
                    'enabled' => '1',
                ),
                'create_run' => array(
                    'enabled' => '1',
                ),
            ),
        ),
        'comment' => array(
            'operations' => array(
                'create' => array(
                    'enabled' => '1',
                ),
                'retrieve' => array(
                    'enabled' => '1',
                ),
                'update' => array(
                    'enabled' => '1',
                ),
                'delete' => array(
                    'enabled' => '1',
                ),
                'index' => array(
                    'enabled' => '1',
                ),
            ),
            'actions' => array(
                'countAll' => array(
                    'enabled' => '1',
                ),
                'countNew' => array(
                    'enabled' => '1',
                ),
            ),
        ),
        'file' => array(
            'operations' => array(
                'create' => array(
                    'enabled' => '1',
                ),
                'retrieve' => array(
                    'enabled' => '1',
                ),
                'delete' => array(
                    'enabled' => '1',
                ),
                'index' => array(
                    'enabled' => '1',
                ),
            ),
            'actions' => array(
                'create_raw' => array(
                    'enabled' => '1',
                ),
            ),
        ),
        'node' => array(
            'operations' => array(
                'retrieve' => array(
                    'enabled' => '1',
                ),
                'create' => array(
                    'enabled' => '1',
                ),
                'update' => array(
                    'enabled' => '1',
                ),
                'delete' => array(
                    'enabled' => '1',
                ),
                'index' => array(
                    'enabled' => '1',
                ),
            ),
            'relationships' => array(
                'files' => array(
                    'enabled' => '1',
                ),
                'comments' => array(
                    'enabled' => '1',
                ),
            ),
            'targeted_actions' => array(
                'attach_file' => array(
                    'enabled' => '1',
                ),
            ),
        ),
        'system' => array(
            'actions' => array(
                'connect' => array(
                    'enabled' => '1',
                ),
                'get_variable' => array(
                    'enabled' => '1',
                ),
                'set_variable' => array(
                    'enabled' => '1',
                ),
                'del_variable' => array(
                    'enabled' => '1',
                ),
            ),
        ),
        'taxonomy_term' => array(
            'operations' => array(
                'retrieve' => array(
                    'enabled' => '1',
                ),
                'create' => array(
                    'enabled' => '1',
                ),
                'update' => array(
                    'enabled' => '1',
                ),
                'delete' => array(
                    'enabled' => '1',
                ),
                'index' => array(
                    'enabled' => '1',
                ),
            ),
            'actions' => array(
                'selectNodes' => array(
                    'enabled' => '1',
                ),
            ),
        ),
        'taxonomy_vocabulary' => array(
            'operations' => array(
                'retrieve' => array(
                    'enabled' => '1',
                ),
                'create' => array(
                    'enabled' => '1',
                ),
                'update' => array(
                    'enabled' => '1',
                ),
                'delete' => array(
                    'enabled' => '1',
                ),
                'index' => array(
                    'enabled' => '1',
                ),
            ),
            'actions' => array(
                'getTree' => array(
                    'enabled' => '1',
                ),
            ),
        ),
        'user' => array(
            'operations' => array(
                'retrieve' => array(
                    'enabled' => '1',
                ),
                'create' => array(
                    'enabled' => '1',
                ),
                'update' => array(
                    'enabled' => '1',
                ),
                'delete' => array(
                    'enabled' => '1',
                ),
                'index' => array(
                    'enabled' => '1',
                ),
            ),
            'actions' => array(
                'login' => array(
                    'enabled' => '1',
                    'settings' => array(
                        'services' => array(
                            'resource_api_version' => '1.0',
                        ),
                    ),
                ),
                'logout' => array(
                    'enabled' => '1',
                    'settings' => array(
                        'services' => array(
                            'resource_api_version' => '1.0',
                        ),
                    ),
                ),
                'token' => array(
                    'enabled' => '1',
                ),
                'register' => array(
                    'enabled' => '1',
                ),
            ),
        ),
    );
    $endpoint->debug = 0;
    $export['behat_editor_services'] = $endpoint;

    return $export;
}
