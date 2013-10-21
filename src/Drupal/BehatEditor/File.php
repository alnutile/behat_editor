<?php
/**
 * @file
 * Contains \Drupal\BehatEditor\File.
 */

namespace Drupal\BehatEditor;

/**
 * Class File
 * Methods needed to run a process the test file.
 *
 * @params request
 *   The request input
 * @params module
 *   The machine name of the module
 * @params filename
 *   The filename to process
 * @params parse_type
 *   Defines the format to create eg html-view, html-edit, file, FALSE
 *
 * @package Drupal\BehatEditor
 *
 * @todo use the File class to create the file object during __construct
 */
class File {
    public $module = '';
    public $filename = '';
    public $parse_type = '';
    public $scenario_array = array();
    public $scenario = '';
    public $feature = '';

    public function __construct($request, $module, $filename, $parse_type) {
        $this->module = $module;
        $this->filename = $filename;
        $this->parse_type = $parse_type;
        $this->scenario = (isset($request['scenario'])) ? $request['scenario'] : array();
    }

    /**
     * Save HTML and make File
     *
     * @return array
     */
    public function save_html_to_file() {
        $this->scenario_array = self::_parse_questions();
        $this->feature =  self::_create_file();
        $output = self::_figure_out_where_to_save_file();
        return $output;
    }

    /**
     * Make HTML array from a file
     *
     * @param $file_text
     * @return array
     */
    public function output_file_text_to_html_array($file_text) {
        $this->scenario = self::_turn_file_to_array($file_text);
        $this->scenario_array = self::_parse_questions();
        return $this->scenario_array;
    }

    /**
     * Build out the file_object used in most functions.
     *
     * @return array
     */
    public function get_file_info() {
        if ($this->module == BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER) {
            $sub_folder = BEHAT_EDITOR_DEFAULT_STORAGE_FOLDER;
            $files_folder =  file_build_uri("/{$sub_folder}/");
            $relative_path = url($path = file_create_url("$files_folder/$this->filename"));
            $path = drupal_realpath($files_folder);
            $full_path_with_file = $path . '/' . $this->filename;
        } else {
            $sub_folder = drupal_get_path('module', $this->module) . '/' . BEHAT_EDITOR_FOLDER;
            $relative_path = $sub_folder . '/' . $this->filename;
            $path = DRUPAL_ROOT . '/' . $sub_folder;
            $full_path_with_file = $path . '/' . $this->filename;
        }

        if(file_exists($full_path_with_file) == FALSE) {
            $message = t('The file does not exist !file', array('!file' => $full_path_with_file));
            throw new \RuntimeException($message);
        } else {
            $file_text = self::read_file($full_path_with_file);
            $file_data = array(
                'module' => $this->module,
                'filename' => $this->filename,
                'absolute_path' => $path,
                'absolute_path_with_file' => $full_path_with_file,
                'scenario' => $file_text,
                'filename_no_ext' => substr($this->filename, 0, -8),
                'relative_path' => $relative_path,
                'tags_array' => self::_tags_array($file_text, $this->module)
            );
            return $file_data;
        }
    }

    /**
     * Build out a tags array of a file
     *
     * @param $file
     * @param $module_name
     * @return array
     */
    private function _tags_array($file, $module_name) {
        $file_to_array = self::_turn_file_to_array($file);
        $tags = array();
        foreach($file_to_array as $key => $value) {
            if(strpos($value, '@') !== FALSE && !strpos($value, '"')) {
                foreach(explode(' ', $value) as $tag) {
                    if(!empty($tag)) {
                        $tags[] = $tag;
                    }
                }
            }
        }
        return $tags;
    }

    /**
     * Read file
     *
     * @param $full_path_with_file
     * @return string
     */
    public function read_file($full_path_with_file) {
        if(filesize($full_path_with_file) > 0) {
            $file_open = fopen($full_path_with_file, "r");
            $file_read = fread($file_open, filesize($full_path_with_file));
            return $file_read;
        }
    }

    /**
     * Read file
     *
     * @param $full_path_with_file
     * @return string
     */
    public function delete_file() {
            $file = self::get_file_info();
            $response = file_unmanaged_delete($file['absolute_path_with_file']);
            if($response == FALSE) {
                watchdog('behat_editor', "File could not be deleted...", $variables = array(), $severity = WATCHDOG_ERROR, $link = NULL);
                $output = array('message' => "Error file could not be deleted", 'file' => $response, 'error' => '1');
            } else {
                $gherkin_linkable_path = '';
                $url = '';
                $file_url = '';
                $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
                watchdog('behat_editor', "%date File deleted %name", $variables = array('%date' => $date, '%name' => $this->filename), $severity = WATCHDOG_NOTICE, $link = $file_url);
                $output =  array('message' => t('@date: <br> File deleted !name to download ', array('@date' => $date, '!name' => $file_url)), 'file' => $gherkin_linkable_path, 'error' => '0');
            }
            return $output;
    }


