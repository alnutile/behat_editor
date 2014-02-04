<?php
/**
 * @file
 * behat_editor_reports_service.services.inc
 */

/**
 * Implements hook_default_services_endpoint().
 */
function behat_editor_tokenizer_default_services_endpoint() {
    $export = array();

    $endpoint = new stdClass();
    $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
    $endpoint->api_version = 3;
    $endpoint->name = 'behat_editor_tokenizer';
    $endpoint->server = 'rest_server';
    $endpoint->path = 'behat_tokenizer_v1';
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
        'tokenizer' => array(
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
        ),
    );
    $endpoint->debug = 0;
    $export['behat_tokenizer'] = $endpoint;

    return $export;
}
