<?php

/**
 * @file functions needed to parse scenaro files to and from html
 */


/**
 * Parse Questions and depending on the direction
 * will output the format array/text as needed.
 *
 * @param $scenario
 * @param $direction eg to html or to file format
 * @return string format of file
 */
function _behat_editor_parse_questions($scenario, $direction = 'file') {
    $scenario_array = array();
    $count = 0;                                                                // used to get tags
    $scenario = array_values($scenario);                                       //reset keys since some unset work
    foreach($scenario as $value) {
        if($results = behat_editor_string_type(trim($value), $scenario, $count, $direction)) {
            if(array_key_exists('scenario', $results) || array_key_exists('feature', $results)) {
                $key = key($results);
                foreach($results[$key] as $row) {
                    $scenario_array[] = $row;
                }
            } else {
                $scenario_array[] = $results;
            }
        }
        $count++;
    }
    return $scenario_array;
}


/**
 * Build an array of string types
 * to process. Features, Scenarios, And, Given etc.
 *
 * @param $string
 * @return array
 */
function behat_editor_string_type($string, $scenario, $count, $direction) {
    $compare = behat_editor_string_types();
    foreach($compare as $key) {
        if ($results = $key($string, $scenario, $count, $direction)) {
            return $results;
        }
    }
}

/**
 * Breaking this true false parts into separate functions
 * Later we can consider other string values to check for
 * as well as add different languages that gherkin supports.
 */

function behat_editor_string_types() {
    $options = array('behat_editor_string_feature', 'behat_editor_string_scenario', 'behat_editor_string_steps');
    return $options;
}

/**
 * @param $string
 * Features should have trailing lines
 * 1. The benefits
 * 2. The role of the user
 * 3. The description
 * @return array
 */
function behat_editor_string_feature($string, $scenario, $count, $direction) {
    $results = array();
    $first_word = _behat_editor_pop_first_word($string);
    $options = array('Feature:');
    if(in_array($first_word, $options)) {
        switch($direction) {
            case 'file':
                $tags = array();
                $tags[0] = behat_editor_string_tags($scenario, $count - 1, 0, $direction);
                $feature_line[1] = array(
                    'string' => $string,
                    'spaces' => 0,
                    'new_line' => 0,
                    'new_line_above' =>  0,
                );
                $results['feature'] = $tags + $feature_line;
                return $results;
            case 'html_view':
                $tags = array();
                $tags[0] = behat_editor_string_tags($scenario, $count - 1, 0, $direction);
                $feature_line[1] = array(
                    'data' => $string,
                    'class' => array('feature', "spaces-none")
                );
                $results['feature'] = $tags + $feature_line;
                return $results;
            case 'html_edit':
                $tags = array();
                $tags = behat_editor_string_tags($scenario, $count - 1, 0, $direction);
                //@todo remove number key should be automatic
                $features_tags[0] = array(
                    'data' => "<strong>Feature Tags:</strong>",
                    'class' => array('ignore'),
                );

                $features_tag_input[1] = array(
                    'data' => array('features_tag_value' => array('#id' => 'features-tagit-values', '#type' => 'hidden', '#name' => 'features_tag_value', '#value'=>$tags)),
                    'class' => array('tag hidden'),
                    'id' => 'features-tags'
                );

                $features_tag_it[2] = array(
                    'data' => '<ul id="features-tagit-input"></ul><div class="help-block">Start each tag with @. Just separate by comma for more than one tags. Tags can not have spaces.</div>',
                    'class' => array('ignore'),
                );

                $feature_line[3] = array(
                    'data' => $string,
                    'class' => array('feature')
                );
                $results['feature'] = $features_tags + $features_tag_input + $features_tag_it + $feature_line;
                return $results;
        }
    }
}


