<?php
/**
 * Created by PhpStorm.
 * User: alfrednutile
 * Date: 12/19/13
 * Time: 8:43 AM
 */

namespace Drupal\BehatEditor;


class FilesHelpers {

    public function __construct(){

    }

    protected  static function _hasFolder($full_path, $subpath, $name) {
        $status = array();
        if(drupal_realpath($full_path)) {
            $status['exists'] = TRUE;
            $status['writable'] = (is_writeable($full_path)) ? TRUE : FALSE;
            $nice_name = system_rebuild_module_data();
            $status['nice_name'] = $nice_name[$name]->info['name'];

            return $status;
        }
    }

    protected static function getFilesByTag(array $tag) {
        $files_found = array();
        $files = new Files();
        $files_pre = $files->getFilesArray();
        foreach($files_pre as $key => $value) {
            foreach($value as $key2 => $value2) {
                //Some tags had ending string so had to
                if(isset($value2['tags_array'])) {
                    foreach($value2['tags_array'] as $tag_key => $tag_value) {
                        if(in_array(trim($tag_value), $tag)) {
                            $files_found[$key2] = $value2;
                        }
                    }
                }
            }
        }
        return $files_found;
    }

    /**
     * @todo this method does to much
     * might not even need a scan feature since I fixed up the FilesModel
     * By this point there should be enoug info to know
     * the exact root folder eg
     * /var/www/site/current/sites/default/files/behat_tests/tests
     * or
     * /var/www/site/current/sites/all/modules/custom/xyz/behat_features
     * etc
     * @return array
     */
    protected function _behatEditorScanDirectories($params) {
        $path = $params['full_path'];
        $module_name = $params['module'];
        $file_data = array();
        $files = file_scan_directory($path, '/.*\.feature/', $options = array('recurse' => TRUE), $depth = 0);
        foreach($files as $key => $value) {
            $array_key = $key;
            $found_uri = array_slice(explode('/', $files[$key]->uri), 0, -1); //remove file name
            $base_uri = explode('/', $path);
            if(count($found_uri) > count($base_uri)) {
                $subpath = array_slice($found_uri, count($base_uri), 1);
                $subpath = $subpath[0];
                $array_key = $array_key . $subpath;
            }
            $filename = $files[$key]->filename;
            $params = array(
                'module' => null,
                'filename' => null,
                'parse_type' => 'file',
                'service_path' => array()
            );
            //$file = new FileModel($params);
            //$file_data[$array_key] = $file->get_file_info();
        }
        return $file_data;
    }

} 