    /**
     * Quick Helper to figure out save path
     * based on permissions.
     *
     * @return array
     */
    private function _figure_out_where_to_save_file(){
        if (user_access('behat add test') && $this->module != variable_get('behat_editor_default_folder', BEHAT_EDITOR_DEFAULT_FOLDER)) {
            /* Derived from features.admin.inc module */
            $output = self::_save_file_to_module_folder();
            return $output;
        } else {
            $output = self::_save_file_to_temp_folder();
            return $output;
        }
    }


    /**
     * Save to module folder
     *
     * @return array
     */
    private function _save_file_to_module_folder() {
        $full_path = self::_save_path();
        $response = file_put_contents("{$full_path}/{$this->filename}", $this->feature);
        if($response == FALSE) {
            watchdog('behat_editor', "File could not be made...", $variables = array(), $severity = WATCHDOG_ERROR, $link = NULL);
            $output = array('message' => "Error file could not be saved", 'file' => $response, 'error' => '1');
        } else {
            $gherkin_linkable_path = self::_linkable_path($this->module, $this->filename);
            $url = url($gherkin_linkable_path, $options = array('absolute' => TRUE));
            $file_url = l('click here', $url, array('attributes' => array('target' => '_blank', 'id' => array('test-file'))));
            $date = format_date(time(), $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
            watchdog('behat_editor', "%date File made %name", $variables = array('%date' => $date, '%name' => $this->filename), $severity = WATCHDOG_NOTICE, $link = $file_url);
            $output =  array('message' => t('@date: <br> File created !name to download ', array('@date' => $date, '!name' => $file_url)), 'file' => $gherkin_linkable_path, 'error' => '0');
        }
        return $output;
    }

    /**
     * Make a linkable path to the file.
     *
     * @return string
     */
    private function _linkable_path() {
        $module_path = drupal_get_path('module', $this->module);
        return $module_path . '/' . variable_get('behat_editor_folder', BEHAT_EDITOR_FOLDER) . '/' . $this->filename;
    }

    /**
     * Make a save path for the file
     *
     * @return string
     */
    private function _save_path() {
        $module_path = drupal_get_path('module', $this->module);
        return  DRUPAL_ROOT . '/' . $module_path . '/' . variable_get('behat_editor_folder', BEHAT_EDITOR_FOLDER);
    }

    private function _save_file_to_temp_folder() {
        $folder = variable_get('behat_editor_default_folder', BEHAT_EDITOR_DEFAULT_FOLDER);
        $path = file_build_uri("/{$folder}/");
        $response = file_unmanaged_save_data($this->feature, $path . '/' . $this->filename, $replace = FILE_EXISTS_REPLACE);
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

    /**
     * Turn the file into an array
     *
     * @return array
     */
    private function _parse_questions(){
        $scenario_array = array();
        $count = 0;                                                                      // used to get tags
        $direction = $this->parse_type;
        $scenario = array_values($this->scenario);                                       //reset keys since some unset work
        foreach($scenario as $value) {
            if($results = self::_string_type(trim($value), $scenario, $count, $direction)) {
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
     * Quick helper to turn a file into a simple array
     *
     * @param $file
     * @return array
     */
    private function _turn_file_to_array($file) {
        $array = explode("\n", $file);
        foreach($array as $key => $value) {
            if(strlen($value) <= 1) {
                unset($array[$key]);
            }
        }
        return $array;
    }

    /**
     * @param $string
     * @param $scenario
     * @param $count
     * @param $direction
     * @return mixed
     */
    private function _string_type($string, $scenario, $count, $direction){
        $compare = self::_string_types();
        foreach($compare as $key) {
            if ($results = self::$key($string, $scenario, $count, $direction)) {
                return $results;
            }
        }
    }

    /**
     * Different functions used to parse the file
     * Later we can add scenario_outline, background etc.
     *
     * @return array
     */
    private function _string_types() {
        $options = array('behat_editor_string_feature', 'behat_editor_string_scenario', 'behat_editor_string_steps');
        return $options;
    }

    /**
     * Search for the Feature text
     *
     * @param $string
     * @param $scenario
     * @param $count
     * @param $direction
     * @return array
     */
    private function behat_editor_string_feature($string, $scenario, $count, $direction) {
        $results = array();
        $first_word = self::_pop_first_word($string);
        $options = array('Feature:');
        if(in_array($first_word, $options)) {
            switch($direction) {
                case 'file':
                    $tags = array();
                    $tags[0] = self::_string_tags($scenario, $count - 1, 0, $direction);
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
                    $tags[0] = self::_string_tags($scenario, $count - 1, 0, $direction);
                    $feature_line[1] = array(
                        'data' => $string,
                        'class' => array('feature', "spaces-none")
                    );
                    $results['feature'] = $tags + $feature_line;
                    return $results;
                case 'html_edit':
                    $tags = array();
                    $tags = self::_string_tags($scenario, $count - 1, 0, $direction);
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

    /**
     * Search for the Scenario Text
     *
     * @param $string
     * @param $scenario
     * @param $count
     * @param $direction
     * @return array
     */
    private function behat_editor_string_scenario($string, $scenario, $count, $direction) {
        $results = array();
        $first_word = self::_pop_first_word($string);
        $options = array('Scenario:');
        drupal_alter('behat_editor_string_feature', $options);
        if(in_array($first_word, $options)) {
            switch($direction) {
                case 'file':
                    $tags = array();
                    $tags[0] = self::_string_tags($scenario, $count - 1, 2, $direction);
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
                    $tags[0] = self::_string_tags($scenario, $count - 1, 2, $direction);
                    $scenario_line[1] = array(
                        'data' => $string,
                        'class' => array("spaces-two")
                    );
                    $results['scenario'] = $tags + $scenario_line;
                    return $results;
                case 'html_edit':
                    $tags = array();
                    $tags = self::_string_tags($scenario, $count - 1, 2, $direction);
                    $uid = rand(100000000, 900000000);
                    $scenario_tag_input[0] = array(
                        'data' => array("scenario-tags-$uid" => array('#class' => 'section-tag', '#id' => "scenario-values-$uid", '#type' => 'hidden', '#value' => $tags)),
                        //'data' => array('features_tag_value' => array('#id' => "scenario-values-$uid", '#type' => 'hidden', '#name' => 'features_tag_value', '#value'=>$tags)),
                        'class' => array('tag')
                    );
                    $scenario_tag_it[2] = array(
                        'data' => '<i class="glyphicon glyphicon-move pull-left"></i><ul id="scenario-input-' . $uid . '" class="tagit" data-scenario-id="'.$uid.'"></ul>',
                        'class' => array('ignore'),
                    );
                    $scenario_line[3] = array(
                        'data' => self::_question_wrapper($string),
                        'class' => array('name'),
                        'data-scenario-tag-box' => "scenario-values-$uid"
                    );
                    $results['scenario'] = $scenario_tag_input + $scenario_tag_it + $scenario_line;
                    return $results;
            }

        }
    }

    /**
     * Search for Tags in Scenario
     *
     * @param $scenario_array
     * @return array
     */
    private function _parse_tags($scenario_array) {
        $tags = array();
        foreach($scenario_array as $key => $value) {
            if(strpos('@', $value)) {
                $string = str_replace(',', '', $value);
                $tags[] = explode(' ', $string);
            }
        }
        return $tags;
    }

    /**
     * Search for tags when
     * searching for Features and Scenario lines
     *
     * @param $scenario
     * @param $count
     * @param int $spaces
     * @param $direction
     * @return array|mixed
     */
    private function _string_tags($scenario, $count, $spaces = 0, $direction) {

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

    /**
     * Parse Steps
     *
     * @param $string
     * @param $parent
     * @param $count
     * @param $direction
     * @return array
     */
    private function behat_editor_string_steps($string, $parent, $count, $direction) {
        $first_word = self::_pop_first_word($string);
        $options = array('Given', 'When', 'Then', 'And', 'But');
        drupal_alter('behat_editor_string_steps', $options);
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
                        'data' => self::_question_wrapper($string),
                        'class' => array('steps', "spaces-four")
                    );
            }
        }
    }

    /**
     * Get the first word from a line
     *
     * @param $string
     * @return mixed
     */
    private function _pop_first_word($string){
        $first_word = explode(' ', $string);
        return array_shift($first_word);
    }

    /**
     * Wrap the editable questions in the close string
     *
     * @param $string
     * @return string
     */
    private function _question_wrapper($string) {
        return '<i class="glyphicon glyphicon-move pull-left"></i>' . $string . '<i class="remove glyphicon glyphicon-remove-circle"></i>';
    }

    /**
     * Format the file creation from the array
     *
     * @return string
     */
    private function _create_file(){
        $file = '';
        foreach($this->scenario_array as $key) {
            $new_line = self::_new_line($key['new_line']);
            $new_line_above = self::_new_line($key['new_line_above']);
            $spaces = self::_spaces($key['spaces']);
            $file = $file . "{$new_line_above}" . "{$spaces}" . $key['string'] . "{$new_line}\r\n";
        }
        return $file;
    }

    /**
     * New line parse
     *
     * @param $new_line
     * @return string
     */
    private function _new_line($new_line) {
        if($new_line == 1) {
            return "\r\n";
        } else {
            return "";
        }
    }

    /**
     * Spaces needed to output the HTML or file to look
     * right.
     *
     * @param $spaces
     * @return string
     */
    private function _spaces($spaces) {
        $spaces_return = '';
        for($i = 0; $i <= $spaces; $i++) {
            $spaces_return = $spaces_return . " ";
        }
        return $spaces_return;
    }
}