function behat_editor_string_scenario($string, $scenario, $count, $direction) {
    $results = array();
    $first_word = _behat_editor_pop_first_word($string);
    $options = array('Scenario:');
    if(in_array($first_word, $options)) {
        switch($direction) {
            case 'file':
                $tags = array();
                $tags[0] = behat_editor_string_tags($scenario, $count - 1, 2, $direction);
                $scenario_line[1] = array(
                    'string' => $string,
                    'spaces' => 2,
                    'new_line' => 0,
                    'new_line_above' =>  0,
                );
                $results['scenario'] = $tags + $scenario_line;
                return $results;
            case 'html_view':
                $tags = array();
                $tags[0] = behat_editor_string_tags($scenario, $count - 1, 2, $direction);
                $scenario_line[1] = array(
                    'data' => $string,
                    'class' => array("spaces-two")
                );
                $results['scenario'] = $tags + $scenario_line;
                return $results;
            case 'html_edit':
                $tags = array();
                $tags = behat_editor_string_tags($scenario, $count - 1, 2, $direction);
                $uid = rand(100000000, 900000000);
                $scenario_tag_input[0] = array(
                    'data' => array("scenario-tags-$uid" => array('#class' => 'section-tag', '#id' => "scenario-values-$uid", '#type' => 'hidden', '#value' => $tags)),
                    //'data' => array('features_tag_value' => array('#id' => "scenario-values-$uid", '#type' => 'hidden', '#name' => 'features_tag_value', '#value'=>$tags)),
                    'class' => array('tag')
                );
                $scenario_tag_it[2] = array(
                    'data' => '<i class="icon-move pull-left"></i><ul id="scenario-input-' . $uid . '" class="tagit" data-scenario-id="'.$uid.'"></ul>',
                    'class' => array('ignore'),
                );
                $scenario_line[3] = array(
                    'data' => _behat_editor_question_wrapper($string),
                    'class' => array('name'),
                    'data-scenario-tag-box' => "scenario-values-$uid"
                );
                $results['scenario'] = $scenario_tag_input + $scenario_tag_it + $scenario_line;
                return $results;
        }

    }
}

function behat_editor_string_tags($scenario, $count, $spaces = 0, $direction) {

    if(array_key_exists($count, $scenario)) {
        $string = $scenario[$count];
        $options = array('@');
        foreach($options as $key => $value) {
            if(strpos($string, $value) !== false) {
                switch($direction) {
                    case 'file':
                        $string = str_replace(',', ' ', $string);
                        return array(
                            'string' => $string,
                            'spaces' => $spaces,
                            'new_line' => 0,
                            'new_line_above' => ($count > 1) ? 1 : 0,
                        );
                    case 'html_view':
                        $results = array(
                            'data' => $string,
                            'class' => array('tag', "spaces-$spaces")
                        );
                        return $results;
                    case 'html_edit':
                         return str_replace(' ', ', ', $string);
                }
            }
        }
    }
}

function behat_editor_string_steps($string, $parent, $count, $direction) {
    $first_word = _behat_editor_pop_first_word($string);
    $options = array('Given', 'When', 'Then', 'And', 'But');
    if(in_array($first_word, $options)) {
        switch($direction) {
            case 'file':
                return array(
                    'string' => $string,
                    'spaces' => 4,
                    'new_line' => 0,
                    'new_line_above' => 0
                );
            case 'html_view':
                return  array(
                    'data' => $string,
                    'class' => array('steps', "spaces-four")
                );
            case 'html_edit':
                return  array(
                    'data' => _behat_editor_question_wrapper($string),
                    'class' => array('steps', "spaces-four")
                );
        }
    }
}

function _behat_editor_question_wrapper($string) {
    return '<i class="icon-move pull-left"></i>' . $string . '<i class="remove icon-remove-circle"></i>';
}

function _behat_editor_pop_first_word($string) {
    $first_word = explode(' ', $string);
    return array_shift($first_word);
}


function _behat_editor_output_html_line($line) {
    return array('data' => $line, 'class' => array('test'));
}


function _behat_editor_turn_file_to_array($file) {
    $array = explode("\n", $file);
    foreach($array as $key => $value) {
        if(strlen($value) <= 1) {
            unset($array[$key]);
        }
    }
    return $array;
}

/**
 * @param $request
 *   this is an array keyed as such
 *     scenario - an array of the scenario lines one line = one array row
 *     filename -
 * @return array
 */
