<?php

use Drupal\BehatEditor;

/**
 * @file
 *   Show reports for pages
 */

function behat_editor_report() {
    $arg = arg(3);
    if(!empty($arg)) {
        composer_manager_register_autoloader();
        $path = drupal_get_path('module', 'behat_editor');
        $loader     = new Twig_Loader_Filesystem($path . '/twig/');
        $twig       = new Twig_Environment($loader);

        $rids = array_slice(arg(), 3);
        $build['intro'] = array(
            '#markup' => t('<h3>Below are the results for the following tests <small>@tests</small></h3>', array('@tests' => "#" . implode(' #', $rids)))
        );

        $results = BehatEditor\Results::getResultsForRids($rids);
        if($results['results']) {
            foreach($results['results'] as $key => $value) {
                dpm($value);
                //If batch test then want to grab the files / tags being used
                if(strpos($value['filename'], '|')) {
                    //Get Row from Batch Report
                    //if tag then show that tags
                    $batch_rid = explode('|', $value['filename']);
                    $batch_row = BehatEditor\ResultsBatch::getResultsByRid($batch_rid[1]);
                    if($batch_row['results']) {
                        $type = $batch_row['results']['method'];
                        $using = theme('item_list', array('items' => unserialize($batch_row['results']['operations'])));
                    }
                    //if files show the files
                    $title = "<h3>This was part of a batch process #{$batch_rid[1]}</h3>";
                    $title .= "<p class=\"lead\">The type was {$type} using {$using}</p>";

                } else {
                    $title = "<h4>Filename: {$value['filename']}</h4>";
                }
                $output = $title;
                list($browser, $version) = explode('|', $value['settings']['browser_version']);
                $browser = ucwords($browser);
                $group   = $value['url']['gid'];
                if ($group != 0 ) {
                    $group = node_load($group)->title;
                } else {
                    $group = "Ran as user";
                }
                $created = date('m-d-Y H:i ', $value['created']);
                $os_version = $value['settings']['os_version'];
                $url        = $value['url']['base_url'];
                $nice_name  = $value['url']['nice_name'];
                $mail       = $value['user']->mail;
                $results    = $value['results'];
                $output = $twig->render('reports.html', compact('title', 'created', 'browser', 'version', 'group', 'os_version', 'url', 'nice_name'. 'mail', 'results'));
                $build[$key] = array(
                    '#markup' => $output,
                );
            }
        }

        return $build;
    } else {
        return drupal_not_found();
    }
}