function _behat_editor_save_html_to_file($request, $module, $filename) {
    //@todo why was this not loaded with info file files[] call?
    $scenario_array = _behat_editor_parse_questions($request['scenario']);
    $feature = _behat_editor_create_file($scenario_array);
    global $user;
    //@todo DRY this up for either type of user and response
    if (user_access('behat add test') && $module != variable_get('behat_editor_default_folder', BEHAT_EDITOR_DEFAULT_FOLDER)) {
        /* Derived from features.admin.inc module */
        $output = _behat_editor_process_file_in_module_folder($module, $filename, $feature, $request);
        return $output;
    } else {
        $output = _behat_editor_process_file_in_tmp($filename, $feature);
        return $output;
    }
}


function _behat_editor_create_file($scenario) {

    $file = '';
    foreach($scenario as $key) {
        $new_line = _behat_editor_new_line($key['new_line']);
        $new_line_above = _behat_editor_new_line($key['new_line_above']);
        $spaces = _behat_editor_spaces($key['spaces']);
        $file = $file . "{$new_line_above}" . "{$spaces}" . $key['string'] . "{$new_line}\r\n";
    }
    return $file;
}



function _behat_editor_new_line($new_line) {
    if($new_line == 1) {
        return "\r\n";
    } else {
        return "";
    }
}

function _behat_editor_spaces($spaces) {
    $spaces_return = '';
    for($i = 0; $i <= $spaces; $i++) {
        $spaces_return = $spaces_return . " ";
    }
    return $spaces_return;
}


function _behat_editor_process_file_in_module_folder($module, $filename, $feature, $request) {
    $full_path = _behat_editor_save_path($module);
    $response = file_put_contents("{$full_path}/{$filename}", $feature);
    if($response == FALSE) {
        watchdog('behat_editor', "File could not be made...", $variables = array(), $severity = WATCHDOG_ERROR, $link = NULL);
        $output = array('message' => "Error file could not be save", 'file' => $response, 'error' => '1');
    } else {
        $gherkin_linkable_path = _behat_editor_linkable_path($module, $filename);
        $url = url($gherkin_linkable_path, $options = array('absolute' => TRUE));
        $file_url = l('click here', $url, array('attributes' => array('target' => '_blank', 'id' => array('test-file'))));
        $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
        watchdog('behat_editor', "%date File made %name", $variables = array('%date' => $date, '%name' => $filename), $severity = WATCHDOG_NOTICE, $link = $file_url);
        $output =  array('message' => t('@date: <br> File created !name to download ', array('@date' => $date, '!name' => $file_url)), 'file' => $gherkin_linkable_path, 'error' => '0');
    }
    return $output;
}

function _behat_editor_linkable_path($module, $filename) {
    $module_path = drupal_get_path('module', $module);
    return $module_path . '/' . variable_get('behat_editor_folder', BEHAT_EDITOR_FOLDER) . '/' . $filename;
}

/**
 * @todo this needs to consider non root as well
 *   eg anonymous user saves to tmp while test run?
 *   or to allow them to link to it.
 */
function _behat_editor_save_path($module) {
    $module_path = drupal_get_path('module', $module);
    return  DRUPAL_ROOT . '/' . $module_path . '/' . variable_get('behat_editor_folder', BEHAT_EDITOR_FOLDER);
}


function _behat_editor_process_file_in_tmp($filename, $feature) {
    $folder = variable_get('behat_editor_default_folder', BEHAT_EDITOR_DEFAULT_FOLDER);
    $path = file_build_uri("/{$folder}/");
    $response = file_unmanaged_save_data($feature, $path . '/' . $filename, $replace = FILE_EXISTS_REPLACE);
    if($response == FALSE) {
        watchdog('behat_editor', "File could not be made.", $variables = array(), $severity = WATCHDOG_ERROR, $link = NULL);
        $output = array('message' => "Error file could not be save", 'file' => $response, 'error' => '1');
    } else {
        $file_uri = $response;
        $file_url = l('click here', file_create_url($response), array('attributes' => array('target' => '_blank', 'id' => array('test-file'))));
        $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
        watchdog('behat_editor', "%date File made %name", $variables = array('%date' => $date, '%name' => $response), $severity = WATCHDOG_NOTICE, $link = $file_url);
        $output = array('message' => t('@date: <br> File created !name to download ', array('@date' => $date, '!name' => $file_url)), 'file' => $file_uri, 'error' => '0');
    }
    return $output;
}